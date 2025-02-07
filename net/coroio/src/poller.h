#pragma once

#include <assert.h>
#include <bits/types/struct_timespec.h>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "base.h"

namespace NNet {

using TMSecond = std::chrono::milliseconds;

class TPollerBase {
public:
  TPollerBase() = default;
  TPollerBase(const TPollerBase &) = delete;
  TPollerBase &operator=(const TPollerBase &) = delete;

  unsigned AddTimer(TTime deadline, THandle h) {
    Timers_.emplace(TTimer{deadline, TimerId_, h});
    return TimerId_++;
  }

  bool RemoveTimer(unsigned timerId, TTime deadline) {
    bool fired = timerId == LastFiredTimer_;
    // if timer is not fired, insert empty timer instead of removing it
    if (!fired) {
      Timers_.emplace(TTimer{deadline, timerId, THandle{}});
    }
    return fired;
  }

  void AddRead(int fd, THandle h) {
    MaxFd_ = std::max(MaxFd_, fd);
    Changes_.emplace_back(fd, TEvent::READ, h);
  }

  void AddWrite(int fd, THandle h) {
    MaxFd_ = std::max(MaxFd_, fd);
    Changes_.emplace_back(fd, TEvent::WRITE, h);
  }
  void AddRemoteHup(int fd, THandle h) {
    MaxFd_ = std::max(MaxFd_, fd);
    Changes_.emplace_back(fd, TEvent::RHUP, h);
  }

  void RemoveEvent(int fd) {
    MaxFd_ = std::max(MaxFd_, fd);
    Changes_.emplace_back(fd, TEvent::READ | TEvent::WRITE | TEvent::RHUP,
                          THandle{});
  }

  auto Sleep(TTime until) {
    struct TAwaitableSleep {
      TAwaitableSleep(TPollerBase *poller, TTime n) : poller(poller), n(n) {}
      ~TAwaitableSleep() {
        if (poller) {
          poller->RemoveTimer(timerId, n);
        }
      }

      TAwaitableSleep(const TAwaitableSleep &) = delete;
      TAwaitableSleep &operator=(const TAwaitableSleep &) = delete;
      TAwaitableSleep(TAwaitableSleep &&other) {
        poller = other.poller;
        n = other.n;
        other.poller = nullptr;
      }

      bool await_ready() { return false; }

      void await_suspend(std::coroutine_handle<> h) {
        timerId = poller->AddTimer(n, h);
      }

      void await_resume() { poller = nullptr; }

      TPollerBase *poller;
      TTime n;
      unsigned timerId = 0;
    };

    return TAwaitableSleep{this, until};
  }

  template <typename Rep, typename Period>
  auto Sleep(std::chrono::duration<Rep, Period> duration) {
    return Sleep(TTime{TClock::now() + duration});
  }

  auto Yield() { return Sleep(TTime{}); }

  void Wakeup(TEvent &&change) {
    change.Handle.resume();
    if (Changes_.empty() || !Changes_.back().Match(change)) {
      if (change.Fd >= 0) {
        change.Handle = {};
        Changes_.emplace_back(std::move(change));
      }
    }
  }

  void WakeupReadyHandles() {
    for (auto &&ev : ReadyEvents_) {
      Wakeup(std::move(ev));
    }
  }

  void SetMaxDuration(std::chrono::milliseconds maxDuration) {
    MaxDuration_ = maxDuration;
    MaxDurationTs_ = GetMaxDuration(MaxDuration_);
  }

  auto TimersSize() const { return Timers_.size(); }

private:
  timespec GetTimeout() const {
    return Timers_.empty() ? MaxDurationTs_
                           : Timers_.top().Deadline == TTime{}
               ? GetTimespec(TClock::now(), Timers_.top().Deadline,
                             MaxDuration_);
  }

  static constexpr timespec GetMaxDuration(std::chrono::milliseconds duration) {
    auto p1 = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto p2 =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration - p1);
    timespec ts;
    ts.tv_sec = p1.count();
    ts.tv_nsec = p2.count();
    return ts;
  }

  void Reset() {
    ReadyEvents_.clear();
    Changes_.clear();
    MaxFd_ = 0;
  }

  void ProcessTimers() {
    auto now = TClock::now();
    bool first = true;
    unsigned prevId = 0;

    while (!Timers_.empty() && Timers_.top().Deadline <= now) {
      TTimer timer = Timers_.top();
      Timers_.pop();

      if ((first || prevId != timer.Id) &&
          timer.Handle) { // skip removed timers
        LastFiredTimer_ = timer.Id;
        timer.Handle.resume();
      }

      first = false;
      prevId = timer.Id;
    }

    LastTimersProcessTime_ = now;
  }

  int MaxFd_ = -1;                     // current max fd
  std::vector<TEvent> Changes_;        // events to be processed
  std::vector<TEvent> ReadyEvents_;    // events ready to be processed
  unsigned TimerId_ = 0;               // timer id counter
  std::priority_queue<TTimer> Timers_; // timer queue
  TTime LastTimersProcessTime_;  // last time when timer queue was processed
  unsigned LastFiredTimer_ = -1; // last fired timer id
  TMSecond MaxDuration_ =
      TMSecond(100); // max duration of timer queue processing
  timespec MaxDurationTs_ =
      GetMaxDuration(MaxDuration_); // max duration of timer queue processing
};

} // namespace NNet
