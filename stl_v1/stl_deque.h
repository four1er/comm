#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>

#include "iterator.h"
#include "stl_alloc.h"
#include "stl_config.h"
#include "type_traits"

inline size_t deque_buf_size(size_t n, size_t sz) {
  return n != 0 ? n : (sz < 256 ? (1024 / sz) : size_t(4));
}

template <typename T, typename Ref, typename Ptr>
struct deque_iterator {
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using pointer = Ptr;
  using reference = Ref;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using map_pointer = T**;

  using iterator = deque_iterator<T, Ref, Ptr>;
  using const_iterator = deque_iterator<T, Ref, Ptr>;
  using self = deque_iterator<T, Ref, Ptr>;

  T* cur;
  T* first;
  T* last;

  map_pointer node;

  static size_t buffer_size() { return deque_buf_size(0, sizeof(T)); }

  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }

  deque_iterator(T* value_ptr, map_pointer node_ptr)
      : cur(value_ptr),
        node(node_ptr),
        first(*node_ptr),
        last(*node_ptr + buffer_size()) {}

  deque_iterator()
      : cur(nullptr), node(nullptr), first(nullptr), last(nullptr) {}

  deque_iterator(const iterator& x)
      : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

  reference operator*() const { return *cur; }
  pointer operator->() const { return &(operator*()); }

  difference_type operator-(const self& iter) const {
    return difference_type(buffer_size()) * (node - iter.node - 1) +
           (cur - first) + (iter.last - iter.cur);
  }

  self& operator++() {
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
  };

  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() {
    if (cur == first) {
      set_node(node - 1);
      cur = last;
    }
    --cur;
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

  self& operator+=(difference_type n) {
    difference_type offset = n + (cur - first);
    if (offset > 0 && offset < buffer_size()) {
      cur += n;
    } else {
      difference_type node_offset =
          offset > 0 ? offset / difference_type(buffer_size())
                     : -difference_type((-offset - 1) / buffer_size()) - 1;
      set_node(node + node_offset);
      cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
  }

  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;
  }
  self& operator-=(difference_type n) { return *this += -n; }
  self operator-(difference_type n) {
    self tmp = *this;
    return tmp -= n;
  }
  reference operator[](difference_type n) const { return *(*this + n); }

  bool operator==(const self& iter) const { return cur == iter.cur; }
  bool operator!=(const self& iter) const { return !(*this == iter); }
  bool operator<(const self& iter) const {
    return (node == iter.node) ? (cur < iter.cur) : (node < iter.node);
  }
};
