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
};