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

namespace detail {

template <class TD, class TS, int Shift> struct TFixedShifter {
public:
  inline static TD call(TS value) {
    if
      constexpr(sizeof(TD) > sizeof(TS)) {
        if
          constexpr(Shift > 0) { return static_cast<TD>(value) >> Shift; }
        else {
          return static_cast<TD>(value) << -Shift;
        }
      }
    else {
      if
        constexpr(Shift > 0) { return static_cast<TD>(value >> Shift); }
      else {
        return static_cast<TD>(value << -Shift);
      }
    }
  }
};
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T> class TFixedTypeCaster {
public:
  inline static void Exec(int *pOut, TFixed<F, T> rhs) {
    const T &value = rhs.GetRaw();
    *pOut = static_cast<int>(value >> TFixed<F, T>::FRAC);
  }

  inline static void Exec(float *pOut, TFixed<F, T> rhs) {
    const T &value = rhs.GetRaw();
    *pOut = FixedToFloat(F, value);
  }

  template <uint32_t F2, typename T2>
  inline static void Exec(TFixed<F2, T2> *pOut, TFixed<F, T> rhs) {
    *pOut = TFixed<F2, T2>(rhs);
  }

  friend class TFixed<F, T>;
};

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T> inline TFixed<F, T>::TFixed() {}

template <uint32_t F, typename T> inline TFixed<F, T>::TFixed(float rhs) {
  m_value = FloatToFixed(F, rhs);
  ASSERT(m_value == static_cast<int>(rhs * ONE));
}

template <uint32_t F, typename T> inline TFixed<F, T>::TFixed(int rhs) {
  m_value = rhs << FRAC;
  ASSERT((int64_t(rhs) << FRAC) == m_value);
}

template <uint32_t F, typename T>
inline TFixed<F, T>::TFixed(uint32_t rhs) {
  m_value = static_cast<T>(rhs) << FRAC;
  ASSERT((int64_t(rhs) << FRAC) == m_value);
}

template <uint32_t F, typename T> inline TFixed<F, T>::TFixed(short rhs) {
  m_value = static_cast<T>(rhs) << FRAC;
  ASSERT((int64_t(rhs) << FRAC) == m_value);
}

template <uint32_t F, typename T>
inline TFixed<F, T>::TFixed(uint16_t rhs) {
  m_value = static_cast<T>(rhs) << FRAC;
  ASSERT((int64_t(rhs) << FRAC) == m_value);
}

template <uint32_t F, typename T> inline TFixed<F, T>::TFixed(char rhs) {
  m_value = static_cast<T>(rhs) << FRAC;
  ASSERT((int64_t(rhs) << FRAC) == m_value);
}

template <uint32_t F, typename T>
inline TFixed<F, T>::TFixed(uint8_t rhs) {
  m_value = static_cast<T>(rhs) << FRAC;
  ASSERT((int64_t(rhs) << FRAC) == m_value);
}

template <uint32_t F, typename T>
template <uint32_t F2, typename T2>
inline TFixed<F, T>::TFixed(TFixed<F2, T2> rhs) {
  m_value = detail::TFixedShifter<T, T2, static_cast<int>(F2) - static_cast<int>(F)>::call(rhs.m_value);
  ASSERT((detail::TFixedShifter<int64_t, int64_t, static_cast<int>(F2) - static_cast<int>(F)>::call(rhs.m_value)) == m_value);
}

template <uint32_t F, typename T>
template <class C>
inline C TFixed<F, T>::To() const {
  C result;
  TFixedTypeCaster<F, T>::Exec(&result, *this);
  return result;
}

template <uint32_t F, typename T>
inline bool TFixed<F, T>::operator==(TFixed<F, T> rhs) const {
  return m_value == rhs.m_value;
}

template <uint32_t F, typename T>
inline bool TFixed<F, T>::operator!=(TFixed<F, T> rhs) const {
  return m_value != rhs.m_value;
}

template <uint32_t F, typename T>
inline bool TFixed<F, T>::operator<(TFixed<F, T> rhs) const {
  return m_value < rhs.m_value;
}

template <uint32_t F, typename T>
inline bool TFixed<F, T>::operator<=(TFixed<F, T> rhs) const {
  return m_value <= rhs.m_value;
}

template <uint32_t F, typename T>
inline bool TFixed<F, T>::operator>(TFixed<F, T> rhs) const {
  return m_value > rhs.m_value;
}

template <uint32_t F, typename T>
inline bool TFixed<F, T>::operator>=(TFixed<F, T> rhs) const {
  return m_value >= rhs.m_value;
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator-() const {
  return TFixed<F, T>::Make(-m_value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator+=(TFixed<F, T> rhs) {
  (*this) = (*this) + rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator-=(TFixed<F, T> rhs) {
  (*this) = (*this) - rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator*=(TFixed<F, T> rhs) {
  (*this) = (*this) * rhs;
  return (*this);
}

template <uint32_t F, typename T>
template <uint32_t F2, typename T2>
inline TFixed<F, T> TFixed<F, T>::operator*=(TFixed<F2, T2> rhs) {
  (*this) = (*this) * rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator*=(int rhs) {
  (*this) = (*this) * rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator*=(uint32_t rhs) {
  (*this) = (*this) * rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator*=(float rhs) {
  (*this) = static_cast<TFixed<F, T>>((*this) * rhs);
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator/=(TFixed<F, T> rhs) {
  (*this) = (*this) / rhs;
  return (*this);
}

template <uint32_t F, typename T>
template <uint32_t F2, typename T2>
inline TFixed<F, T> TFixed<F, T>::operator/=(TFixed<F2, T2> rhs) {
  (*this) = (*this) / rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator/=(int rhs) {
  (*this) = (*this) / rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator/=(uint32_t rhs) {
  (*this) = (*this) / rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::operator/=(float rhs) {
  (*this) = (*this) / rhs;
  return (*this);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator+(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  ASSERT((static_cast<int64_t>(lhs.m_value) + rhs.m_value) ==
         (lhs.m_value + rhs.m_value));
  return TFixed<F, T>::Make(lhs.m_value + rhs.m_value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator-(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  ASSERT((static_cast<int64_t>(lhs.m_value) - rhs.m_value) ==
         (lhs.m_value - rhs.m_value));
  return TFixed<F, T>::Make(lhs.m_value - rhs.m_value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  return TFixed<F, T>::Make((static_cast<int64_t>(lhs.m_value) * rhs.m_value) >>
                            TFixed<F, T>::FRAC);
}

template <uint32_t F, uint32_t F2, typename T, typename T2>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, TFixed<F2, T2> rhs) {
  return TFixed<F, T>::Make(
      (static_cast<int64_t>(lhs.GetRaw()) * rhs.GetRaw()) >>
      TFixed<F2, T2>::FRAC);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  ASSERT(rhs.m_value != 0);
  return TFixed<F, T>::Make(
      (static_cast<int64_t>(lhs.m_value) << TFixed<F, T>::FRAC) / rhs.m_value);
}

template <uint32_t F, uint32_t F2, typename T, typename T2>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, TFixed<F2, T2> rhs) {
  ASSERT(rhs.GetRaw() != 0);
  return TFixed<F, T>::Make(
      (static_cast<int64_t>(lhs.GetRaw()) << TFixed<F2, T2>::FRAC) /
      rhs.GetRaw());
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T>
inline float operator*(TFixed<F, T> lhs, float rhs) {
  return Math::TCast<float>(lhs) * rhs;
}

template <uint32_t F, typename T>
inline float operator*(float lhs, TFixed<F, T> rhs) {
  return lhs * Math::TCast<float>(rhs);
}

template <uint32_t F, typename T>
inline float operator/(TFixed<F, T> lhs, float rhs) {
  return Math::TCast<float>(lhs) / rhs;
}

template <uint32_t F, typename T>
inline float operator/(float lhs, TFixed<F, T> rhs) {
  return lhs / Math::TCast<float>(rhs);
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, char rhs) {
  return lhs * static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(char lhs, TFixed<F, T> rhs) {
  return rhs * lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, char rhs) {
  return lhs / static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(char lhs, TFixed<F, T> rhs) {
  return rhs / lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, uint8_t rhs) {
  return lhs * static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(uint8_t lhs, TFixed<F, T> rhs) {
  return rhs * lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, uint8_t rhs) {
  return lhs / static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(uint8_t lhs, TFixed<F, T> rhs) {
  return rhs / lhs;
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, short rhs) {
  return lhs * static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(short lhs, TFixed<F, T> rhs) {
  return rhs * lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, short rhs) {
  return lhs / static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(short lhs, TFixed<F, T> rhs) {
  return rhs / lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, uint16_t rhs) {
  return lhs * static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(uint16_t lhs, TFixed<F, T> rhs) {
  return rhs * lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, uint16_t rhs) {
  return lhs / static_cast<int>(rhs);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(uint16_t lhs, TFixed<F, T> rhs) {
  return rhs / lhs;
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, int rhs) {
  auto value = static_cast<T>(lhs.m_value * rhs);
  ASSERT((lhs.m_value * static_cast<int64_t>(rhs)) == value);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator<<(TFixed<F, T> lhs, int rhs) {
  auto value = static_cast<T>(lhs.m_value << rhs);
  ASSERT((lhs.m_value << static_cast<int64_t>(rhs)) == value);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(int lhs, TFixed<F, T> rhs) {
  return rhs * lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, int rhs) {
  ASSERT(rhs);
  auto value = static_cast<T>(lhs.m_value / rhs);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator>>(TFixed<F, T> lhs, int rhs) {
  auto value = static_cast<T>(lhs.m_value >> rhs);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(int lhs, TFixed<F, T> rhs) {
  return rhs / lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(TFixed<F, T> lhs, uint32_t rhs) {
  auto value = static_cast<T>(lhs.m_value << rhs);
  ASSERT((lhs.m_value << static_cast<int64_t>(rhs)) == value);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator<<(TFixed<F, T> lhs, uint32_t rhs) {
  auto value = static_cast<T>(lhs.m_value << rhs);
  ASSERT((lhs.m_value << static_cast<int64_t>(rhs)) == value);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator*(uint32_t lhs, TFixed<F, T> rhs) {
  return rhs * lhs;
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(TFixed<F, T> lhs, uint32_t rhs) {
  ASSERT(rhs);
  auto value = static_cast<T>(lhs.m_value / rhs);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator>>(TFixed<F, T> lhs, uint32_t rhs) {
  auto value = static_cast<T>(lhs.m_value >> rhs);
  return TFixed<F, T>::Make(value);
}

template <uint32_t F, typename T>
inline TFixed<F, T> operator/(uint32_t lhs, TFixed<F, T> rhs) {
  return rhs / lhs;
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::Make(int value) {
  TFixed<F, T> fx;
  fx.m_value = static_cast<T>(value);
  ASSERT((fx.m_value ^ value) == 0);
  return fx;
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::Make(uint32_t value) {
  TFixed<F, T> fx;
  fx.m_value = static_cast<T>(value);
  ASSERT((fx.m_value ^ value) == 0);
  return fx;
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::Make(int64_t value) {
  TFixed<F, T> fx;
  fx.m_value = static_cast<T>(value);
  ASSERT(fx.m_value == value);
  return fx;
}

template <uint32_t F, typename T>
inline TFixed<F, T> TFixed<F, T>::Make(float value) {
  TFixed<F, T> fx;
  fx.m_value = static_cast<T>(value);
  ASSERT(fx.m_value == static_cast<int64_t>(value));
  return fx;
}

template <uint32_t F, typename T>
inline void TFixed<F, T>::SetRaw(T value) {
  m_value = value;
}

template <uint32_t F, typename T> inline T TFixed<F, T>::GetRaw() const {
  return m_value;
}