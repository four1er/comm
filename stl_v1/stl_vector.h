#pragma once

#include <algorithm>
#include <memory>

#include "iterator.h"
#include "stl_alloc.h"
#include "stl_config.h"
#include "type_traits"

template <class _Tp, class _Allocator, bool _IsStatic>
class _Vector_alloc_base {
 public:
  typedef
      typename _Alloc_traits<_Tp, _Allocator>::allocator_type allocator_type;
  allocator_type get_allocator() const { return _M_data_allocator; }

  _Vector_alloc_base(const allocator_type &__a)
      : _M_data_allocator(__a),
        _M_start(0),
        _M_finish(0),
        _M_end_of_storage(0) {}

 private:
  allocator_type _M_data_allocator;
  _Tp *_M_start;
  _Tp *_M_finish;
  _Tp *_M_end_of_storage;

  _Tp *_M_allocate(size_t __n) { return _M_data_allocator.allocate(__n); }

  void _M_deallocate(_Tp *__p, size_t __n) {
    _M_data_allocator.deallocate(__p, __n);
  }
};

// Specialization
template <class _Tp, class _Allocator>
class _Vector_alloc_base<_Tp, _Allocator, true> {
 public:
  typedef
      typename _Alloc_traits<_Tp, _Allocator>::allocator_type allocator_type;

  allocator_type get_allocator() const { return allocator_type(); }

  _Vector_alloc_base(const allocator_type &)
      : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}

 protected:
  _Tp *_M_start;
  _Tp *_M_finish;
  _Tp *_M_end_of_storage;

  typedef typename _Alloc_traits<_Tp, _Allocator>::_Alloc_type _Alloc_type;
  _Tp *_M_allocate(size_t __n) { return _Alloc_type::allocate(__n); }
  void _M_deallocate(_Tp *__p, size_t __n) {
    _Alloc_type::deallocate(__p, __n);
  }
};

template <class _Tp, class _Alloc>
class _Vector_base {
 public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }
  _Vector_base(const _Alloc &)
      : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}

  _Vector_base(size_t __n, const _Alloc &)
      : _M_start(0), _M_finish(0), _M_end_of_storage(0) {
    _M_start = _M_allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
  }

  ~_Vector_base() {
    if (_M_start) {
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    }
  }

 protected:
  _Tp *_M_start;
  _Tp *_M_finish;
  _Tp *_M_end_of_storage;

  typedef simple_alloc<_Tp, _Alloc> _M_data_allocator;
  _Tp *_M_allocate(size_t __n) { return _M_data_allocator::allocate(__n); }

  void _M_deallocate(_Tp *__p, size_t __n) {
    _M_data_allocator::deallocate(__p, __n);
  }
};

template <typename T, typename Alloc = allocator<T>>
class vector {
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = T *;
  using const_iterator = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using allocator_type = Alloc;

  using reverse_iter = reverse_iterator<iterator, T>;
  using const_reverse_iter =
      reverse_iterator<const_iterator, T, const_reference, difference_type>;

 protected:
  iterator start_;
  iterator finish_;
  iterator end_of_storage_;
  allocator_type allocator_;

  void insert_aux(iterator position, const T &x);
  void fill_init(size_type n, const T &value);

  template <typename InputIterator>
  void copy_init(InputIterator first, InputIterator last);

 public:
  vector() : start_(0), finish_(0), end_of_storage_(0) {}
  vector(size_type n, const T &value);
  vector(int n, const T &value);
  vector(long n, const T &value);
  explicit vector(size_type n);
  vector(const vector<T> &vec);
  template <typename InputIterator>
  vector(InputIterator first, InputIterator last);
  vector(std::initializer_list<T> rhs);
  vector<T, Alloc> &operator=(const vector<T, Alloc> &vec);
  vector<T, Alloc> &operator=(std::initializer_list<T> rhs);
  ~vector();

 public:
  iterator begin() noexcept { return start_; }
  const_iterator begin() const noexcept { return start_; }
  iterator end() noexcept { return finish_; }
  const_iterator end() const noexcept { return finish_; }

  reverse_iter rbegin() noexcept { return reverse_iter(finish_); }
  const_reverse_iter rbegin() const noexcept {
    return const_reverse_iter(finish_);
  }
  reverse_iter rend() noexcept { return reverse_iter(begin); }
  const_reverse_iter rend() const noexcept {
    return const_reverse_iter(finish_);
  }

  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend() const noexcept { return end(); }
  const_iterator crbegin() const noexcept { return rbegin(); }
  const_iterator crend() const noexcept { return rend(); }

  size_type size() const noexcept { return size_type(end() - begin()); }
  size_type max_size() const noexcept { return size_type(-1) / sizeof(T); }
  size_type capacity() const noexcept {
    return size_type(end_of_storage_ - begin());
  }
  bool empty() const noexcept { return begin() == end(); }
  void reserve(size_type n);

  reference front() { return *start_; }
  const_reference front() const { return *start_; }
  reference back() { return *(finish_ - 1); }
  const_reference back() const { return *(finish_ - 1); }
  reference operator[](size_type n) { return *(begin() + n); }
  const_reference operator[](size_type n) const { return *(begin() + n); }

  void push_back(const T &x);
  void pop_back();
  void swap(vector<T, Alloc> &rhs);
  iterator insert(iterator position, const T &x);
  iterator insert(iterator position);
  void insert(iterator position, size_type n, const T &value);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void resize(size_type new_size);
  void resize(size_type new_size, const T &x);
  void clear();
};

template <typename T, typename Alloc>
void vector<T, Alloc>::fill_init(size_type n, const T &value) {
  start_ = allocator_.allocate(n);
  std::uninitialized_fill_n(start_, n, value);
  finish_ = start_ + n;
  end_of_storage_ = finish_;
}

template <typename T, typename Alloc>
template <typename InputIterator>
void vector<T, Alloc>::copy_init(InputIterator first, InputIterator last) {
  size_type n = last - first;
  start_ = allocator_.allocate(n);
  std::uninitialized_copy(first, last, start_);
  end_of_storage_ = finish_ = start_ + n;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T &x) {
  if (finish_ != end_of_storage_) {
    allocator_.construct(finish_, *(finish_ - 1));
    ++finish_;
    std::copy_backward(position, finish_ - 1, finish_);
    *position = x;
  } else {
    const size_type old_size = size();
    const size_type new_size = old_size != 0 ? 2 * old_size : 1;
    iterator new_start = allocator_.allocate(new_size);
    iterator new_finish = new_start;
    new_finish = std::uninitialized_copy(start_, finish_, new_start);
    allocator_.construct(new_finish, x);
    new_finish = std::uninitialized_copy(position, finish_, new_finish);
    allocator_.destroy(start_, finish_);
    allocator_.deallocate(start_, end_of_storage_ - start_);
    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = new_start + new_size;
  }
}

template <typename T, typename Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &vec) {
  if (this != &vec) {
    size_type new_size = vec.size();
    if (new_size > capacity()) {
      iterator new_start = allocator_.allocate(new_size);
      end_of_storage_ =
          std::uninitialized_copy(vec.begin(), vec.end(), new_start);
      allocator_.destroy(start_, finish_);
      allocator_.deallocate(start_, end_of_storage_ - start_);
      start_ = new_start;
    } else if (new_size < size()) {
      iterator it = std::uninitialized_copy(vec.begin(), vec.end(), start_);
      allocator_.destroy(it, finish_);
    } else {
      std::copy(vec.begin(), vec.end() + size(), start_);
      std::uninitialized_copy(vec.begin() + size(), vec.end(), finish_);
    }
    finish_ = start_ + new_size;
  }
  return *this;
}

template <typename T, typename Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(std::initializer_list<T> rhs) {
  vector<T, Alloc> tmp(rhs.begin(), rhs.end());
  swap(tmp);
  return *this;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::push_back(const T &value) {
  if (finish_ != end_of_storage_)
    allocator_.construct(finish_++, value);
  else
    insert_aux(finish_, value);
}

template <typename T, typename Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc> &rhs) {
  std::swap(start_, rhs.start_);
  std::swap(finish_, rhs.finish_);
  std::swap(end_of_storage_, rhs.end_of_storage_);
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos,
                                                             const T &value) {
  size_type n = pos - start_;
  if (finish_ != end_of_storage_ && pos == finish_) {
    allocator_.construct(finish_++, value);
  } else {
    insert_aux(pos, value);
  }
  return start_ + n;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T &value) {
  if (n == 0) return;
  if (size() + n < capacity()) {
    const size_type elems_after = finish_ - pos;
    if (elems_after > n) {
      std::uninitialized_copy(finish_ - n, finish_, finish_);
      std::copy_backward(pos, finish_ - n, finish_);
      std::fill(pos, pos + n, value);
    } else {
      std::uninitialized_fill_n(finish_, n - elems_after, value);
      std::uninitialized_copy(pos, finish_, finish_ + n - elems_after);
      std::fill(pos, finish_, value);
    }
  } else {
    const size_type old_size = size();
    const size_type new_size = old_size > n ? old_size : n;
    iterator new_start = allocator_.allocate(new_size);
    iterator new_finish = new_start;
    try {
      new_finish = std::uninitialized_copy(start_, pos, new_start);
      new_finish = std::uninitialized_fill_n(new_finish, n, value);
      new_finish = std::uninitialized_copy(pos, finish_, new_finish);
    } catch (...) {
      allocator_.destroy(new_start, new_finish);
      allocator_.deallocate(new_start, new_size);
      throw;
    }
    allocator_.destroy(start_, finish_);
    allocator_.deallocate(start_, end_of_storage_ - start_);
    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = new_start + new_size;
  }
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
  if (pos + 1 != finish_) {
    std::copy(pos + 1, finish_, pos);
  }
  allocator_.destroy(--finish_);
  return pos;
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first,
                                                            iterator last) {
  if (first != last) {
    std::copy(last, finish_, first);
    allocator_.destroy(finish_ - (last - first), finish_);
    finish_ -= last - first;
  }
  return first;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type new_size, const T &value) {
  if (new_size < size()) {
    erase(start_ + new_size, finish_);
  } else {
    insert(finish_, new_size - size(), value);
  }
}

template <typename T, typename Alloc>
inline bool operator==(const vector<T, Alloc> &lhs,
                       const vector<T, Alloc> &rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline bool operator<(const vector<T, Alloc> &lhs,
                      const vector<T, Alloc> &rhs) {
  typename vector<T, Alloc>::iterator first1 = lhs.begin();
  auto last1 = lhs.end();
  auto first2 = rhs.begin();
  auto last2 = rhs.end();
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2) return true;
    if (*first2 < *first1) return false;
  }
  return first1 == last1 && first2 != last2;
}

template <typename T, typename Alloc>
inline void swap(vector<T, Alloc> &lhs, vector<T, Alloc> &rhs) {
  lhs.swap(rhs);
}
