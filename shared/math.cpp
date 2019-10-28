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
#include "stdafx.h"

namespace Math {

static const uint16_t __fp_sin_table[] = {
#include "fpsin.inl"
};

// 1 / 4x - domain 0.5 .. 1.0-1/16
static const uint16_t __rcp_tab[] = {0x8000, 0x71c7, 0x6666, 0x5d17,
                                 0x5555, 0x4ec4, 0x4924, 0x4444};

// 1 / ( 2 * sqrt( x ) ) - extra divide by 2 to scale to 16 bits
static const uint16_t __rsq_tab[] = {
    // domain 0.5 .. 1.0-1/16
    0xb504, 0xaaaa, 0xa1e8, 0x9a5f, 0x93cd, 0x8e00, 0x88d6, 0x8432,
};

// -ln(x)/ln(2) with x = N/16, N=[8...16]
static const uint16_t __log_tab[] = {0xffff, 0xd47f, 0xad96, 0x8a62, 0x6a3f,
                                 0x4caf, 0x3151, 0x17d6, 0x0000};

// domain [0 - 1.0]
static uint16_t __alog_tab[] = {0xffff, 0xeac0, 0xd744, 0xc567, 0xb504,
                            0xa5fe, 0x9837, 0x8b95, 0x8000};

// table[i] = round(2^31*exp2(a)) where a=(i+0.5)/32
static uint32_t __exp2_tab[] = {
    0x8164d1f4, 0x843a28c4, 0x871f6197, 0x8a14d575, 0x8d1adf5b, 0x9031dc43,
    0x935a2b2f, 0x96942d37, 0x99e04593, 0x9d3ed9a7, 0xa0b05110, 0xa43515ae,
    0xa7cd93b5, 0xab7a39b6, 0xaf3b78ad, 0xb311c413, 0xb6fd91e3, 0xbaff5ab2,
    0xbf1799b6, 0xc346ccda, 0xc78d74c9, 0xcbec14ff, 0xd06333db, 0xd4f35aac,
    0xd99d15c2, 0xde60f482, 0xe33f8973, 0xe8396a50, 0xed4f301f, 0xf281773c,
    0xf7d0df73, 0xfd3e0c0d,
};

#define __fpmul(a, b) \
  static_cast<int>((static_cast<int64_t>(a) * static_cast<int64_t>(b)) >> 16)

#define smull(a, b) ((static_cast<int64_t>(a) * static_cast<int64_t>(b)) >> 32)

#define umull(a, b) ((static_cast<uint64_t>(a) * static_cast<uint64_t>(b)) >> 32)

// By Ken Turkowski's Fixed Point Square Root from "Graphics Gems V".
unsigned int iSqrt(unsigned int rhs) {
  unsigned g = 0;
  unsigned s = (31 - Clz(rhs)) >> 1;
  unsigned b = 1 << s;
  do {
    const unsigned tmp = (b + (g << 1)) << s;
    if (rhs >= tmp) {
      g += b;
      rhs -= tmp;
    }

    b >>= 1;
  } while (s--);

  return g;
}

// By Ken Turkowski's Inverse Square Root from "Graphics Gems V".
template <>
fixed16 TInvSqrt(fixed16 rhs) {
  const int value = rhs.GetRaw();

  ASSERT(value > 0);

  // Check for values that fall within supported range
  if (value & 0x7ffffffc) {
    int exp = Clz(value);

    int x = __rsq_tab[(value >> (28 - exp)) & 0x7] << 1;

    exp -= 16;
    if (exp <= 0) {
      x >>= (-exp) >> 1;
    } else {
      x <<= (exp >> 1) + (exp & 1);
    }

    if (exp & 1) {
      x = __fpmul(x, __rsq_tab[0]);
    }

    // Two iterations of newton-raphson x = ( x / 2 ) * ( 3 - a * x * x )
    const int64_t a = static_cast<int64_t>(value);
    x = static_cast<int>(
        ((x >> 1) * ((3 << 16) - ((((a * x) >> 16) * x) >> 16))) >> 16);
    x = static_cast<int>(
        ((x >> 1) * ((3 << 16) - ((((a * x) >> 16) * x) >> 16))) >> 16);

    return fixed16::Make(x);
  } else {
    return fixed16::Make(0x7fffffff);
  }
}

//--
template <>
fixed16 TPow(fixed16 lhs, fixed16 rhs) {
  // prerequisite: 0 <= x <= 1, and y >=0
  // pow(x,y) = 2^(y*log2(x))
  // =  2^(y*log2(x*(2^exp)*(2^-exp))))
  // =  2^(y*(log2(X)-exp))
  // =  2^(log2(X)*y - y*exp)
  // =  2^( - (-log2(X)*y + y*exp) )

  int x = lhs.GetRaw();
  int y = rhs.GetRaw();

  if ((x < fixed16::ONE) && (y > 0)) {
    if (x) {
      int exp = Clz(x) - 16;
      int f = (x << exp);
      x = (f & 0x0fff) << 4;
      f = (f >> 12) & 0x7;

      int p = __fpmul(x, __log_tab[f + 1] - __log_tab[f]) + __log_tab[f];
      p = __fpmul(y, p) + (y * exp);

      exp = (p >> 16);
      if (exp < 31) {
        p = (p & 0xffff);
        x = (p & 0x1fff) << 3;
        p >>= 13;
        p = __fpmul(x, __alog_tab[p + 1] - __alog_tab[p]) + __alog_tab[p];
        p >>= exp;

        return fixed16::Make(p);
      }
    }

    return TConst<fixed16>::Zero();
  }

  return TConst<fixed16>::One();
}

//--
template <>
fixed16 TPow2(fixed16 rhs) {
  ASSERT((rhs.To<int>() >= 0) && (rhs.To<int>() < 32));

  const unsigned x5log2 = 0xb17217f8; // round(2^32*log(2))
  const int x5d3 = 0x55555555;        // round(2^32/3)

  int n, d, q, r;

  n = (rhs.GetRaw() << 10); // Convert to N.26
  d = n << 6;
  n = n >> 26;

  if (n >= 0) {
    q = d >> 27;
    d = d - (q << 27);
    d = umull(d, x5log2);

    int d2 = d - (x5log2 >> 6);
    int r2 = smull(d2, x5d3);
    r2 = smull(d2, r2);
    r2 = r2 + d2;
    r2 = smull(d2, r2);
    r2 = d2 + (r2 >> 1);

    r = r2;
    q = __exp2_tab[q];
    r = umull(q, r);

    if (r2 >= 0) {
      r = r + q;
    }

    n = 31 - n;

    if (n > 16) {
      n = r >> (n - 16);
    } else {
      n = r << (16 - n);
    }

    return fixed16::Make(n);
  }

  return TConst<fixed16>::Zero();
}

//--
template <>
fixed16 TSin(fixed16 rhs) {
  const int TWOPI = 411775;
  const int INVTWOPI = 10430;

  int value = rhs.GetRaw();
  while (value < 0) {
    value += TWOPI;
  }

  value *= INVTWOPI;
  value >>= 16;
  value >>= 4;

  const int result = (value & 0x400) ? __fp_sin_table[0x3ff - (value & 0x3ff)]
                                     : __fp_sin_table[value & 0x3ff];

  return fixed16::Make((value & 0x800) ? -result : result);
}

// By Ken Turkowski's Fixed Point Square Root from "Graphics Gems V".
template <>
fixed16 TCos(fixed16 rhs) {
  const int TWOPI = 411775;
  const int INVTWOPI = 10430;

  int value = rhs.GetRaw();
  while (value < 0) {
    value += TWOPI;
  }

  value *= INVTWOPI;
  value >>= 16;
  value += 0x4000;
  value >>= 4;

  const int result = (value & 0x400) ? __fp_sin_table[0x3ff - (value & 0x3ff)]
                                     : __fp_sin_table[value & 0x3ff];

  return fixed16::Make((value & 0x800) ? -result : result);
}

//--
int Inverse32(int value) {
  // Calculate the sign
  int sign = 1;
  if (value < 0) {
    sign = -1;
    value = -value;
  }

  // Check for values that fall within supported range
  if (value & 0x7ffffffc) {
    int exponent = Clz(value);

    // Look up for a precalculated inverse in 1/16th value range
    int x = __rcp_tab[(value >> (28 - exponent)) & 0x7] << 2;

    exponent -= 16;

    if (exponent <= 0) {
      x >>= -exponent;
    } else {
      x <<= exponent;
    }

    // two iterations of newton-raphson  x = x * ( 2 - a * x )
    const int64_t a = static_cast<int64_t>(value);
    x = static_cast<int>((x * ((2 << 16) - ((a * x) >> 16))) >> 16);
    x = static_cast<int>((x * ((2 << 16) - ((a * x) >> 16))) >> 16);

    // Apply the sign back
    return sign * x;
  }

  return sign * 0x7fffffff;
}

} // namespace Math