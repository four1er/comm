#pragma once

#include <algorithm>
#include <memory>

#include "iterator.h"
#include "stl_alloc.h"
#include "stl_config.h"
#include "type_traits"

template <typename T>
struct list_node {
  list_node<T*> next;
  list_node<T*> prev;
  T data;
};

template <typename T, typename Ref, typename Ptr>
struct list_iterator {
  using iterator = list_iterator<T, T&, T*>;
  using const_iterator = list_iterator<T, const T&, const T*>;
  using self = list_iterator<T, Ref, Ptr>;

  using iterator_category = bidirectional_iterator_tag;
  using value_type = T;
  using pointer = Ptr;
  using reference = Ref;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using link_type = list_node<T>*;

  link_type node;
  list_iterator(link_type x) : node(x) {}
  list_iterator() {}

  bool operator==(const self& rhs) const { return node == rhs.node; }
  bool operator!=(const self& rhs) const { return !(node == rhs.node); }
  reference operator*() { return (*node).data; }
  pointer operator->() const { return &(operator*()); }
  self& operator++() {
    node = (*node).next;
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  self& operator--() {
    node = (*node).prev;
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }
  self operator+(difference_type n) const {
    self tmp = *this;
    if (n > 0)
      for (; n != 0; --n, ++tmp)
        ;
    else
      for (; n != 0; ++n, --tmp)
        ;
    return tmp;
  }

  self operator-(difference_type n) const {
    self tmp = *this;
    difference_type i = -n;
    return (tmp + i);
  }
};