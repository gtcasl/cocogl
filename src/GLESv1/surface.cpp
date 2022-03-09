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
#include <filesystem>
#include "tga.hpp"
#include "png.hpp"
#include "bmp.hpp"

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

GLSurface::~GLSurface() {
  __profileAPI(" - %s()\n", __FUNCTION__);
   assert(pfnColorConv_ != nullptr);
}

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

GLenum GLSurface::saveImage(const char *filename) {
  uint8_t bpp = Format::GetInfo((ePixelFormat)colorDesc_.Format).BytePerPixel;
  auto ext = std::filesystem::path(filename).extension();
  if (0 == ext.compare(".tga")) {
    int ret = cocogfx::SaveTGA(filename, colorDesc_.pBits, colorDesc_.Width, colorDesc_.Height, bpp, colorDesc_.Pitch);
    if (ret) {
      __glLogError("SavePNG() failed, ret=%d\r\n", ret);
      return GL_OUT_OF_MEMORY;
    }
  } else 
  if (0 == ext.compare(".png")) {
    int ret = cocogfx::SavePNG(filename, colorDesc_.pBits, colorDesc_.Width, colorDesc_.Height, bpp, colorDesc_.Pitch);
    if (ret) {
      __glLogError("SavePNG() failed, ret=%d\r\n", ret);
      return GL_OUT_OF_MEMORY;
    }
  } else 
  if (0 == ext.compare(".bmp")) {
    int ret = cocogfx::SaveBMP(filename, colorDesc_.pBits, colorDesc_.Width, colorDesc_.Height, bpp, colorDesc_.Pitch);
    if (ret) {
      __glLogError("SaveBMP() failed, ret=%d\r\n", ret);
      return GL_OUT_OF_MEMORY;
    }
  } else {
    __glLogError("saveImage() failed, invalid file extension: %s\r\n", ext);
    return GL_INVALID_OPERATION;
  }

  return GL_NO_ERROR;
}
