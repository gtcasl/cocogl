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

#include "int24.hpp"

enum ePixelFormat {
  FORMAT_UNKNOWN,
  FORMAT_A8,
  FORMAT_L8,
  FORMAT_A8L8,
  FORMAT_R5G6B5,
  FORMAT_A8R8G8B8,
  FORMAT_A1R5G5B5,
  FORMAT_R8G8B8,
  FORMAT_A4R4G4B4,
  FORMAT_A8B8G8R8,
  FORMAT_R5G5B5A1,
  FORMAT_B8G8R8,
  FORMAT_R4G4B4A4,
  FORMAT_COLOR_SIZE_,
  FORMAT_D16 = FORMAT_COLOR_SIZE_,
  FORMAT_X8S8D16,
  FORMAT_PAL4_B8G8R8,
  FORMAT_PAL4_A8B8G8R8,
  FORMAT_PAL4_R5G6B5,
  FORMAT_PAL4_R4G4B4A4,
  FORMAT_PAL4_R5G5B5A1,
  FORMAT_PAL8_B8G8R8,
  FORMAT_PAL8_A8B8G8R8,
  FORMAT_PAL8_R5G6B5,
  FORMAT_PAL8_R4G4B4A4,
  FORMAT_PAL8_R5G5B5A1,
  FORMAT_SIZE_,
};

#define FORMAT_A FORMAT_A8
#define FORMAT_RGB FORMAT_R5G6B5
#define FORMAT_RGB_ FORMAT_R8G8B8
#define FORMAT_ARGB FORMAT_A8R8G8B8
#define FORMAT_ARGB_ FORMAT_A4R4G4B4

template <uint32_t PixelFormat> struct TFormatInfo {};

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
  typedef uint24_t TYPE;

  enum {
    CBSIZE = 3,
    RED = 8,
    GREEN = 8,
    BLUE = 8,
    LERP = 8,
  };
};

template <> struct TFormatInfo<FORMAT_B8G8R8> {
  typedef uint24_t TYPE;

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

///////////////////////////////////////////////////////////////////////////////

#define DEF_GET_ENUM_VALUE(Name, Default)                                      \
  template <typename T, typename Enable = void> struct enum_get_##Name {       \
    static constexpr int value = Default;                                      \
  };                                                                           \
  template <typename T>                                                        \
  struct enum_get_##Name<T, std::enable_if_t<(T::Name != 0)>> {                \
    static constexpr int value = T::Name;                                      \
  }

#define __formatInfo(format)                                                   \
  {                                                                            \
    TFormatInfo<format>::CBSIZE, TFormatSize<TFormatInfo<format>>::RED,        \
        TFormatSize<TFormatInfo<format>>::GREEN,                               \
        TFormatSize<TFormatInfo<format>>::BLUE,                                \
        TFormatSize<TFormatInfo<format>>::ALPHA,                               \
        TFormatSize<TFormatInfo<format>>::LUMINANCE,                           \
        TFormatSize<TFormatInfo<format>>::DEPTH,                               \
        TFormatSize<TFormatInfo<format>>::STENCIL,                             \
        TFormatSize<TFormatInfo<format>>::PALETTE,                             \
        TFormatSize<TFormatInfo<format>>::LERP                                 \
  }

///////////////////////////////////////////////////////////////////////////////

struct FormatInfo {
  uint8_t BytePerPixel;
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  uint8_t Alpha;
  uint8_t Luminance;
  uint8_t Depth;
  uint8_t Stencil;
  uint8_t PaletteBits;
  uint8_t LerpBits;
};

template <typename F> class TFormatSize {
protected:
  DEF_GET_ENUM_VALUE(RED, 0);
  DEF_GET_ENUM_VALUE(GREEN, 0);
  DEF_GET_ENUM_VALUE(BLUE, 0);
  DEF_GET_ENUM_VALUE(ALPHA, 0);
  DEF_GET_ENUM_VALUE(LUMINANCE, 0);
  DEF_GET_ENUM_VALUE(DEPTH, 0);
  DEF_GET_ENUM_VALUE(STENCIL, 0);
  DEF_GET_ENUM_VALUE(PALETTE, 0);
  DEF_GET_ENUM_VALUE(LERP, 0);

public:
  enum {
    RED = enum_get_RED<F>::value,
    GREEN = enum_get_GREEN<F>::value,
    BLUE = enum_get_BLUE<F>::value,
    ALPHA = enum_get_ALPHA<FILE>::value,
    LUMINANCE = enum_get_LUMINANCE<F>::value,
    DEPTH = enum_get_DEPTH<F>::value,
    STENCIL = enum_get_STENCIL<F>::value,
    PALETTE = enum_get_PALETTE<F>::value,
    LERP = enum_get_LERP<F>::value,

    RGB = RED + GREEN + BLUE + LUMINANCE,
    RGBA = RGB + ALPHA
  };
};

namespace Format {

inline static const FormatInfo &GetInfo(uint32_t pixelFormat) {
  static const FormatInfo sc_formatInfos[FORMAT_SIZE_] = {
      __formatInfo(FORMAT_UNKNOWN),
      __formatInfo(FORMAT_A8),
      __formatInfo(FORMAT_L8),
      __formatInfo(FORMAT_A8L8),
      __formatInfo(FORMAT_RGB),
      __formatInfo(FORMAT_ARGB),
      __formatInfo(FORMAT_A1R5G5B5),
      __formatInfo(FORMAT_RGB_),
      __formatInfo(FORMAT_ARGB_),
      __formatInfo(FORMAT_R4G4B4A4),
      __formatInfo(FORMAT_R5G5B5A1),
      __formatInfo(FORMAT_B8G8R8),
      __formatInfo(FORMAT_A8B8G8R8),
      __formatInfo(FORMAT_D16),
      __formatInfo(FORMAT_X8S8D16),
      __formatInfo(FORMAT_PAL4_B8G8R8),
      __formatInfo(FORMAT_PAL4_A8B8G8R8),
      __formatInfo(FORMAT_PAL4_R5G6B5),
      __formatInfo(FORMAT_PAL4_R4G4B4A4),
      __formatInfo(FORMAT_PAL4_R5G5B5A1),
      __formatInfo(FORMAT_PAL8_B8G8R8),
      __formatInfo(FORMAT_PAL8_A8B8G8R8),
      __formatInfo(FORMAT_PAL8_R5G6B5),
      __formatInfo(FORMAT_PAL8_R4G4B4A4),
      __formatInfo(FORMAT_PAL8_R5G5B5A1),
  };
  assert(pixelFormat < FORMAT_SIZE_);
  return sc_formatInfos[pixelFormat];
}

#undef __formatInfo
#undef DEF_GET_ENUM_VALUE

typedef void (*pfn_convert_from)(Color4 *pOut, const void *pIn);

typedef void (*pfn_convert_to)(void *pOut, const Color4 &in);

template <uint32_t PixelFormat> static uint32_t ConvertTo(const Color4 &color);

template <uint32_t PixelFormat> static void ConvertTo(void *pOut, const Color4 &in) {
  *reinterpret_cast<typename TFormatInfo<PixelFormat>::TYPE *>(pOut) =
      static_cast<typename TFormatInfo<PixelFormat>::TYPE>(
          ConvertTo<PixelFormat>(in));
}

template <uint32_t PixelFormat, bool bForceAlpha>
static void ConvertFrom(Color4 *pOut, uint32_t in);

template <uint32_t PixelFormat, bool bForceAlpha>
static void ConvertFrom(Color4 *pOut, const void *pIn) {
  ConvertFrom<PixelFormat, bForceAlpha>(
      pOut,
      *reinterpret_cast<const typename TFormatInfo<PixelFormat>::TYPE *>(pIn));
}

inline static pfn_convert_to GetConvertTo(uint32_t pixelFormat) {
  switch (pixelFormat) {
  case FORMAT_A8:
    return &ConvertTo<FORMAT_A8>;
  case FORMAT_L8:
    return &ConvertTo<FORMAT_L8>;
  case FORMAT_A8L8:
    return &ConvertTo<FORMAT_A8L8>;
  case FORMAT_R5G6B5:
    return &ConvertTo<FORMAT_R5G6B5>;
  case FORMAT_A1R5G5B5:
    return &ConvertTo<FORMAT_A1R5G5B5>;
  case FORMAT_A4R4G4B4:
    return &ConvertTo<FORMAT_A4R4G4B4>;
  case FORMAT_R8G8B8:
    return &ConvertTo<FORMAT_R8G8B8>;
  case FORMAT_A8R8G8B8:
    return &ConvertTo<FORMAT_A8R8G8B8>;
  case FORMAT_R5G5B5A1:
    return &ConvertTo<FORMAT_R5G5B5A1>;
  case FORMAT_R4G4B4A4:
    return &ConvertTo<FORMAT_R4G4B4A4>;
  case FORMAT_B8G8R8:
    return &ConvertTo<FORMAT_B8G8R8>;
  case FORMAT_A8B8G8R8:
    return &ConvertTo<FORMAT_A8B8G8R8>;
  case FORMAT_D16:
    return &ConvertTo<FORMAT_D16>;
  case FORMAT_X8S8D16:
    return &ConvertTo<FORMAT_X8S8D16>;
  }
  return nullptr;
}

inline static pfn_convert_from GetConvertFrom(uint32_t pixelFormat, bool bForceAlpha) {
  if (bForceAlpha) {
    switch (pixelFormat) {
    case FORMAT_A8:
      return &ConvertFrom<FORMAT_A8, true>;
    case FORMAT_L8:
      return &ConvertFrom<FORMAT_L8, true>;
    case FORMAT_A8L8:
      return &ConvertFrom<FORMAT_A8L8, true>;
    case FORMAT_R5G6B5:
      return &ConvertFrom<FORMAT_R5G6B5, true>;
    case FORMAT_A1R5G5B5:
      return &ConvertFrom<FORMAT_A1R5G5B5, true>;
    case FORMAT_A4R4G4B4:
      return &ConvertFrom<FORMAT_A4R4G4B4, true>;
    case FORMAT_R8G8B8:
      return &ConvertFrom<FORMAT_R8G8B8, true>;
    case FORMAT_A8R8G8B8:
      return &ConvertFrom<FORMAT_A8R8G8B8, true>;
    case FORMAT_R5G5B5A1:
      return &ConvertFrom<FORMAT_R5G5B5A1, true>;
    case FORMAT_R4G4B4A4:
      return &ConvertFrom<FORMAT_R4G4B4A4, true>;
    case FORMAT_B8G8R8:
      return &ConvertFrom<FORMAT_B8G8R8, true>;
    case FORMAT_A8B8G8R8:
      return &ConvertFrom<FORMAT_A8B8G8R8, true>;
    case FORMAT_D16:
      return &ConvertFrom<FORMAT_D16, false>;
    case FORMAT_X8S8D16:
      return &ConvertFrom<FORMAT_X8S8D16, false>;
    }
  } else {
    switch (pixelFormat) {
    case FORMAT_A8:
      return &ConvertFrom<FORMAT_A8, false>;
    case FORMAT_L8:
      return &ConvertFrom<FORMAT_L8, false>;
    case FORMAT_A8L8:
      return &ConvertFrom<FORMAT_A8L8, false>;
    case FORMAT_R5G6B5:
      return &ConvertFrom<FORMAT_R5G6B5, false>;
    case FORMAT_A1R5G5B5:
      return &ConvertFrom<FORMAT_A1R5G5B5, false>;
    case FORMAT_A4R4G4B4:
      return &ConvertFrom<FORMAT_A4R4G4B4, false>;
    case FORMAT_R8G8B8:
      return &ConvertFrom<FORMAT_R8G8B8, false>;
    case FORMAT_A8R8G8B8:
      return &ConvertFrom<FORMAT_A8R8G8B8, false>;
    case FORMAT_R5G5B5A1:
      return &ConvertFrom<FORMAT_R5G5B5A1, false>;
    case FORMAT_R4G4B4A4:
      return &ConvertFrom<FORMAT_R4G4B4A4, false>;
    case FORMAT_B8G8R8:
      return &ConvertFrom<FORMAT_B8G8R8, false>;
    case FORMAT_A8B8G8R8:
      return &ConvertFrom<FORMAT_A8B8G8R8, false>;
    case FORMAT_D16:
      return &ConvertFrom<FORMAT_D16, false>;
    case FORMAT_X8S8D16:
      return &ConvertFrom<FORMAT_X8S8D16, false>;
    }
  }

  return nullptr;
}

inline static uint32_t GetNativeFormat(uint32_t pixelFormat) {
  switch (pixelFormat) {
  case FORMAT_PAL4_B8G8R8:
  case FORMAT_PAL8_B8G8R8:
    return FORMAT_B8G8R8;

  case FORMAT_PAL4_A8B8G8R8:
  case FORMAT_PAL8_A8B8G8R8:
    return FORMAT_A8B8G8R8;

  case FORMAT_PAL4_R5G6B5:
  case FORMAT_PAL8_R5G6B5:
    return FORMAT_R5G6B5;

  case FORMAT_PAL4_R4G4B4A4:
  case FORMAT_PAL8_R4G4B4A4:
    return FORMAT_R4G4B4A4;

  case FORMAT_PAL4_R5G5B5A1:
  case FORMAT_PAL8_R5G5B5A1:
    return FORMAT_R5G5B5A1;
  }
  return pixelFormat;
}

///////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_UNKNOWN>(const Color4 &in) {
  __unreferenced(in);
  return 0;
}

template <>
inline void ConvertFrom<FORMAT_UNKNOWN, false>(Color4 *pOut, uint32_t in) {
  __unreferenced(pOut);
  __unreferenced(in);
}

template <>
inline void ConvertFrom<FORMAT_UNKNOWN, true>(Color4 *pOut, uint32_t in) {
  __unreferenced(pOut);
  __unreferenced(in);
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_R5G6B5>(const Color4 &in) {
  return ((in.r & 0xf8) << 8) | ((in.g & 0xfc) << 3) | (in.b >> 3);
}

template <>
inline void ConvertFrom<FORMAT_R5G6B5, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->r = ((in >> 11) << 3) | (in >> 13);
  pOut->g = ((in >> 3) & 0xfc) | ((in >> 9) & 0x3);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

template <>
inline void ConvertFrom<FORMAT_R5G6B5, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff;
  pOut->r = ((in >> 11) << 3) | (in >> 13);
  pOut->g = ((in >> 3) & 0xfc) | ((in >> 9) & 0x3);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_A1R5G5B5>(const Color4 &in) {
  return (in.a ? 0x8000 : 0) | ((in.r & 0xf8) << 7) | ((in.g & 0xf8) << 2) |
         (in.b >> 3);
}

template <>
inline void ConvertFrom<FORMAT_A1R5G5B5, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff * (in >> 15);
  pOut->r = ((in >> 7) & 0xf8) | ((in << 1) >> 13);
  pOut->g = ((in >> 2) & 0xf8) | ((in >> 7) & 7);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

template <>
inline void ConvertFrom<FORMAT_A1R5G5B5, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff * (in >> 15);
  pOut->r = ((in >> 7) & 0xf8) | ((in << 1) >> 13);
  pOut->g = ((in >> 2) & 0xf8) | ((in >> 7) & 7);
  pOut->b = ((in & 0x1f) << 3) | ((in & 0x1c) >> 2);
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_R5G5B5A1>(const Color4 &in) {
  return ((in.r & 0xf8) << 8) | ((in.g & 0xf8) << 3) | ((in.b & 0xf8) >> 2) |
         (in.a ? 0x1 : 0);
}

template <>
inline void ConvertFrom<FORMAT_R5G5B5A1, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff * (in & 0x1);
  pOut->r = ((in >> 8) & 0xf8) | (in >> 13);
  pOut->g = ((in >> 3) & 0xf8) | ((in >> 8) & 7);
  pOut->b = ((in & 0x3e) << 2) | ((in & 0x3e) >> 3);
}

template <>
inline void ConvertFrom<FORMAT_R5G5B5A1, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff * (in & 0x1);
  pOut->r = ((in >> 8) & 0xf8) | (in >> 13);
  pOut->g = ((in >> 3) & 0xf8) | ((in >> 8) & 7);
  pOut->b = ((in & 0x3e) << 2) | ((in & 0x3e) >> 3);
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_A4R4G4B4>(const Color4 &in) {
  return ((in.a & 0xf0) << 8) | ((in.r & 0xf0) << 4) | ((in.g & 0xf0) << 0) |
         (in.b >> 4);
}

template <>
inline void ConvertFrom<FORMAT_A4R4G4B4, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->r = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->g = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
  pOut->b = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
}

template <>
inline void ConvertFrom<FORMAT_A4R4G4B4, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->r = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->g = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
  pOut->b = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_R4G4B4A4>(const Color4 &in) {
  return ((in.r & 0xf0) << 8) | ((in.g & 0xf0) << 4) | ((in.b & 0xf0) << 0) |
         (in.a >> 4);
}

template <>
inline void ConvertFrom<FORMAT_R4G4B4A4, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
  pOut->r = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->g = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->b = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
}

template <>
inline void ConvertFrom<FORMAT_R4G4B4A4, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = ((in & 0x0f) << 4) | ((in & 0x0f) >> 0);
  pOut->r = ((in >> 8) & 0xf0) | (in >> 12);
  pOut->g = ((in >> 4) & 0xf0) | ((in >> 8) & 0x0f);
  pOut->b = ((in & 0xf0) >> 0) | ((in & 0xf0) >> 4);
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_R8G8B8>(const Color4 &in) {
  return (in.r << 16) | (in.g << 8) | in.b;
}

template <>
inline void ConvertFrom<FORMAT_R8G8B8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->r = in >> 16;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

template <>
inline void ConvertFrom<FORMAT_R8G8B8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff;
  pOut->r = in >> 16;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_B8G8R8>(const Color4 &in) {
  return (in.b << 16) | (in.g << 8) | in.r;
}

template <>
inline void ConvertFrom<FORMAT_B8G8R8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in >> 16;
}

template <>
inline void ConvertFrom<FORMAT_B8G8R8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff;
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in >> 16;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_A8R8G8B8>(const Color4 &in) {
  return (in.a << 24) | (in.r << 16) | (in.g << 8) | in.b;
}

template <>
inline void ConvertFrom<FORMAT_A8R8G8B8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in >> 24;
  pOut->r = (in >> 16) & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

template <>
inline void ConvertFrom<FORMAT_A8R8G8B8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in >> 24;
  pOut->r = (in >> 16) & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = in & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_A8B8G8R8>(const Color4 &in) {
  return (in.a << 24) | (in.b << 16) | (in.g << 8) | in.r;
}

template <>
inline void ConvertFrom<FORMAT_A8B8G8R8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in >> 24;
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = (in >> 16) & 0xff;
}

template <>
inline void ConvertFrom<FORMAT_A8B8G8R8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in >> 24;
  pOut->r = in & 0xff;
  pOut->g = (in >> 8) & 0xff;
  pOut->b = (in >> 16) & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_A8>(const Color4 &in) {
  return in.a;
}

template <>
inline void ConvertFrom<FORMAT_A8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in;
}

template <>
inline void ConvertFrom<FORMAT_A8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_L8>(const Color4 &in) {
  return in.r;
}

template <>
inline void ConvertFrom<FORMAT_L8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->r = in;
  pOut->g = in;
  pOut->b = in;
}

template <>
inline void ConvertFrom<FORMAT_L8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = 0xff;
  pOut->r = in;
  pOut->g = in;
  pOut->b = in;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_A8L8>(const Color4 &in) {
  return (in.a << 8) | in.r;
}

template <>
inline void ConvertFrom<FORMAT_A8L8, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in >> 8;
  pOut->r = in & 0xff;
  pOut->g = in & 0xff;
  pOut->b = in & 0xff;
}

template <>
inline void ConvertFrom<FORMAT_A8L8, true>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->a = in >> 8;
  pOut->r = in & 0xff;
  pOut->g = in & 0xff;
  pOut->b = in & 0xff;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_D16>(const Color4 &in) {
  return in.b;
}

template <>
inline void ConvertFrom<FORMAT_D16, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->b = in;
}

//////////////////////////////////////////////////////////////////////////////

template <> 
inline uint32_t ConvertTo<FORMAT_X8S8D16>(const Color4 &in) {
  return in.b;
}

template <>
inline void ConvertFrom<FORMAT_X8S8D16, false>(Color4 *pOut, uint32_t in) {
  assert(pOut);
  pOut->b = in;
}

} // namespace Format