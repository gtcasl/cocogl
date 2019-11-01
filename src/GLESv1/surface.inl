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
inline void CGLSurface::TColorFill<uint16_t>(const GLSurfaceDesc &surfDesc,
                                             uint32_t value, uint32_t mask,
                                             const Rect &rect) {
  uint32_t width = rect.right - rect.left;
  uint32_t height = rect.bottom - rect.top;
  int32_t pitch = surfDesc.Pitch;
  uint8_t *pbBits =
      surfDesc.pBits + rect.left * sizeof(uint16_t) + rect.top * pitch;

  const uint16_t wValue = static_cast<uint16_t>(value);
  const uint16_t wMask = static_cast<uint16_t>(mask);
  const uint32_t dwInvMask = (~wMask << 16) | ~wMask;
  const uint32_t dwValueMask = ((wValue & wMask) << 16) | (wValue & wMask);

  if (int(width * sizeof(uint16_t)) == pitch) {
    if (0xffff == wMask) {
#ifdef ARM
      memset16(pbBits, wValue, width * height);
#else
      uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      const uint32_t dwValue = (wValue << 16) | wValue;
      const uint32_t size = (width >> 1) * height;
      for (uint32_t i = 0; i < size; ++i) {
        pdwBits[i] = dwValue;
      }

#endif
    } else {
      uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      const uint32_t size = (width >> 1) * height;
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
      const uint32_t dwValue = (wValue << 16) | value;
      const uint32_t width_d2 = width >> 1; // Divide the width by 2
      while (height--) {
        uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (uint32_t i = 0; i < width_d2; ++i) {
          pdwBits[i] = dwValue;
        }

        pbBits += pitch;
      }

#endif
    } else {
      const uint32_t width_d2 = width >> 1; // Divide the width by 2
      while (height--) {
        uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (uint32_t i = 0; i < width_d2; ++i) {
          pdwBits[i] = (pdwBits[i] & dwInvMask) | dwValueMask;
        }

        pbBits += pitch;
      }
    }
  }
}

template <>
inline void CGLSurface::TColorFill<uint32_t>(const GLSurfaceDesc &surfDesc,
                                             uint32_t value, uint32_t mask,
                                             const Rect &rect) {
  uint32_t height = rect.bottom - rect.top;
  uint32_t width = rect.right - rect.left;
  int32_t pitch = surfDesc.Pitch;
  uint8_t *pbBits =
      surfDesc.pBits + rect.left * sizeof(uint32_t) + rect.top * pitch;

  if (int(width * sizeof(uint32_t)) == pitch) {
    if (0xffffffff == mask) {
#ifdef ARM
      memset32(pbBits, value, width * height);
#else
      uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      const uint32_t size = width * height;
      for (uint32_t i = 0; i < size; ++i) {
        pdwBits[i] = value;
      }

#endif
    } else {
      uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
      const uint32_t dwValueMask = value & mask;
      const uint32_t dwInvMask = ~mask;
      const uint32_t size = width * height;
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
        uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (uint32_t i = 0; i < width; ++i) {
          pdwBits[i] = value;
        }

        pbBits += pitch;
      }

#endif
    } else {
      const uint32_t dwValueMask = value & mask;
      const uint32_t dwInvMask = ~mask;
      while (height--) {
        uint32_t *const pdwBits = reinterpret_cast<uint32_t *>(pbBits);
        for (uint32_t i = 0; i < width; ++i) {
          pdwBits[i] = (pdwBits[i] & dwInvMask) | dwValueMask;
        }

        pbBits += pitch;
      }
    }
  }
}
