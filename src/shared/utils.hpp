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

#include <type_traits>

void DbgPrintf(int level, const char *format, ...);

///////////////////////////////////////////////////////////////////////////////

namespace detail {
struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const &) = delete;
  void operator=(nonesuch const &) = delete;
};

template <typename Default, typename Void, template <class...> class Op,
          class... Args>
struct detector {
  using value_t = std::false_type;
  using type = Default;
};

template <typename Default, template <class...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
  using value_t = std::true_type;
  using type = Op<Args...>;
};
} // namespace detail

template <template <class...> class Op, class... Args>
using detected_t = typename detail::detector<detail::nonesuch, void, Op, Args...>::type;

template <typename Default, template <class...> class Op, class... Args>
using detected_or_t = typename detail::detector<Default, void, Op, Args...>::type;

template <template <class...> class Op, class... Args>
constexpr bool is_detected_v =
    detail::detector<detail::nonesuch, void, Op, Args...>::value_t::value;

///////////////////////////////////////////////////////////////////////////////

class scope_exit {
public:
  scope_exit(const std::function<void()> &func)
      : func_(func) {}
  ~scope_exit() {
    func_();
  }
  // force stack only allocation!
  static void *operator new(size_t) = delete;
  static void *operator new[](size_t) = delete;

protected:
  std::function<void()> func_;
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline int __countof(const T &a) {
  return (sizeof(a) / sizeof(a[0]));
}

template <typename T>
inline void __safeAcquire(T *p) {
  if (p) {
    p->addRef();
  }
}

template <typename T>
inline void __safeRelease(T *&p) {
  if (p) {
    p->release();
    p = nullptr;
  }
}

template <typename T>
inline void __safeDelete(T *&p) {
  if (p) {
    delete p;
    p = nullptr;
  }
}

template <typename T>
inline void __safeDeleteArray(T *&p) {
  if (p) {
    delete[] p;
    p = nullptr;
  }
}

template <typename T>
inline void __safeFree(T *&p) {
  if (p) {
    free(p);
    p = nullptr;
  }
}

template <typename T>
inline bool __isAligned32(T *ptr) {
  return (0 == (reinterpret_cast<uintptr_t>(ptr) & 3));
}

inline size_t __align(size_t offset, size_t alignment) {
  return (offset + alignment - 1) & ~(alignment - 1);
}

template <typename T>
inline T *__alignPtr(T *ptr, size_t alignment) {
  auto offset = reinterpret_cast<uintptr_t>(ptr);
  return reinterpret_cast<T *>((offset + alignment - 1) & ~(alignment - 1));
}

template <typename T>
inline void __swap(T &a, T &b) {
  T c(a);
  a = b;
  b = c;
}

template <uint64_t N>
struct __countbits {
  static const uint32_t nbits = __countbits<(N >> 1)>::nbits + 1;
};

template <>
struct __countbits<0> { static const uint32_t nbits = 0; };

inline int32_t Clz(int32_t rhs) {
  return __builtin_clz(rhs);
}

inline int32_t Ctz(int32_t rhs) {
  return 31 - Clz(rhs & -rhs);
}

inline int iLog2(int value) {
  return 31 - Clz(value);
}

inline bool IsPowerOf2(int value) {
  return 0 == (value & (value - 1));
}

#ifndef NDEBUG
#define __debugMsg(level, ...) DbgPrintf(level, __VA_ARGS__);
#else
#define __debugMsg(...)
#endif

#ifdef NDEBUG
#define __no_default assert(0)
#else
#define __no_default assert(false);
#endif

template <typename... Args>
void __unused(Args &&...) {}