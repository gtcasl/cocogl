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

const unsigned int PRECISION = 16;
const GLfixed ONE = 1 << PRECISION;
const GLfixed ZERO = 0;

#define __f2x(f)        ((int)(f * 65536))
#define __mulx(a, b)    ((((__int64)(a)) * (b)) >> 16)
#define __divx(a, b)    (((__int64)(a) << 16) / (b))
#define __PI            3.14159265358979323846f

inline GLfixed FixedFromInt(int value) { return value << PRECISION; }

inline GLfixed FixedFromFloat(float value) {
  return (GLfixed)(value * (float)(ONE));
}

inline float FloatFromFixed(GLfixed value) { return (float)value / (float)ONE; }

inline int IntFromFixed(GLfixed value) { return value >> PRECISION; }

inline GLfixed MultiplyFixed(GLfixed op1, GLfixed op2) {
  return (GLfixed)(((int64_t)op1 * op2) >> PRECISION);
}

inline GLfixed DivideFixed(GLfixed op1, GLfixed op2) {
  return (GLfixed)(((int64_t)op1 << PRECISION) / op2);
}