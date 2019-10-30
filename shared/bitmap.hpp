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

// file header
struct BITMAPFILEHEADER {
  unsigned short bfType; // magic number
  unsigned int bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned int bfOffBits; // offset to data
};

#define BF_TYPE 0x4D42

// file info
struct BITMAPINFOHEADER {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short biPlanes;     // number of color planes
  unsigned short biBitCount;   // number of bits per pixel
  unsigned int biCompression;  // type of compression to use
  unsigned int biSizeImage;    // size of image data
  int biXPelsPerMeter;         // X pixels per meter
  int biYPelsPerMeter;         // Y pixels per meter
  unsigned int biClrUsed;      // number of colors used
  unsigned int biClrImportant; // number of important colors
};

#define BI_RGB 0       // no compression
#define BI_RLE8 1      // 8-bit run-length compression
#define BI_RLE4 2      // 4-bit run-length compression
#define BI_BITFIELDS 3 // with RGB masks

struct RGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
};

struct BITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[256];
};

#endif