#include <coroutine>
#include <cstdlib>
#include <iostream>
#include <thread>

class IntReader {
 public:
  bool await_ready() {
    std::cout << "await_ready: " << std::this_thread::get_id() << std::endl;
    return false;
  }
  void await_suspend(std::coroutine_handle<> handle) {
    std::cout << "await_suspend: " << std::this_thread::get_id() << std::endl;
    std::thread thread([this, handle]() {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::srand(std::time(nullptr));
      value_ = std::rand();
      handle.resume();
    });
    thread.detach();
  }

  int await_resume() {
    std::cout << "await_resume: " << std::this_thread::get_id() << std::endl;
    return value_;
  }

 private:
  int value_;
};

class Task {
 public:
  class promise_type {
   public:
    Task get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
    void return_void() {}
  };
};

Task PrintInt() {
  IntReader reader1;
  int total = co_await reader1;
  std::cout << "wait1: " << total << std::endl;

  IntReader reader2;
  total += co_await reader2;
  std::cout << "wait2: " << total << std::endl;

  IntReader reader3;
  total += co_await reader3;
  std::cout << "wait3: " << total << std::endl;

  std::cout << total << std::endl;
}

int main() {
  PrintInt();

  std::string line;
  std::cout << "continue executing" << std::endl;
  while (std::cin >> line) {
  }
  return 0;
}
