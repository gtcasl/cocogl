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

#define FORMAT_A FORMAT_A8
#define FORMAT_RGB FORMAT_R5G6B5
#define FORMAT_RGB_ FORMAT_R8G8B8
#define FORMAT_ARGB FORMAT_A8R8G8B8
#define FORMAT_ARGB_ FORMAT_A4R4G4B4

enum ePixelFormat {
  FORMAT_UNKNOWN,
  FORMAT_A8,
  FORMAT_L8,
  FORMAT_A8L8,
  FORMAT_RGB,
  FORMAT_ARGB,
  FORMAT_A1R5G5B5,
  FORMAT_RGB_,
  FORMAT_ARGB_,
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

template <unsigned PixelFormat> struct TFormatInfo {};

#define DEF_GET_ENUM_VALUE(Name, Default) \
  template<typename T, typename Enable = void> \
  struct enum_get_##Name { \
    static constexpr int value = Default; \
  }; \
  template<typename T> \
  struct enum_get_##Name<T, std::enable_if_t<(T::Name != 0)>> { \
    static constexpr int value = T::Name; \
  }

template <typename FormatInfo> class TFormatSize {
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
    RED       = enum_get_RED<FormatInfo>::value,
    GREEN     = enum_get_GREEN<FormatInfo>::value,
    BLUE      = enum_get_BLUE<FormatInfo>::value,
    ALPHA     = enum_get_ALPHA<FormatInfo>::value,
    LUMINANCE = enum_get_LUMINANCE<FormatInfo>::value,
    DEPTH     = enum_get_DEPTH<FormatInfo>::value,
    STENCIL   = enum_get_STENCIL<FormatInfo>::value,
    PALETTE   = enum_get_PALETTE<FormatInfo>::value,
    LERP      = enum_get_LERP<FormatInfo>::value,

    RGB  = RED + GREEN + BLUE + LUMINANCE,
    RGBA = RGB + ALPHA
  };
};

namespace Format {
typedef void (*PFN_CONVERTFROM)(Color4 *pOut, const void *pIn);
typedef void (*PFN_CONVERTTO)(void *pOut, const Color4 &in);

template <unsigned PixelFormat> unsigned TConvertToNative(const Color4 &color);

template <unsigned PixelFormat, bool bForceAlpha>
void TConvertFromNative(Color4 *pOut, unsigned in);

template <unsigned PixelFormat> void TConvertTo(void *pOut, const Color4 &in) {
  *reinterpret_cast<typename TFormatInfo<PixelFormat>::TYPE *>(pOut) =
      static_cast<typename TFormatInfo<PixelFormat>::TYPE>(
          Format::TConvertToNative<PixelFormat>(in));
}

template <unsigned PixelFormat, bool bForceAlpha>
void TConvertFrom(Color4 *pOut, const void *pIn) {
  Format::TConvertFromNative<PixelFormat, bForceAlpha>(
      pOut, *reinterpret_cast<const typename TFormatInfo<PixelFormat>::TYPE *>(pIn));
}

PFN_CONVERTTO GetConvertTo(unsigned pixelFormat);

PFN_CONVERTFROM GetConvertFrom(unsigned pixelFormat, bool bForceAlpha);

const FormatInfo &GetInfo(unsigned pixelFormat);

unsigned GetNativeFormat(unsigned pixelFormat);
} // namespace Format
