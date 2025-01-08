#pragma once

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

template <class _Tp, class _Alloc>
struct _Vector_base {};
