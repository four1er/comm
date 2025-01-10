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

template <typename T, typename Alloc = allocator<list_node<T>>>
class list {
 public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using const_pointer = const T*;
  using const_reference = const T&;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using link_type = list_node<T>*;
  using allocator_type = Alloc;

  using iterator = list_iterator<T, T&, T*>;
  using const_iterator = list_iterator<T, const T&, const T*>;
  using reverse_iter = reverse_iterator<iterator, T>;
  using const_reverse_iter =
      reverse_iterator<const_iterator, const T&, const T*>;

 protected:
  allocator_type allocator_;
  link_type node_;
  link_type get_node() { allocator_.allocate(); }
  void put_node(link_type ptr) { allocator_.deallocate(ptr); }
  link_type create_node(const T& x);
  void destroy_node(link_type p);
  void empty_init();
  void fill_init(size_type n, const T& value);
  template <typename InputIterator>
  void range_init(InputIterator first, InputIterator last);
  void transfer(iterator position, iterator first, iterator last);

 public:
  /* 各种构造拷贝析构函数 */
  list() { empty_init(); }
  list(size_type n, const T& value) { fill_init(n, value); }
  list(int n, const T& value) { fill_init(size_type(n), value); }
  list(long n, const T& value) { fill_init(size_type(n), value); }
  explicit list(size_type n) { fill_init(n, T()); }
  template <typename InputIterator>
  list(InputIterator first, InputIterator last) {
    range_init(first, last);
  }
  list(const list<T, Alloc>& rhs) { range_init(rhs.begin(), rhs.end()); }
  list(std::initializer_list<T> rhs) { range_init(rhs.begin(), rhs.end()); }
  ~list() {
    clear();
    put_node(node_);
  }
  list<T, Alloc>& operator=(const list<T, Alloc>& rhs);
  list<T, Alloc>& operator=(std::initializer_list<T> rhs);

  /* 迭代器相关操作 */
  iterator begin() noexcept { return node_->next; }
  const_iterator begin() const noexcept { return node_->next; }
  const_iterator cbegin() const noexcept { return begin(); }
  iterator end() noexcept { return node_; }
  const_iterator end() const noexcept { return node_; }
  const_iterator cend() const noexcept { return end(); }

  reverse_iter rbegin() noexcept { return reverse_iter(end()); }
  const_reverse_iter rbegin() const noexcept {
    return const_reverse_iter(end());
  }
  reverse_iter rend() noexcept { return reverse_iter(begin()); }
  const_reverse_iter rend() const noexcept {
    return const_reverse_iter(begin());
  }
  /* 容量相关操作 */
  bool empty() const noexcept { return node_->next == node_; }
  size_type size() const noexcept { return distance(begin(), end()); }
  size_type max_size() const noexcept { return size_type(-1); }

  /* 取值相关操作 */
  reference front() { return node_->next->data; }
  const_reference front() const { return node_->next->data; }
  reference back() { return node_->prev->data; }
  const_reference back() const { return node_->prev->data; }
  reference operator[](const size_type& n);

  /* 修改链表操作 */
  void swap(list<T, Alloc>& rhs) { std::swap(node_, rhs.node_); }
  iterator insert(iterator pos, const T& value);
  iterator insert(iterator pos);
  template <typename InputIterator>
  void insert(iterator pos, InputIterator first, InputIterator last);
  void insert(iterator pos, size_type n, const T& value);
  void insert(iterator pos, int n, const T& value);
  void insert(iterator pos, long n, const T& value);
  void push_front(const T& value) { insert(begin(), value); }
  void push_back(const T& value) { insert(end(), value); }
  iterator erase(iterator pos);
  iterator erase(iterator first, iterator last);
  void resize(size_type new_size, const T& value);
  void resize(size_type new_size) { resize(new_size, T()); }
  void clear() { erase(begin(), end()); }
  void pop_front() { erase(begin()); }
  void pop_back() { erase(--end()); }
  void splice(iterator pos, list& x);
  void splice(iterator pos, list&, iterator i);
  void splice(iterator pos, list&, iterator first, iterator last);
  void remove(const T& value);
  void unique();
  void merge(list& x);
  void reverse();
  void sort();
};
