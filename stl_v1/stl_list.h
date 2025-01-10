#pragma once

#include <algorithm>
#include <initializer_list>
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

template <typename T, typename Alloc>
typename list<T, Alloc>::link_type list<T, Alloc>::create_node(const T& x) {
  link_type ptr = get_node();
  allocator_.construct(&ptr->data, x);
  return ptr;
}

template <typename T, typename Alloc>
void list<T, Alloc>::destroy_node(link_type p) {
  allocator_.destroy(&p->data);
  put_node(p);
}

template <typename T, typename Alloc>
void list<T, Alloc>::empty_init() {
  node_ = get_node();
  node_->next = node_;
  node_->prev = node_;
}

template <typename T, typename Alloc>
void list<T, Alloc>::fill_init(size_type n, const T& value) {
  empty_init();
  insert(begin(), n, value);
}

template <typename T, typename Alloc>
template <typename InputIterator>
void list<T, Alloc>::range_init(InputIterator first, InputIterator last) {
  empty_init();
  insert(begin(), first, last);
}

template <typename T, typename Alloc>
void list<T, Alloc>::transfer(iterator position, iterator first,
                              iterator last) {
  if (position != last) {
    (*last.node_).prev->next = position.node_;
    (*first.node_).prev->next = last.node_;
    (*position.node_).prev->next = first.node_;
    link_type tmp = (*position.node_).prev;
    (*position.node_).prev = (*last.node_).prev;
    (*last.node_).prev = (*first.node_).prev;
    (*first.node_).prev = tmp;
  }
}

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& rhs) {
  if (this != &rhs) {
    iterator first1 = begin();
    iterator last1 = end();
    const_iterator first2 = rhs.begin();
    const_iterator last2 = rhs.end();
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
      *first1 = *first2;
    }
    if (first1 == last1) {
      insert(last1, first2, last2);
    } else {
      erase(first1, last1);
    }
  }
  return *this;
}

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(std::initializer_list<T> rhs) {
  iterator first1 = begin();
  iterator last1 = end();
  auto first2 = rhs.begin();
  auto last2 = rhs.end();
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    *first1 = *first2;
  }
  if (first1 == last1) {
    insert(last1, first2, last2);
  } else {
    erase(first1, last1);
  }
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::operator[](
    const size_type& n) {
  return *(begin() + n);
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos,
                                                         const T& value) {
  link_type tmp = create_node(value);
  pos.node->prev->next = tmp;
  tmp->prev = pos.node->prev;
  pos.node->prev = tmp;
  tmp->next = pos.node;
}

template <typename T, typename Alloc>
template <typename InputIterator>
void list<T, Alloc>::insert(iterator pos, InputIterator first,
                            InputIterator last) {
  for (; first != last; ++first) insert(pos, *first);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, size_type n, const T& value) {
  for (; n > 0; --n) insert(pos, value);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, int n, const T& value) {
  insert(pos, size_type(n), value);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, long n, const T& value) {
  insert(pos, size_type(n), value);
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator pos) {
  iterator tmp = pos;
  ++tmp;
  pos.node->prev->next = tmp.node;
  tmp.node->prev = pos.node->prev;
  destroy_node(pos.node);
  return tmp;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first,
                                                        iterator last) {
  while (first != last) erase(first++);
  return last;
}

template <typename T, typename Alloc>
void list<T, Alloc>::resize(size_type new_size, const T& value) {
  size_type old_size = size();
  if (old_size < new_size) {
    insert(end(), new_size - old_size, value);
  } else {
    iterator new_end = end();
    for (; old_size != new_size; --old_size, --new_end)
      ;
    erase(new_end, end());
  }
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x) {
  if (x.empty()) return;
  transfer(pos, x.begin(), x.end());
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator pos, list&, iterator i) {
  iterator j = i;
  ++j;
  if (pos == i || pos == j) return;
  transfer(pos, i, j);
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator pos, list&, iterator first,
                            iterator last) {
  if (first == last) return;
  transfer(pos, first, last);
}

template <typename T, typename Alloc>
void list<T, Alloc>::remove(const T& value) {
  iterator first = begin();
  iterator last = end();
  for (; first != last; ++first)
    if (*first == value) erase(first);
}

template <typename T, typename Alloc>
void list<T, Alloc>::unique() {
  iterator first = begin();
  iterator last = end();
  if (first == last) return;
  iterator next = first;
  while (++next != last) {
    if (*first == *next)
      erase(next);
    else
      first = next;
  }
}

template <typename T, typename Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) {
  if (x.empty()) return;
  iterator first1 = begin();
  iterator last1 = end();
  iterator first2 = x.begin();
  iterator last2 = x.end();
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1) {
      splice(first1, x, first2);
      first2 = x.begin();
    } else
      ++first1;
  }
  if (first1 == last1) splice(last1, x, first2, last2);
}

template <typename T, typename Alloc>
void list<T, Alloc>::sort() {
  if (node_->next == node_ || node_->next->next == node_) return;
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while (i < fill && !counter[i].empty()) {
      counter[i].merge(carry);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i == fill) ++fill;
  }
  for (int i = 1; i < fill; ++i) counter[i].merge(counter[i - 1]);
  swap(counter[fill - 1]);
}

template <typename T, class Alloc>
inline bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  auto end1 = lhs.node;
  auto end2 = rhs.node;
  auto first1 = end1->next;
  auto first2 = end2->next;
  for (; first1 != end1 && first2 != end2;
       first1 = first1->next, first2 = first2->next)
    if (first1->data != first2->data) return false;
  return first1 == end1 && first2 == end2;
}

template <typename T, typename Alloc>
inline bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  auto end1 = lhs.node;
  auto end2 = rhs.node;
  auto first1 = end1->next;
  auto first2 = end2->next;
  for (; first1 != end1 && first2 != end2;
       first1 = first1->next, first2 = first2->next) {
    if (first1->data < first2->date)
      return true;
    else if (first2->data < first1->data)
      return false;
  }
  return first1 != end1 && first2 == end2;
}
