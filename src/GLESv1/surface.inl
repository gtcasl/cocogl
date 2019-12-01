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

template <>
inline void GLSurface::colorFill<uint16_t>(const GLSurfaceDesc &surfDesc,
                                             uint32_t value, uint32_t mask,
                                             const Rect &rect) {
  auto width = rect.right - rect.left;
  auto height = rect.bottom - rect.top;
  auto pitch = surfDesc.Pitch;
  auto pbBits = surfDesc.pBits + rect.left * sizeof(uint16_t) + rect.top * pitch;

  auto wValue = static_cast<uint16_t>(value);
  auto wMask = static_cast<uint16_t>(mask);
  uint32_t dwInvMask = (~wMask << 16) | ~wMask;
  uint32_t dwValueMask = ((wValue & wMask) << 16) | (wValue & wMask);

  if (int32_t(width * sizeof(uint16_t)) == pitch) {
    if (0xffff == wMask) {
#ifdef ARM
      memset16(pbBits, wValue, width * height);
#else
      auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      uint32_t dwValue = (wValue << 16) | wValue;
      uint32_t size = (width >> 1) * height;
      for (uint32_t i = 0; i < size; ++i) {
        pdwBits[i] = dwValue;
      }
#endif
    } else {
      auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      uint32_t size = (width >> 1) * height;
      for (uint32_t i = 0; i < size; ++i) {
        pdwBits[i] = (pdwBits[i] & dwInvMask) | dwValueMask;
      }
    }
  } else {
    if (0xffff == (mask & 0xffff)) {
#ifdef ARM
      while (height--) {
        memset16(pbBits, static_cast<uint16_t>(value), width);
        pbBits += pitch;
      }
#else
      uint32_t dwValue = (wValue << 16) | value;
      uint32_t width_d2 = width >> 1; // Divide the width by 2
      while (height--) {
        auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (uint32_t i = 0; i < width_d2; ++i) {
          pdwBits[i] = dwValue;
        }
        pbBits += pitch;
      }
#endif
    } else {
      uint32_t width_d2 = width >> 1; // Divide the width by 2
      while (height--) {
        auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (uint32_t i = 0; i < width_d2; ++i) {
          pdwBits[i] = (pdwBits[i] & dwInvMask) | dwValueMask;
        }
        pbBits += pitch;
      }
    }
  }
}

template <>
inline void GLSurface::colorFill<uint32_t>(const GLSurfaceDesc &surfDesc,
                                             uint32_t value, uint32_t mask,
                                             const Rect &rect) {
  auto height = rect.bottom - rect.top;
  auto width = rect.right - rect.left;
  auto pitch = surfDesc.Pitch;
  auto pbBits = surfDesc.pBits + rect.left * sizeof(uint32_t) + rect.top * pitch;

  if (int32_t(width * sizeof(uint32_t)) == pitch) {
    if (0xffffffff == mask) {
#ifdef ARM
      memset32(pbBits, value, width * height);
#else
      auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      auto size = width * height;
      for (int32_t i = 0; i < size; ++i) {
        pdwBits[i] = value;
      }
#endif
    } else {
      auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      uint32_t dwValueMask = value & mask;
      uint32_t dwInvMask = ~mask;
      uint32_t size = width * height;
      for (uint32_t i = 0; i < size; ++i) {
        pdwBits[i] = (pdwBits[i] & dwInvMask) | dwValueMask;
      }
    }
  } else {
    if (0xffffffff == mask) {
#ifdef ARM
      while (height--) {
        memset32(pbBits, value, width);
        pbBits += pitch;
      }
#else
      while (height--) {
        auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (int32_t i = 0; i < width; ++i) {
          pdwBits[i] = value;
        }
        pbBits += pitch;
      }
#endif
    } else {
      uint32_t dwValueMask = value & mask;
      uint32_t dwInvMask = ~mask;
      while (height--) {
        auto pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (int32_t i = 0; i < width; ++i) {
          pdwBits[i] = (pdwBits[i] & dwInvMask) | dwValueMask;
        }
        pbBits += pitch;
      }
    }
  }
}
