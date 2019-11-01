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

struct ColorARGB {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      uint32_t value;
    };
    struct {
      uint8_t b, g, r, a;
    };
    struct {
      uint8_t m[4];
    };
  };
  DISABLE_WARNING_POP

  ColorARGB();
  ColorARGB(int a, int r, int g, int b);
  ColorARGB(int r, int g, int b);
  ColorARGB(int value);
};

struct Color4 {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      int b, g, r, a;
    };
    struct {
      int m[4];
    };
  };
  DISABLE_WARNING_POP

  Color4();
  Color4(int a, int r, int g, int b);
  Color4(int r, int g, int b);
  Color4(const ColorARGB color);
  Color4(int a, int rgb);

  ~Color4();

  void operator=(const Color4 &rhs);
};
