//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools
// installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR
// INDEMNITIES.
//
#pragma once

void DbgPrintf(int level, LPCTSTR format, ...);

///////////////////////////////////////////////////////////////////////////////

namespace detail {
struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const&) = delete;
  void operator=(nonesuch const&) = delete;
};

template <class Default, class Void, template<class...> class Op, class... Args>
struct detector {
  using value_t = std::false_type;
  using type = Default;
};

template <class Default, template<class...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
  using value_t = std::true_type;
  using type = Op<Args...>;
};
}

template <template<class...> class Op, class... Args>
using detected_t = typename detail::detector<detail::nonesuch, void, Op, Args...>::type;

template <class Default, template<class...> class Op, class... Args>
using detected_or_t = typename detail::detector<Default, void, Op, Args...>::type;

template<template<class...> class Op, class... Args>
constexpr bool is_detected_v = detail::detector<detail::nonesuch, void, Op, Args...>::value_t::value;

///////////////////////////////////////////////////////////////////////////////

template <class T> inline size_t __countof(const T &a) {
  return (sizeof(a) / sizeof(a[0]));
}

template <class T> inline void __safeAcquire(T *p) {
  if (p) {
    p->AddRef();
  }
}

template <class T> inline void __safeRelease(T *&p) {
  if (p) {
    p->Release();
    p = NULL;
  }
}

template <class T> inline void __safeDelete(T *&p) {
  if (p) {
    delete p;
    p = NULL;
  }
}

template <class T> inline void __safeDeleteArray(T *&p) {
  if (p) {
    delete[] p;
    p = NULL;
  }
}

template <class T> inline void __safeFree(T *&p) {
  if (p) {
    free(p);
    p = NULL;
  }
}

template <class T> inline bool __isAligned32(T *ptr) {
  const size_t offset = ptr - (T *)(NULL);
  return (0 == (offset & 3));
}

inline size_t __align(size_t offset, size_t alignment) {
  return (offset + alignment - 1) & ~(alignment - 1);
}

template <class T> inline T *__alignPtr(T *ptr, size_t alignment) {
  const size_t offset = ptr - (T *)(NULL);
  return (T *)(NULL) + ((offset + alignment - 1) & ~(alignment - 1));
}

template <class T> inline void __swap(T &a, T &b) {
  T c(a);
  a = b;
  b = c;
}

template <unsigned long N> struct __countbits {
  static const unsigned nbits = __countbits<(N >> 1)>::nbits + 1;
};

template <> struct __countbits<0> { 
  static const unsigned nbits = 0; 
};

inline unsigned int Clz(unsigned int rhs) {
  return __builtin_clz(rhs);
}

inline unsigned int Ctz(unsigned int rhs) { 
  return 31 - Clz(rhs & -(int)rhs); 
}