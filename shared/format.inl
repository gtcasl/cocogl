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

namespace Format {

template <> inline uint32_t TConvertToNative<FORMAT_UNKNOWN>(const Color4 &in) {
  __unreferenced(in);
  return 0;
}

template <>
inline void TConvertFromNative<FORMAT_UNKNOWN, false>(Color4 *pOut,
                                                      uint32_t in) {
  __unreferenced(pOut);
  __unreferenced(in);
}

template <>
inline void TConvertFromNative<FORMAT_UNKNOWN, true>(Color4 *pOut,
                                                     uint32_t in) {
  __unreferenced(pOut);
  __unreferenced(in);
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_R5G6B5>(const Color4 &in) {
  return ((in.r & 0xf8) << 8) | ((in.g & 0xfc) << 3) | (in.b >> 3);
}

template <>
inline void TConvertFromNative<FORMAT_R5G6B5, false>(Color4 *pOut,
                                                     uint32_t in) {
  ASSERT(pOut);
  pOut->r = ((in >> 11) << 3) | (in >> 13);
  pOut->g = ((in >> 3) & 0xfc) | ((in >> 9) & 0x3);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

template <>
inline void TConvertFromNative<FORMAT_R5G6B5, true>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff;
  pOut->r = ((in >> 11) << 3) | (in >> 13);
  pOut->g = ((in >> 3) & 0xfc) | ((in >> 9) & 0x3);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline uint32_t TConvertToNative<FORMAT_A1R5G5B5>(const Color4 &in) {
  return (in.a ? 0x8000 : 0) | ((in.r & 0xf8) << 7) | ((in.g & 0xf8) << 2) |
         (in.b >> 3);
}

template <>
inline void TConvertFromNative<FORMAT_A1R5G5B5, false>(Color4 *pOut,
                                                       uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff * (in >> 15);
  pOut->r = ((in >> 7) & 0xf8) | ((in << 1) >> 13);
  pOut->g = ((in >> 2) & 0xf8) | ((in >> 7) & 7);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

template <>
inline void TConvertFromNative<FORMAT_A1R5G5B5, true>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff * (in >> 15);
  pOut->r = ((in >> 7) & 0xf8) | ((in << 1) >> 13);
  pOut->g = ((in >> 2) & 0xf8) | ((in >> 7) & 7);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline uint32_t TConvertToNative<FORMAT_R5G5B5A1>(const Color4 &in) {
  return ((in.r & 0xf8) << 8) | ((in.g & 0xf8) << 3) | ((in.b & 0xf8) >> 2) |
         (in.a ? 0x1 : 0);
}

template <>
inline void TConvertFromNative<FORMAT_R5G5B5A1, false>(Color4 *pOut,
                                                       uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff * (in & 0x1);
  pOut->r = ((in >> 8) & 0xf8) | (in >> 13);
  pOut->g = ((in >> 3) & 0xf8) | ((in >> 8) & 7);
  pOut->b = ((in & 0x3e) << 2) | ((in & 0x3e) >> 3);
}

template <>
inline void TConvertFromNative<FORMAT_R5G5B5A1, true>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff * (in & 0x1);
  pOut->r = ((in >> 8) & 0xf8) | (in >> 13);
  pOut->g = ((in >> 3) & 0xf8) | ((in >> 8) & 7);
  pOut->b = ((in & 0x3e) << 2) | ((in & 0x3e) >> 3);
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline uint32_t TConvertToNative<FORMAT_A4R4G4B4>(const Color4 &in) {
  return ((in.a & 0xf0) << 8) | ((in.r & 0xf0) << 4) | ((in.g & 0xf0) << 0) |
         (in.b >> 4);
}

template <>
inline void TConvertFromNative<FORMAT_A4R4G4B4, false>(Color4 *pOut,
                                                       uint32_t in) {
  ASSERT(pOut);
  pOut->a = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->r = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->g = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
  pOut->b = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
}

template <>
inline void TConvertFromNative<FORMAT_A4R4G4B4, true>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->a = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->r = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->g = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
  pOut->b = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline uint32_t TConvertToNative<FORMAT_R4G4B4A4>(const Color4 &in) {
  return ((in.r & 0xf0) << 8) | ((in.g & 0xf0) << 4) | ((in.b & 0xf0) << 0) |
         (in.a >> 4);
}

template <>
inline void TConvertFromNative<FORMAT_R4G4B4A4, false>(Color4 *pOut,
                                                       uint32_t in) {
  ASSERT(pOut);
  pOut->a = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
  pOut->r = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->g = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->b = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
}

template <>
inline void TConvertFromNative<FORMAT_R4G4B4A4, true>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->a = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
  pOut->r = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->g = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->b = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_R8G8B8>(const Color4 &in) {
  return (in.r << 16) | (in.g << 8) | in.b;
}

template <>
inline void TConvertFromNative<FORMAT_R8G8B8, false>(Color4 *pOut,
                                                     uint32_t in) {
  ASSERT(pOut);
  pOut->r = in >> 16;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

template <>
inline void TConvertFromNative<FORMAT_R8G8B8, true>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff;
  pOut->r = in >> 16;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_B8G8R8>(const Color4 &in) {
  return (in.b << 16) | (in.g << 8) | in.r;
}

template <>
inline void TConvertFromNative<FORMAT_B8G8R8, false>(Color4 *pOut,
                                                     uint32_t in) {
  ASSERT(pOut);
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in >> 16;
}

template <>
inline void TConvertFromNative<FORMAT_B8G8R8, true>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff;
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in >> 16;
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline uint32_t TConvertToNative<FORMAT_A8R8G8B8>(const Color4 &in) {
  return (in.a << 24) | (in.r << 16) | (in.g << 8) | in.b;
}

template <>
inline void TConvertFromNative<FORMAT_A8R8G8B8, false>(Color4 *pOut,
                                                       uint32_t in) {
  ASSERT(pOut);
  pOut->a = in >> 24;
  pOut->r = (in >> 16) & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

template <>
inline void TConvertFromNative<FORMAT_A8R8G8B8, true>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->a = in >> 24;
  pOut->r = (in >> 16) & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline uint32_t TConvertToNative<FORMAT_A8B8G8R8>(const Color4 &in) {
  return (in.a << 24) | (in.b << 16) | (in.g << 8) | in.r;
}

template <>
inline void TConvertFromNative<FORMAT_A8B8G8R8, false>(Color4 *pOut,
                                                       uint32_t in) {
  ASSERT(pOut);
  pOut->a = in >> 24;
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = (in >> 16) & 0xff;
}

template <>
inline void TConvertFromNative<FORMAT_A8B8G8R8, true>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->a = in >> 24;
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = (in >> 16) & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_A8>(const Color4 &in) {
  return in.a;
}

template <>
inline void TConvertFromNative<FORMAT_A8, false>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = in;
}

template <>
inline void TConvertFromNative<FORMAT_A8, true>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = in;
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_L8>(const Color4 &in) {
  return in.r;
}

template <>
inline void TConvertFromNative<FORMAT_L8, false>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->r = in;
  pOut->g = in;
  pOut->b = in;
}

template <>
inline void TConvertFromNative<FORMAT_L8, true>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = 0xff;
  pOut->r = in;
  pOut->g = in;
  pOut->b = in;
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_A8L8>(const Color4 &in) {
  return (in.a << 8) | in.r;
}

template <>
inline void TConvertFromNative<FORMAT_A8L8, false>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = in >> 8;
  pOut->r = in & 0xff;
  pOut->g = in & 0xff;
  pOut->b = in & 0xff;
}

template <>
inline void TConvertFromNative<FORMAT_A8L8, true>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->a = in >> 8;
  pOut->r = in & 0xff;
  pOut->g = in & 0xff;
  pOut->b = in & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_D16>(const Color4 &in) {
  return in.b;
}

template <>
inline void TConvertFromNative<FORMAT_D16, false>(Color4 *pOut, uint32_t in) {
  ASSERT(pOut);
  pOut->b = in;
}

//////////////////////////////////////////////////////////////////////////////

template <> inline uint32_t TConvertToNative<FORMAT_X8S8D16>(const Color4 &in) {
  return in.b;
}

template <>
inline void TConvertFromNative<FORMAT_X8S8D16, false>(Color4 *pOut,
                                                      uint32_t in) {
  ASSERT(pOut);
  pOut->b = in;
}

} // namespace Format

//////////////////////////////////////////////////////////////////////////////

template <> struct TFormatInfo<FORMAT_UNKNOWN> {
  typedef uint8_t TYPE;

  enum {
    CBSIZE = 0,
  };
};

template <> struct TFormatInfo<FORMAT_A4R4G4B4> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 4,
    RED = 4,
    GREEN = 4,
    BLUE = 4,
    LERP = 4,
  };
};

template <> struct TFormatInfo<FORMAT_R4G4B4A4> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 4,
    RED = 4,
    GREEN = 4,
    BLUE = 4,
    LERP = 4,
  };
};

template <> struct TFormatInfo<FORMAT_A1R5G5B5> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 1,
    RED = 5,
    GREEN = 5,
    BLUE = 5,
    LERP = 5,
  };
};

template <> struct TFormatInfo<FORMAT_R5G5B5A1> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 1,
    RED = 5,
    GREEN = 5,
    BLUE = 5,
    LERP = 5,
  };
};

template <> struct TFormatInfo<FORMAT_R5G6B5> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    RED = 5,
    GREEN = 6,
    BLUE = 5,
    LERP = 5,
  };
};

template <> struct TFormatInfo<FORMAT_R8G8B8> {
  typedef uint24 TYPE;

  enum {
    CBSIZE = 3,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_B8G8R8> {
  typedef uint24 TYPE;

  enum {
    CBSIZE = 3,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_A8R8G8B8> {
  typedef uint32_t TYPE;

  enum {
    CBSIZE = 4,
    ALPHA = 8,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_A8B8G8R8> {
  typedef uint32_t TYPE;

  enum {
    CBSIZE = 4,
    ALPHA = 8,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_A8> {
  typedef uint8_t TYPE;

  enum {
    CBSIZE = 1,
    ALPHA = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_L8> {
  typedef uint8_t TYPE;

  enum {
    CBSIZE = 1,
    LUMINANCE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_A8L8> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 8,
    LUMINANCE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_D16> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    DEPTH = 16,
  };
};

template <> struct TFormatInfo<FORMAT_X8S8D16> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 4,
    DEPTH = 16,
    STENCIL = 8,
  };
};

template <> struct TFormatInfo<FORMAT_PAL4_B8G8R8> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 3,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    PALETTE = 4,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_PAL4_A8B8G8R8> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 4,
    ALPHA = 8,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    PALETTE = 4,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_PAL4_R5G6B5> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    RED = 5,
    GREEN = 6,
    BLUE = 5,
    PALETTE = 4,
    LERP = 5,
  };
};

template <> struct TFormatInfo<FORMAT_PAL4_R4G4B4A4> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 4,
    RED = 4,
    GREEN = 4,
    BLUE = 4,
    PALETTE = 4,
    LERP = 4,
  };
};

template <> struct TFormatInfo<FORMAT_PAL4_R5G5B5A1> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 1,
    RED = 5,
    GREEN = 5,
    BLUE = 5,
    PALETTE = 4,
    LERP = 5,
  };
};

template <> struct TFormatInfo<FORMAT_PAL8_B8G8R8> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 3,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    PALETTE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_PAL8_A8B8G8R8> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 4,
    ALPHA = 8,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    PALETTE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_PAL8_R5G6B5> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    RED = 5,
    GREEN = 6,
    BLUE = 5,
    PALETTE = 8,
    LERP = 5,
  };
};

template <> struct TFormatInfo<FORMAT_PAL8_R4G4B4A4> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 4,
    RED = 4,
    GREEN = 4,
    BLUE = 4,
    PALETTE = 8,
    LERP = 4,
  };
};

template <> struct TFormatInfo<FORMAT_PAL8_R5G5B5A1> {
  typedef uint16_t TYPE;

  enum {
    CBSIZE = 2,
    ALPHA = 1,
    RED = 5,
    GREEN = 5,
    BLUE = 5,
    PALETTE = 8,
    LERP = 5,
  };
};