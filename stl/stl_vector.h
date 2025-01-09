#pragma once

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

  using reverse_iter = reverse_iterator<iterator, T>;
  using const_reverse_iter =
      reverse_iterator<const_iterator, T, const_reference, difference_type>;

 protected:
  iterator start_;
  iterator finish_;
  iterator end_of_storage_;

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
  iterator insert(iterator position, size_type n, const T &value);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void resize(size_type new_size);
  void resize(size_type new_size, const T &x);
  void clear();
};
