#pragma once

#include "stl_config.h"

struct __true_type {};
struct __false_type {};

template <class _Tp>
struct __type_traits {
    typedef __true_type this_dummy_member_must_be_first;
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};

template <> 
struct __type_traits<bool> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<char> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<signed char> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<unsigned char> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<wchar_t> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<short> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<unsigned short> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<int> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<unsigned int> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<long> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<unsigned long> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<long long> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<unsigned long long> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};


template <> 
struct __type_traits<float> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<double> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <> 
struct __type_traits<long double> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};


#ifdef __USE_STDCPP11_TYPE_TRAITS__

template <typename _Tp, _Tp __v>
struct integral_constant {
  static constexpr _Tp value = __v;
  typedef _Tp value_type;
  typedef integral_constant<_Tp, __v> type;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

template <typename _Tp, _Tp __v>
constexpr _Tp integral_constant<_Tp, __v>::value;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

/// remove_const
template <typename _Tp> 
struct remove_const { typedef _Tp type; };

template <typename _Tp> 
struct remove_const<_Tp const> { typedef _Tp type; };

/// remove_volatile
template <typename _Tp> 
struct remove_volatile { typedef _Tp type; };

template <typename _Tp> 
struct remove_volatile<_Tp volatile> {
  typedef _Tp type;
};

// remove_cv
template <typename _Tp>
struct remove_cv {
  using type = _Tp;
};

template <typename _Tp>
struct remove_cv<const _Tp> {
  using type = _Tp;
};

template <typename _Tp>
struct remove_cv<volatile _Tp> {
  using type = _Tp;
};

template <typename _Tp>
struct remove_cv<const volatile _Tp> {
  using type = _Tp;
};

// add const
template <typename _Tp>
struct add_const {
  // typedef _Tp const type;
  using type = const _Tp;
};

// add volatile
template <typename _Tp>
struct add_volatile {
  // typedef _Tp volatile type;
  using type = volatile _Tp;
};

// add cv
template <typename _Tp>
struct add_cv {
  typedef typename add_const<typename add_volatile<_Tp>::type>::type type;
};

template <typename _Tp>
using remove_const_t = typename remove_const<_Tp>::type;

template <typename _Tp>
using remove_volatile_t = typename remove_volatile<_Tp>::type;

template <typename _Tp>
using add_const_t = typename add_const<_Tp>::type;

template <typename _Tp>
using add_volatile_t = typename add_volatile<_Tp>::type;

template <typename _Tp>
using add_cv_t = typename add_cv<_Tp>::type;

template <typename _Tp>
using __remove_cv_t = typename remove_cv<_Tp>::type;

// is_void

template <typename>
struct __is_void_helper : public false_type {};

template <>
struct __is_void_helper<void> : public true_type {};

template <typename _Tp>
struct is_void : public __is_void_helper<__remove_cv_t<_Tp>>::type {

};

#endif
