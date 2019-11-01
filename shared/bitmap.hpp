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

#ifdef _WIN32
include<wingdi.h>
#else

#define BF_TYPE 0x4D42 // bitmap file type
#define BI_RGB 0       // no compression
#define BI_RLE8 1      // 8-bit run-length compression
#define BI_RLE4 2      // 4-bit run-length compression
#define BI_BITFIELDS 3 // with RGB masks

// file header
 struct __attribute__((__packed__)) BITMAPFILEHEADER {
  uint16_t bfType; // magic number
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits; // offset to data
};

// file info
struct __attribute__((__packed__)) BITMAPINFOHEADER {
  uint32_t biSize;
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;     // number of color planes
  uint16_t biBitCount;   // number of bits per pixel
  uint32_t biCompression;  // type of compression to use
  uint32_t biSizeImage;    // size of image data
  int32_t  biXPelsPerMeter;         // X pixels per meter
  int32_t  biYPelsPerMeter;         // Y pixels per meter
  uint32_t biClrUsed;      // number of colors used
  uint32_t biClrImportant; // number of important colors
};

struct __attribute__((__packed__)) RGBQUAD {
  uint8_t rgbBlue;
  uint8_t rgbGreen;
  uint8_t rgbRed;
  uint8_t rgbReserved;
};

struct __attribute__((__packed__)) BITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
};

#endif