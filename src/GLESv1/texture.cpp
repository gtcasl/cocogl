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
#include "texture.hpp"
#include "surface.hpp"

GLenum Surface2D::initialize(uint32_t width, uint32_t height,
                             ePixelFormat format) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (width > MAX_TEXTURE_SIZE) {
    __glLogError(
        "Surface2D::initialize() failed, invalid width parameter: %d.\r\n",
        width);
    return GL_INVALID_VALUE;
  }

  if (height > MAX_TEXTURE_SIZE) {
    __glLogError("Surface2D::initialize() failed, invalid height "
                 "parameter: %d.\r\n",
                 height);
    return GL_INVALID_VALUE;
  }

  uint8_t nBPP = Format::GetInfo(format).BytePerPixel;
  auto pbBits = new uint8_t[nBPP * width * height];
  if (nullptr == pbBits) {
    __glLogError("Surface2D::initialize() failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  if (bOwnedBuffer_) {
    __safeDeleteArray(pbBits_);
  }

  logWidth_ = static_cast<uint8_t>(iLog2(width));
  logHeight_ = static_cast<uint8_t>(iLog2(height));
  pitch_ = nBPP << logWidth_;
  pbBits_ = pbBits;
  format_ = static_cast<uint8_t>(format);
  bOwnedBuffer_ = true;

  return GL_NO_ERROR;
}

GLenum Surface2D::initialize(uint32_t width, uint32_t height, int32_t pitch,
                             ePixelFormat format, GLvoid *pPixels) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (width > MAX_TEXTURE_SIZE) {
    __glLogError(
        "Surface2D::initialize() failed, invalid width parameter: %d.\r\n",
        width);
    return GL_INVALID_VALUE;
  }

  if (height > MAX_TEXTURE_SIZE) {
    __glLogError("Surface2D::initialize() failed, invalid height "
                 "parameter: %d.\r\n",
                 height);
    return GL_INVALID_VALUE;
  }

  if (bOwnedBuffer_) {
    __safeDeleteArray(pbBits_);
  }

  logWidth_ = static_cast<uint8_t>(iLog2(width));
  logHeight_ = static_cast<uint8_t>(iLog2(height));
  pbBits_ = reinterpret_cast<uint8_t *>(pPixels);
  pitch_ = pitch;
  format_ = static_cast<uint8_t>(format);
  bOwnedBuffer_ = false;

  return GL_NO_ERROR;
}

GLenum Surface2D::initialize(uint32_t width, uint32_t height,
                             ePixelFormat format, const GLvoid *pPixels) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (width > MAX_TEXTURE_SIZE) {
    __glLogError(
        "Surface2D::initialize() failed, invalid width parameter: %d.\r\n",
        width);
    return GL_INVALID_VALUE;
  }

  if (height > MAX_TEXTURE_SIZE) {
    __glLogError("Surface2D::initialize() failed, invalid height "
                 "parameter: %d.\r\n",
                 height);
    return GL_INVALID_VALUE;
  }

  auto formatInfo = Format::GetInfo(format);
  auto pixelFormat = Format::GetNativeFormat(format);
  auto pfnConvertFrom = Format::GetConvertFrom(pixelFormat, false);
  auto pfnConvertTo = Format::GetConvertTo(pixelFormat);

  uint32_t paletteSize = 1 << formatInfo.PaletteBits;

  auto pColorTable = new ColorARGB[paletteSize];
  if (nullptr == pColorTable) {
    __glLogError("Surface2D::initialize() failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  uint8_t nBPP = formatInfo.BytePerPixel;
  auto pbPixels = reinterpret_cast<const uint8_t *>(pPixels);
  for (uint32_t i = 0; i < paletteSize; ++i) {
    pColorTable[i] = (pfnConvertFrom)(pbPixels);
    pbPixels += nBPP;
  }

  uint32_t surfaceSize = width * height;
  auto pbBits = new uint8_t[nBPP * surfaceSize];
  if (nullptr == pbBits) {
    delete[] pColorTable;
    __glLogError("Surface2D::initialize() failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  if (16 == paletteSize) {
    BitPtr<uint8_t, 4> pb4Bits(pbPixels);

    uint8_t *_pbBits = pbBits;
    for (uint32_t n = surfaceSize; n--;) {
      (pfnConvertTo)(_pbBits, pColorTable[*pb4Bits]);
      ++pb4Bits;
      _pbBits += nBPP;
    }
  } else if (256 == paletteSize) {
    uint8_t *_pbBits = pbBits;
    for (uint32_t n = surfaceSize; n--;) {
      (pfnConvertTo)(_pbBits, pColorTable[*pbPixels++]);
      _pbBits += nBPP;
    }
  } else {
    delete[] pbBits;
    delete[] pColorTable;
    __glLogError("Surface2D::initialize() failed, invalid texture palette "
                 "size: %d.\r\n",
                 paletteSize);
    return GL_INVALID_VALUE;
  }

  delete[] pColorTable;

  if (bOwnedBuffer_) {
    __safeDeleteArray(pbBits_);
  }

  logWidth_ = static_cast<uint8_t>(iLog2(width));
  logHeight_ = static_cast<uint8_t>(iLog2(height));
  pitch_ = nBPP << logWidth_;
  pbBits_ = reinterpret_cast<uint8_t *>(pbBits);
  format_ = static_cast<uint8_t>(format);
  bOwnedBuffer_ = true;

  return GL_NO_ERROR;
}

void Surface2D::destroy() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (bOwnedBuffer_) {
    __safeDeleteArray(pbBits_);
  }

  this->clear();
}

void Surface2D::clear() {
  pbBits_ = nullptr;
  logWidth_ = 0;
  logHeight_ = 0;
  pitch_ = 0;
  format_ = FORMAT_UNKNOWN;
  bOwnedBuffer_ = false;
}

//////////////////////////////////////////////////////////////////////////////

Texture::Texture() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  pbMipBuffer_ = nullptr;
  bIsDirty_ = true;
  handle_ = 0;
  maxMipLevel_ = 0;
  pBoundSurface_ = nullptr;

  this->Params.MinFilter = TexFilterFromEnum(GL_NEAREST_MIPMAP_LINEAR);
  this->Params.MagFilter = TexFilterFromEnum(GL_LINEAR);
  this->Params.AddressU = TexAddressFromEnum(GL_REPEAT);
  this->Params.AddressV = TexAddressFromEnum(GL_REPEAT);
  this->bGenMipMaps = false;
}

Texture::~Texture() {
  __profileAPI(" - %s()\n", __FUNCTION__);
  this->freeSurfaces();
}

GLenum Texture::Create(Texture **ppTexture) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(ppTexture);

  // Create a new texture object
  auto pTexture = new Texture();
  if (nullptr == pTexture) {
    __glLogError("Texture allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  pTexture->addRef();
  *ppTexture = pTexture;

  return GL_NO_ERROR;
}

GLenum Texture::bindSurface(GLSurface *pSurface, bool bGenMipMaps) {
  GLenum err;

  if (pBoundSurface_) {
    return GL_INVALID_VALUE;
  }

  GLSurfaceDesc colorDesc;
  pSurface->getColorDesc(&colorDesc);

  // Initialize the texture surface
  err = surfaces_[0].initialize(
      colorDesc.Width, colorDesc.Height, colorDesc.Pitch,
      static_cast<ePixelFormat>(colorDesc.Format), colorDesc.pBits);
  if (__glFailed(err)) {
    __glLogError("Surface2D::initialize() failed, err = %d.\r\n", err);
    return err;
  }

  pSurface->addRef();
  pBoundSurface_ = pSurface;

  if (bGenMipMaps && this->bGenMipMaps) {
    err = this->generateMipmaps();
    if (__glFailed(err)) {
      __glLogError("Texture::generateMipmaps() failed, err = %d.\r\n", err);
      return err;
    }
  }

  return GL_NO_ERROR;
}

GLenum Texture::releaseSurface(GLSurface *pSurface) {
  if (pBoundSurface_ != pSurface) {
    return GL_INVALID_VALUE;
  }

  this->freeSurfaces();

  return GL_NO_ERROR;
}

void Texture::freeSurfaces() {
  for (uint32_t i = 0; i <= MAX_TEXTURE_LEVELS; ++i) {
    surfaces_[i].destroy();
  }

  __safeDeleteArray(pbMipBuffer_);
  __safeRelease(pBoundSurface_);
}

GLenum Texture::generateMipmaps() {
  GLenum err;

  Surface2D &surface = surfaces_[0];

  ePixelFormat format = surface.getFormat();
  uint32_t bpp = Format::GetInfo(format).BytePerPixel;
  uint32_t width = surface.getWidth();
  uint32_t height = surface.getHeight();

  //
  // Calculate mipmaps buffer size
  //
  uint32_t cbSize = 0;
  for (uint32_t w = width, h = height; ((w > 1) || (h > 1));) {
    if (w > 1) {
      w >>= 1;
    }
    if (h > 1) {
      h >>= 1;
    }
    cbSize += w * h;
  }
  cbSize *= bpp;

  //
  // Allocate the mipmaps buffer
  //
  __safeDeleteArray(pbMipBuffer_);

  pbMipBuffer_ = new uint8_t[cbSize];
  if (nullptr == pbMipBuffer_) {
    __glLogError("Mipmap buffer allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  //
  // Generate mipmaps
  //

  uint8_t *pbMipBuffer = pbMipBuffer_;

  for (uint32_t i = 1; ((width > 1) || (height > 1)); ++i) {
    Surface2D &surfaceSrc = surfaces_[i - 1];
    Surface2D &surfaceDst = surfaces_[i];

    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }

    err = surfaceDst.initialize(width, height, width * bpp, format, pbMipBuffer);
    if (__glFailed(err)) {
      __glLogError("Surface2D::initialize() failed, err = %d.\r\n", err);
      return err;
    }

    pbMipBuffer += width * height * bpp;

    switch (format) {
    case FORMAT_A8: {
      auto pSrc = surfaceSrc.getBits();
      auto pDst = surfaceDst.getBits();

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_A8, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_A8, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_A8, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_A8, false>(pSrc2 + x1);

          ColorARGB avg;
          avg.a = (c1.a + c2.a + c3.a + c4.a) >> 2;

          Format::ConvertTo<FORMAT_A8>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_L8: {
      auto pSrc = surfaceSrc.getBits();
      auto pDst = surfaceDst.getBits();

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_L8, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_L8, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_L8, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_L8, false>(pSrc2 + x1);

          ColorARGB avg(0);
          avg.b = (c1.b + c2.b + c3.b + c4.b) >> 2;

          Format::ConvertTo<FORMAT_L8>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_A8L8: {
      auto pSrc = reinterpret_cast<const uint16_t *>(surfaceSrc.getBits());
      auto pDst = reinterpret_cast<uint16_t *>(surfaceDst.getBits());

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_A8L8, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_A8L8, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_A8L8, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_A8L8, false>(pSrc2 + x1);

          ColorARGB avg(0);
          avg.a = (c1.a + c2.a + c3.a + c4.a) >> 2;
          avg.b = (c1.b + c2.b + c3.b + c4.b) >> 2;

          Format::ConvertTo<FORMAT_A8L8>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_R5G6B5: {
      auto pSrc = reinterpret_cast<const uint16_t *>(surfaceSrc.getBits());
      auto pDst = reinterpret_cast<uint16_t *>(surfaceDst.getBits());

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_R5G6B5, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_R5G6B5, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_R5G6B5, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_R5G6B5, false>(pSrc2 + x1);

          const ColorARGB avg((c1.r + c2.r + c3.r + c4.r) >> 2,
                              (c1.g + c2.g + c3.g + c4.g) >> 2,
                              (c1.b + c2.b + c3.b + c4.b) >> 2);

          Format::ConvertTo<FORMAT_R5G6B5>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_A1R5G5B5: {
      auto pSrc = reinterpret_cast<const uint16_t *>(surfaceSrc.getBits());
      auto pDst = reinterpret_cast<uint16_t *>(surfaceDst.getBits());

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_A1R5G5B5, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_A1R5G5B5, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_A1R5G5B5, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_A1R5G5B5, false>(pSrc2 + x1);

          const ColorARGB avg((c1.a + c2.a + c3.a + c4.a) >> 2,
                              (c1.r + c2.r + c3.r + c4.r) >> 2,
                              (c1.g + c2.g + c3.g + c4.g) >> 2,
                              (c1.b + c2.b + c3.b + c4.b) >> 2);

          Format::ConvertTo<FORMAT_A1R5G5B5>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_A4R4G4B4: {
      auto pSrc = reinterpret_cast<const uint16_t *>(surfaceSrc.getBits());
      auto pDst = reinterpret_cast<uint16_t *>(surfaceDst.getBits());

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_A4R4G4B4, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_A4R4G4B4, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_A4R4G4B4, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_A4R4G4B4, false>(pSrc2 + x1);

          const ColorARGB avg((c1.a + c2.a + c3.a + c4.a) >> 2,
                              (c1.r + c2.r + c3.r + c4.r) >> 2,
                              (c1.g + c2.g + c3.g + c4.g) >> 2,
                              (c1.b + c2.b + c3.b + c4.b) >> 2);

          Format::ConvertTo<FORMAT_A4R4G4B4>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_R8G8B8: {
      auto pSrc = reinterpret_cast<const uint24_t *>(surfaceSrc.getBits());
      auto pDst = reinterpret_cast<uint24_t *>(surfaceDst.getBits());

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_R8G8B8, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_R8G8B8, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_R8G8B8, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_R8G8B8, false>(pSrc2 + x1);

          const ColorARGB avg((c1.r + c2.r + c3.r + c4.r) >> 2,
                              (c1.g + c2.g + c3.g + c4.g) >> 2,
                              (c1.b + c2.b + c3.b + c4.b) >> 2);

          Format::ConvertTo<FORMAT_R8G8B8>(pDst + x + width * y, avg);
        }
      }
    }

    break;

    case FORMAT_A8R8G8B8: {
      auto pSrc = reinterpret_cast<const uint32_t *>(surfaceSrc.getBits());
      auto pDst = reinterpret_cast<uint32_t *>(surfaceDst.getBits());

      for (uint32_t y = 0; y < height; ++y) {
        auto y0 = 2 * y;
        auto y1 = 2 * y + ((height > 1) ? 1 : 0);
        auto pSrc1 = pSrc + y0 * (2 * width);
        auto pSrc2 = pSrc + y1 * (2 * width);

        for (uint32_t x = 0; x < width; ++x) {
          auto x0 = 2 * x;
          auto x1 = 2 * x + ((width > 1) ? 1 : 0);

          auto c1 = Format::ConvertFrom<FORMAT_A8R8G8B8, false>(pSrc1 + x0);
          auto c2 = Format::ConvertFrom<FORMAT_A8R8G8B8, false>(pSrc1 + x1);
          auto c3 = Format::ConvertFrom<FORMAT_A8R8G8B8, false>(pSrc2 + x0);
          auto c4 = Format::ConvertFrom<FORMAT_A8R8G8B8, false>(pSrc2 + x1);

          const ColorARGB avg((c1.a + c2.a + c3.a + c4.a) >> 2,
                              (c1.r + c2.r + c3.r + c4.r) >> 2,
                              (c1.g + c2.g + c3.g + c4.g) >> 2,
                              (c1.b + c2.b + c3.b + c4.b) >> 2);

          Format::ConvertTo<FORMAT_A8R8G8B8>(pDst + x + width * y, avg);
        }
      }
    }

    break;
    default:
      break;
    }
  }

  return GL_NO_ERROR;
}

bool Texture::validate() {
  const Surface2D &surface0 = this->getSurface(0);

  ePixelFormat format = surface0.getFormat();
  if (FORMAT_UNKNOWN == format) {
    return false;
  }

  uint32_t level = 0;
  uint32_t width = surface0.getWidth();
  uint32_t height = surface0.getHeight();

  for (; ((width > 1) || (height > 1));) {
    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }

    const Surface2D &surfaceN = this->getSurface(level + 1);

    if ((surfaceN.getFormat() != format) || (surfaceN.getWidth() != width) ||
        (surfaceN.getHeight() != height)) {
      break;
    }

    ++level;
  }

  maxMipLevel_ = static_cast<uint8_t>(level);
  bIsDirty_ = false;

  return true;
}

///////////////////////////////////////////////////////////////////////////////

bool TexUnit::prepare(Sampler *pSampler, TEXTURESTATES *pStates) {
  assert(pSampler && pStates);

  assert(pTexture_);
  if (pTexture_->isDirty()) {
    if (!pTexture_->validate()) {
      return false;
    }
  }

  // Update texture states
  TexParams texParams = pTexture_->Params;

  switch (texParams.MinFilter) {
  default:
    __no_default;

  case FILTER_NEAREST:
  case FILTER_NEAREST_MIPMAP_NEAREST:
  case FILTER_NEAREST_MIPMAP_LINEAR:
    pStates->MinFilter = FILTER_NEAREST;
    break;

  case FILTER_LINEAR:
  case FILTER_LINEAR_MIPMAP_NEAREST:
  case FILTER_LINEAR_MIPMAP_LINEAR:
    pStates->MinFilter = FILTER_LINEAR;
    break;
  }

  switch (texParams.MinFilter) {
  default:
    __no_default;

  case FILTER_NEAREST:
  case FILTER_LINEAR:
    pStates->MipFilter = FILTER_NONE;
    break;

  case FILTER_NEAREST_MIPMAP_NEAREST:
  case FILTER_LINEAR_MIPMAP_NEAREST:
    pStates->MipFilter = FILTER_NEAREST;
    break;

  case FILTER_NEAREST_MIPMAP_LINEAR:
  case FILTER_LINEAR_MIPMAP_LINEAR:
    pStates->MipFilter = FILTER_LINEAR;
    break;
  }

  pStates->MagFilter = texParams.MagFilter;
  pStates->AddressU = texParams.AddressU;
  pStates->AddressV = texParams.AddressV;
  pStates->EnvMode = this->EnvMode;
  pStates->Format = this->getFormat();

  // Update sampler states
  pSampler->pMipLevels = pTexture_->getSurfaces();
  pSampler->MaxMipLevel = pTexture_->getMaxMipLevel();
  pSampler->cEnvColor[0] = this->cEnvColor.b;
  pSampler->cEnvColor[1] = this->cEnvColor.g;
  pSampler->cEnvColor[2] = this->cEnvColor.r;

  return true;
}
