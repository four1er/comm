#include <coroutine>
#include <cstdlib>
#include <iostream>
#include <memory>
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
    promise_type() : value_(std::make_shared<int>()) {}

    Task get_return_object() { return Task{value_}; }

    void return_value(int value) { *value_ = value; }

    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}

   private:
    std::shared_ptr<int> value_;
  };

 public:
  Task(std::shared_ptr<int> value) : value_(value) {}
  int GetValue() { return *value_; }

 private:
  std::shared_ptr<int> value_;
};

Task GetInt() {
  IntReader reader1;
  int total = co_await reader1;

  IntReader reader2;
  total += co_await reader2;

  IntReader reader3;
  total += co_await reader3;

  co_return total;
}

int main() {
  auto task = GetInt();

  std::string line;
  while (std::cin >> line) {
    std::cout << task.GetValue() << std::endl;
  }
  return 0;
}
