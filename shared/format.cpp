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

static const FormatInfo l_formatInfos[FORMAT_SIZE_] = {
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

namespace Format {

PFN_CONVERTTO
GetConvertTo(uint32_t pixelFormat) {
  switch (pixelFormat) {
  case FORMAT_A8:
    return &TConvertTo<FORMAT_A8>;
  case FORMAT_L8:
    return &TConvertTo<FORMAT_L8>;
  case FORMAT_A8L8:
    return &TConvertTo<FORMAT_A8L8>;
  case FORMAT_R5G6B5:
    return &TConvertTo<FORMAT_R5G6B5>;
  case FORMAT_A1R5G5B5:
    return &TConvertTo<FORMAT_A1R5G5B5>;
  case FORMAT_A4R4G4B4:
    return &TConvertTo<FORMAT_A4R4G4B4>;
  case FORMAT_R8G8B8:
    return &TConvertTo<FORMAT_R8G8B8>;
  case FORMAT_A8R8G8B8:
    return &TConvertTo<FORMAT_A8R8G8B8>;
  case FORMAT_R5G5B5A1:
    return &TConvertTo<FORMAT_R5G5B5A1>;
  case FORMAT_R4G4B4A4:
    return &TConvertTo<FORMAT_R4G4B4A4>;
  case FORMAT_B8G8R8:
    return &TConvertTo<FORMAT_B8G8R8>;
  case FORMAT_A8B8G8R8:
    return &TConvertTo<FORMAT_A8B8G8R8>;
  case FORMAT_D16:
    return &TConvertTo<FORMAT_D16>;
  case FORMAT_X8S8D16:
    return &TConvertTo<FORMAT_X8S8D16>;
  }

  return nullptr;
}

PFN_CONVERTFROM
GetConvertFrom(uint32_t pixelFormat, bool bForceAlpha) {
  if (bForceAlpha) {
    switch (pixelFormat) {
    case FORMAT_A8:
      return &TConvertFrom<FORMAT_A8, true>;
    case FORMAT_L8:
      return &TConvertFrom<FORMAT_L8, true>;
    case FORMAT_A8L8:
      return &TConvertFrom<FORMAT_A8L8, true>;
    case FORMAT_R5G6B5:
      return &TConvertFrom<FORMAT_R5G6B5, true>;
    case FORMAT_A1R5G5B5:
      return &TConvertFrom<FORMAT_A1R5G5B5, true>;
    case FORMAT_A4R4G4B4:
      return &TConvertFrom<FORMAT_A4R4G4B4, true>;
    case FORMAT_R8G8B8:
      return &TConvertFrom<FORMAT_R8G8B8, true>;
    case FORMAT_A8R8G8B8:
      return &TConvertFrom<FORMAT_A8R8G8B8, true>;
    case FORMAT_R5G5B5A1:
      return &TConvertFrom<FORMAT_R5G5B5A1, true>;
    case FORMAT_R4G4B4A4:
      return &TConvertFrom<FORMAT_R4G4B4A4, true>;
    case FORMAT_B8G8R8:
      return &TConvertFrom<FORMAT_B8G8R8, true>;
    case FORMAT_A8B8G8R8:
      return &TConvertFrom<FORMAT_A8B8G8R8, true>;
    case FORMAT_D16:
      return &TConvertFrom<FORMAT_D16, false>;
    case FORMAT_X8S8D16:
      return &TConvertFrom<FORMAT_X8S8D16, false>;
    }
  } else {
    switch (pixelFormat) {
    case FORMAT_A8:
      return &TConvertFrom<FORMAT_A8, false>;
    case FORMAT_L8:
      return &TConvertFrom<FORMAT_L8, false>;
    case FORMAT_A8L8:
      return &TConvertFrom<FORMAT_A8L8, false>;
    case FORMAT_R5G6B5:
      return &TConvertFrom<FORMAT_R5G6B5, false>;
    case FORMAT_A1R5G5B5:
      return &TConvertFrom<FORMAT_A1R5G5B5, false>;
    case FORMAT_A4R4G4B4:
      return &TConvertFrom<FORMAT_A4R4G4B4, false>;
    case FORMAT_R8G8B8:
      return &TConvertFrom<FORMAT_R8G8B8, false>;
    case FORMAT_A8R8G8B8:
      return &TConvertFrom<FORMAT_A8R8G8B8, false>;
    case FORMAT_R5G5B5A1:
      return &TConvertFrom<FORMAT_R5G5B5A1, false>;
    case FORMAT_R4G4B4A4:
      return &TConvertFrom<FORMAT_R4G4B4A4, false>;
    case FORMAT_B8G8R8:
      return &TConvertFrom<FORMAT_B8G8R8, false>;
    case FORMAT_A8B8G8R8:
      return &TConvertFrom<FORMAT_A8B8G8R8, false>;
    case FORMAT_D16:
      return &TConvertFrom<FORMAT_D16, false>;
    case FORMAT_X8S8D16:
      return &TConvertFrom<FORMAT_X8S8D16, false>;
    }
  }

  return nullptr;
}

const FormatInfo &GetInfo(uint32_t pixelFormat) {
  ASSERT(pixelFormat < FORMAT_SIZE_);
  return l_formatInfos[pixelFormat];
}

uint32_t GetNativeFormat(uint32_t pixelFormat) {
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
} // namespace Format
