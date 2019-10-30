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

#define POS_INFINITY 0x7fffffff
#define NEG_INFINITY 0x80000000
#define FLOATSIGNMASK 0x80000000
#define FLOATEXPMASK 0x7f800000
#define FLOATMANTMASK 0x007fffff
#define FLOATSIGNSHIFT 31
#define FLOATEXPSHIFT 23
#define FLOATIMPLICITONE (1 << FLOATEXPSHIFT)
#define RADIX 2
#define EXPBIAS 127
#define EXPSHIFT 8

int FloatToFixed(unsigned int base, float fValue) {
  const unsigned uiFloat = *reinterpret_cast<unsigned *>(&fValue);
  const unsigned sign = (uiFloat & FLOATSIGNMASK) >> FLOATSIGNSHIFT;
  unsigned exponent = (uiFloat & FLOATEXPMASK) >> FLOATEXPSHIFT;
  unsigned mantissa = (uiFloat & FLOATMANTMASK);

  if (exponent < 255) {
    if (0 == exponent) {
      if (mantissa) {
        // Handle a de-normalized number.
        do {
          mantissa *= RADIX;
          --exponent;
        } while (0 == (mantissa & ~FLOATMANTMASK));
      } else {
        return 0;
      }
    }

    // This is the normal case
    int result = mantissa | FLOATIMPLICITONE;
    const int shift = (EXPBIAS - exponent) + (FLOATEXPSHIFT - base);

    if (shift >= 0) {
      if (shift <= 23) {
        result >>= shift;
      } else {
        return 0;
      }
    } else {
      if (shift >= -7) {
        result <<= -shift;
      } else {
        if (sign) {
          return NEG_INFINITY;
        } else {
          return POS_INFINITY;
        }
      }
    }

    if (sign) {
      result = -result;
    }

    return result;
  } else // ( 255 == exponent )
  {
    if (mantissa) {
      // Handle +/- NaN
      return 0;
    } else {
      // Handle +/- Infinity
      if (sign) {
        return NEG_INFINITY;
      } else {
        return POS_INFINITY;
      }
    }
  }
}

float FixedToFloat(unsigned int base, int value) {
  if (0 == value) {
    return 0.0f;
  }

  unsigned sign = 0;
  unsigned mantissa = 0;

  if (value < 0) {
    sign = 0x80000000;
    value = -value;
  }

  unsigned exponent = Clz(value);

  if (exponent < EXPSHIFT) {
    mantissa = value >> (EXPSHIFT - exponent);
  } else {
    mantissa = value << (exponent - EXPSHIFT);
  }

  mantissa &= FLOATMANTMASK;
  exponent = 31 - exponent;
  exponent = (EXPBIAS - base) + exponent;

  // Build the float value.
  const unsigned Value = sign | (exponent << FLOATEXPSHIFT) | mantissa;

  return *reinterpret_cast<const float *>(&Value);
}
