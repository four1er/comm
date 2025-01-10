#pragma once

#include "stl_list.h"

template <typename T, typename Contianter = list<T>>
class queue {
 public:
  using value_type = typename Contianter::value_type;
  using size_type = typename Contianter::size_type;
  using reference = typename Contianter::reference;
  using const_reference = typename Contianter::const_reference;

 protected:
  Contianter c_;

 public:
  bool empty() const { return c_.empty(); };
  size_type size() const { return c_.size(); };
  reference front() { return c_.front(); };
  const_reference front() const { return c_.front(); };
  reference back() { return c_.back(); };
  const_reference back() const { return c_.back(); };
  void push(const value_type &x) { c_.push_back(x); };
  void pop() { c_.pop_front(); };

 private:
  friend bool operator==(const queue &, const queue &);
  friend bool operator<(const queue &, const queue &);
};

template <typename T, typename Container>
bool operator==(const queue<T, Container> &x, const queue<T, Container> &y) {
  return x.c_ == y.c_;
}

template <typename T, typename Container>
bool operator<(const queue<T, Container> &x, const queue<T, Container> &y) {
  return x.c_ < y.c_;
}
