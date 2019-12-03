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
#include "surface.hpp"
#include "surface.inl"

GLSurface::GLSurface() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  memset(&colorDesc_, 0, sizeof(colorDesc_));
  memset(&depthStencilDesc_, 0, sizeof(depthStencilDesc_));

  pfnColorConv_ = &GLSurface::colorConvNoop;
  pfnColorFill_ = &GLSurface::colorFillNoop;
  pfnDepthStencilFill_ = &GLSurface::colorFillNoop;

  for (uint32_t i = ATTRIBUTES_FIRST; i <= ATTRIBUTES_LAST; ++i) {
    this->setAttribute(i, 0);
  }
}

GLSurface::~GLSurface() { __profileAPI(" - %s()\n", __FUNCTION__); }

GLenum GLSurface::Create(GLSurface **ppSurface, const GLSurfaceDesc *pColorDesc,
                         const GLSurfaceDesc *pDepthStencilDesc) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new GLSurface();
  if (nullptr == pSurface) {
    __glLogError("GLSurface allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  pSurface->addRef();

  // Initialize the surface
  err = pSurface->initialize(pColorDesc, pDepthStencilDesc);
  if (__glFailed(err)) {
    __glLogError("GLSurface::initialize() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return err;
  }

  *ppSurface = pSurface;

  return GL_NO_ERROR;
}

GLenum GLSurface::initialize(const GLSurfaceDesc *pColorDesc,
                             const GLSurfaceDesc *pDepthStencilDesc) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (pColorDesc && pColorDesc->pBits) {
    colorDesc_ = *pColorDesc;

    // Set color fill function
    switch (pColorDesc->Format) {
    case FORMAT_R5G6B5:
      pfnColorConv_ = Format::ConvertTo<FORMAT_R5G6B5>;
      pfnColorFill_ = &GLSurface::colorFill<uint16_t>;
      this->setAttribute(GL_RED_BITS, 5);
      this->setAttribute(GL_GREEN_BITS, 6);
      this->setAttribute(GL_BLUE_BITS, 5);
      this->setAttribute(GL_ALPHA_BITS, 0);
      break;

      /*case FORMAT_R8G8B8:
          pfnColorConv_ = Format::ConvertTo<FORMAT_B8G8R8>;
          pfnColorFill_ = &GLSurface::TColorFill<uint24_t>;
          this->setAttribute(GL_RED_BITS,   8);
          this->setAttribute(GL_GREEN_BITS, 8);
          this->setAttribute(GL_BLUE_BITS,  8);
          this->setAttribute(GL_ALPHA_BITS, 0);
          break;*/

    case FORMAT_A8R8G8B8:
      pfnColorConv_ = Format::ConvertTo<FORMAT_A8B8G8R8>;
      pfnColorFill_ = &GLSurface::colorFill<uint32_t>;
      this->setAttribute(GL_RED_BITS, 8);
      this->setAttribute(GL_GREEN_BITS, 8);
      this->setAttribute(GL_BLUE_BITS, 8);
      this->setAttribute(GL_ALPHA_BITS, 8);
      break;
    default:
      __glLogError(
          "GLSurface::initialize() failed, invalid color format: %d.\r\n",
          pColorDesc->Format);
      return GL_INVALID_VALUE;
    }

    if ((pColorDesc->Width & 0x1) // The width is a multiple of 2
        || !__isAligned32(
               pColorDesc->pBits)) // The buffer address is uint32_t aligned
    {
      __glLogError("GLSurface::initialize() failed, invalid color buffer "
                   "alignment.\r\n");
      return GL_INVALID_VALUE;
    }
  }

  if (pDepthStencilDesc && pDepthStencilDesc->pBits) {
    depthStencilDesc_ = *pDepthStencilDesc;

    switch (pDepthStencilDesc->Format) {
    case FORMAT_X8S8D16:
      pfnDepthStencilFill_ = &GLSurface::colorFill<uint32_t>;
      this->setAttribute(GL_DEPTH_BITS, 16);
      this->setAttribute(GL_STENCIL_BITS, 8);
      break;

    case FORMAT_D16:
      pfnDepthStencilFill_ = &GLSurface::colorFill<uint16_t>;
      this->setAttribute(GL_DEPTH_BITS, 16);
      this->setAttribute(GL_STENCIL_BITS, 0);
      break;

    default:
      __glLogError("GLSurface::initialize() failed, invalid depth stencil "
                   "format: %d.\r\n",
                   pDepthStencilDesc->Format);
      return GL_INVALID_VALUE;
    }

    if ((pDepthStencilDesc->Width & 0x1) // The width is a multiple of 2
        ||
        !__isAligned32(
            pDepthStencilDesc->pBits)) // The buffer address is uint32_t aligned
    {
      __glLogError("GLSurface::initialize() failed, invalid depth stencil "
                   "buffer alignment.\r\n");
      return GL_INVALID_VALUE;
    }
  }

  this->setAttribute(GL_SUBPIXEL_BITS, SUBPIXEL_BITS);

  return GL_NO_ERROR;
}

GLenum GLSurface::saveBitmap(const char *filename) {
  BITMAPFILEHEADER header;
  header.bfSize = 0;
  header.bfType = BF_TYPE;
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits = 0;

  struct bmp_info_header_t {
    BITMAPINFOHEADER bmiHeader;
    uint32_t bmiColors[3];
  } bmp_info;

  uint8_t nBPP = Format::GetInfo(colorDesc_.Format).BytePerPixel;

  bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmp_info.bmiHeader.biWidth = colorDesc_.Width;
  bmp_info.bmiHeader.biHeight = colorDesc_.Height;
  bmp_info.bmiHeader.biPlanes = 1;
  bmp_info.bmiHeader.biXPelsPerMeter = 0;
  bmp_info.bmiHeader.biYPelsPerMeter = 0;
  bmp_info.bmiHeader.biClrUsed = 0;
  bmp_info.bmiHeader.biClrImportant = 0;

  uint32_t infoSize;

  if (2 == nBPP) {
    bmp_info.bmiHeader.biBitCount = 16;
    bmp_info.bmiHeader.biCompression = BI_BITFIELDS;
    bmp_info.bmiColors[0] = 0xF800;
    bmp_info.bmiColors[1] = 0x07E0;
    bmp_info.bmiColors[2] = 0x001F;
    infoSize = sizeof(bmp_info_header_t);
  }
  if (3 == nBPP || 4 == nBPP) {
    bmp_info.bmiHeader.biBitCount = 24;
    bmp_info.bmiHeader.biCompression = BI_RGB;
    infoSize = sizeof(BITMAPINFOHEADER);
  } else {
    return GL_INVALID_OPERATION;
  }

  bmp_info.bmiHeader.biSizeImage = colorDesc_.Height *
                                   bmp_info.bmiHeader.biWidth *
                                   (bmp_info.bmiHeader.biBitCount / 8);
  header.bfOffBits = sizeof(BITMAPFILEHEADER) + infoSize;
  header.bfSize = header.bfOffBits + bmp_info.bmiHeader.biSizeImage;

  auto pBits = colorDesc_.pBits;
  if (colorDesc_.Pitch < 0) {
    bmp_info.bmiHeader.biHeight *= -1;
    int32_t offset = colorDesc_.Pitch * (colorDesc_.Height - 1);
    pBits += offset;
  }

  auto pFile = fopen(filename, "w");
  if (nullptr == pFile) {
    return GL_INVALID_OPERATION;
  }

  if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), pFile) !=
      sizeof(BITMAPFILEHEADER)) {
    fclose(pFile);
    return GL_OUT_OF_MEMORY;
  }

  if (fwrite(&bmp_info, 1, infoSize, pFile) != infoSize) {
    fclose(pFile);
    return GL_OUT_OF_MEMORY;
  }

  if (4 == nBPP) {
    for (uint32_t offset = 0; offset < bmp_info.bmiHeader.biSizeImage;
         offset += 3) {
      if (fwrite(pBits + (4 * (offset / 3)), 1, 3, pFile) != 3) {
        fclose(pFile);
        return GL_OUT_OF_MEMORY;
      }
    }
  } else {
    if (fwrite(pBits, 1, bmp_info.bmiHeader.biSizeImage, pFile) !=
        bmp_info.bmiHeader.biSizeImage) {
      fclose(pFile);
      return GL_OUT_OF_MEMORY;
    }
  }

  fclose(pFile);

  return GL_NO_ERROR;
}
