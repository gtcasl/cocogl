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

template <unsigned int F, typename T = int> class TFixed {
public:
  typedef T Type;

  enum {
    FRAC = F,
    INT = 32 - FRAC,
    HFRAC = FRAC >> 1,
    ONE = 1 << FRAC,
    MASK = ONE - 1,
    IMASK = ~MASK,
    HALF = ONE >> 1,
    TWO = ONE << 1,
  };

  TFixed<F, T>();
  explicit TFixed<F, T>(float rhs);
  explicit TFixed<F, T>(int rhs);
  explicit TFixed<F, T>(unsigned int rhs);
  explicit TFixed<F, T>(short rhs);
  explicit TFixed<F, T>(unsigned short rhs);
  explicit TFixed<F, T>(char rhs);
  explicit TFixed<F, T>(unsigned char rhs);

  template <unsigned int F2, typename T2>
  explicit TFixed<F, T>(TFixed<F2, T2> rhs);

  template <typename C> C To() const;

  bool operator==(TFixed<F, T> rhs) const;
  bool operator!=(TFixed<F, T> rhs) const;
  bool operator<(TFixed<F, T> rhs) const;
  bool operator<=(TFixed<F, T> rhs) const;
  bool operator>(TFixed<F, T> rhs) const;
  bool operator>=(TFixed<F, T> rhs) const;

  TFixed<F, T> operator-() const;

  TFixed<F, T> operator+=(TFixed<F, T> rhs);
  TFixed<F, T> operator-=(TFixed<F, T> rhs);
  TFixed<F, T> operator*=(TFixed<F, T> rhs);
  TFixed<F, T> operator/=(TFixed<F, T> rhs);

  template <unsigned int F2, typename T2>
  TFixed<F, T> operator*=(TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  TFixed<F, T> operator/=(TFixed<F2, T2> rhs);

  TFixed<F, T> operator*=(int rhs);
  TFixed<F, T> operator*=(unsigned int rhs);
  TFixed<F, T> operator*=(float rhs);

  TFixed<F, T> operator/=(int rhs);
  TFixed<F, T> operator/=(unsigned int rhs);
  TFixed<F, T> operator/=(float rhs);

  static TFixed<F, T> Make(int value);
  static TFixed<F, T> Make(unsigned int value);
  static TFixed<F, T> Make(float value);
  static TFixed<F, T> Make(int64_t value);

  T GetRaw() const;

private:
  T m_value;

  void SetRaw(T value);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator+(TFixed<F2, T2> lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator-(TFixed<F2, T2> lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, unsigned int F3, typename T2, typename T3>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, TFixed<F3, T3> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, unsigned int F3, typename T2, typename T3>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, TFixed<F3, T3> rhs);

  template <unsigned int F2, typename T2>
  friend float operator*(TFixed<F2, T2> lhs, float rhs);

  template <unsigned int F2, typename T2>
  friend float operator*(float lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend float operator/(TFixed<F2, T2> lhs, float rhs);

  template <unsigned int F2, typename T2>
  friend float operator/(float lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, char rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(char lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, char rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(char lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, unsigned char rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(unsigned char lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, unsigned char rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(unsigned char lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, short rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(short lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, short rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(short lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, unsigned short rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(unsigned short lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, unsigned short rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(unsigned short lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator<<(TFixed<F2, T2> lhs, int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(int lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator>>(TFixed<F2, T2> lhs, int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(int lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(TFixed<F2, T2> lhs, unsigned int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator<<(TFixed<F2, T2> lhs, unsigned int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator*(unsigned int lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(TFixed<F2, T2> lhs, unsigned int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator>>(TFixed<F2, T2> lhs, unsigned int rhs);

  template <unsigned int F2, typename T2>
  friend TFixed<F2, T2> operator/(unsigned int lhs, TFixed<F2, T2> rhs);

  template <unsigned int F2, typename T2> friend class TFixed;

  template <unsigned int F2, typename T2> friend class TFixedTypeCaster;
};

int FloatToFixed(unsigned int base, float fValue);
float FixedToFloat(unsigned int base, int value);