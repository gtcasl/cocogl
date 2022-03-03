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
#include "context.hpp"

void GLContext::genTextures(GLsizei n, GLuint *phTextures) {
  GLenum err;

  assert(phTextures);

  if (n < 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::genTextures() failed, invalid n parameter: %d.\r\n",
              n);
    return;
  }

  for (GLuint *phTex = phTextures, *const phEnd = phTex + n; phTex != phEnd;
       ++phTex) {
    Texture *pTexture;
    err = Texture::Create(&pTexture);
    if (__glFailed(err)) {
      __glError(err, "Texture::Create() failed, err = %d.\r\n", err);
      return;
    }

    uint32_t handle;
    err = GLERROR_FROM_HRESULT(
        handles_.insert(&handle, pTexture, HANDLE_TEXTURE));
    if (__glFailed(err)) {
      __safeRelease(pTexture);
      __glError(err, "HandleTable::insert() failed, err = %d.\r\n", err);
      return;
    }

    pTexture->setHandle(handle);

    *phTex = handle;
  }
}

void GLContext::bindTexture(GLenum target, GLuint texture) {
  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext::bindTexture() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  Texture *pTexture = nullptr;
  if (texture) {
    // First lookup owned textures
    pTexture = reinterpret_cast<Texture *>(handles_.getObject(texture));
    if ((nullptr == pTexture) && pCtxShared_) {
      // Second lookup the shared context textures
      pTexture = pCtxShared_->getObject<Texture *>(texture);
    }

    if (nullptr == pTexture) {
      __glError(GL_INVALID_VALUE,
                "GLContext::bindTexture() failed, "
                "Invalid texture parameter: %d\r\n",
                texture);
      return;
    }
  } else {
    pTexture = pTexDefault_;
  }

  this->setTexture(activeTexture_, pTexture);
}

void GLContext::setTexImage2D(GLenum target, GLint level, GLint internalformat,
                              GLsizei width, GLsizei height, GLint border,
                              GLenum format, GLenum type,
                              const GLvoid *pPixels) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setTexImage2D() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level > MAX_TEXTURE_LEVELS)) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setTexImage2D() failed, invalid level parameter: %d.\r\n",
        level);
    return;
  }

  if (border != 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexImage2D() failed, invalid "
              "border parameter: %d.\r\n",
              border);
    return;
  }

  if ((width < 0) || !IsPowerOf2(width)) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setTexImage2D() failed, invalid width parameter: %d.\r\n",
        width);
    return;
  }

  if ((height < 0) || !IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexImage2D() failed, invalid "
              "height parameter: %d.\r\n",
              height);
    return;
  }

  if (internalformat != static_cast<GLint>(format)) {
    __glError(GL_INVALID_OPERATION, "GLContext::setTexImage2D() failed, the "
                                    "internalformat and format are "
                                    "different.\r\n");
    return;
  }

  ePixelFormat dstFormat;
  err = ToPixelFormat(&dstFormat, internalformat);
  if (__glFailed(err)) {
    __glError(err, "ToPixelFormat() failed, err = %d.\r\n", err);
    return;
  }

  // Initialize the texture surface
  err = pTexture->InitializeSurface(level, width, height, dstFormat);
  if (__glFailed(err)) {
    __glError(err, "Texture::InitializeSurface() failed, err = %d.\r\n", err);
    return;
  }

  ePixelFormat srcFormat;
  uint32_t srcBPP;
  err = ToPixelFormat(&srcFormat, &srcBPP, format, type);
  if (__glFailed(err)) {
    __glError(err, "ToPixelFormat() failed, err = %d.\r\n", err);
    return;
  }

  if (pPixels) {
    GLSurfaceDesc srcDesc;
    srcDesc.pBits = reinterpret_cast<uint8_t *>(const_cast<GLvoid *>(pPixels));
    srcDesc.Width = width;
    srcDesc.Height = height;
    srcDesc.Pitch = __align(width * srcBPP, unpackAlignment_);
    srcDesc.Format = static_cast<uint8_t>(srcFormat);

    GLSurfaceDesc dstDesc;
    pTexture->getSurface(level).getDesc(&dstDesc);

    err = CopyBuffers(dstDesc, 0, 0, width, height, srcDesc, 0, 0);
    if (__glFailed(err)) {
      __glError(err, "CopyBuffers() failed, err = %d.\r\n", err);
      return;
    }
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->generateMipmaps();
    if (__glFailed(err)) {
      __glError(err, "Texture::generateMipmaps() failed, err = %d.\r\n", err);
      return;
    }
  }

  pTexture->Invalidate();
}

void GLContext::setTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                                 GLint yoffset, GLsizei width, GLsizei height,
                                 GLenum format, GLenum type,
                                 const GLvoid *pPixels) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext:set:TexSubImage2D() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level > MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexSubImage2D() failed, "
              "invalid level parameter: %d.\r\n",
              level);
    return;
  }

  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, "GLContext::setTexSubImage2D() failed, "
                                "invalid input dimensions.\r\n");
    return;
  }

  const Surface2D &surface2D = pTexture->getSurface(level);

  if ((xoffset < 0) || (yoffset < 0) ||
      (static_cast<uint32_t>(xoffset + width) > surface2D.getWidth()) ||
      (static_cast<uint32_t>(yoffset + height) > surface2D.getHeight())) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setTexSubImage2D() failed, invalid input offsets.\r\n");
    return;
  }

  ePixelFormat srcFormat;
  uint32_t srcBPP;
  err = ToPixelFormat(&srcFormat, &srcBPP, format, type);
  if (__glFailed(err)) {
    __glError(err, "ToPixelFormat() failed, err = %d.\r\n", err);
    return;
  }

  if (pPixels) {
    GLSurfaceDesc srcDesc;
    srcDesc.pBits = reinterpret_cast<uint8_t *>(const_cast<GLvoid *>(pPixels));
    srcDesc.Width = width;
    srcDesc.Height = height;
    srcDesc.Pitch = __align(width * srcBPP, unpackAlignment_);
    srcDesc.Format = static_cast<uint8_t>(srcFormat);

    GLSurfaceDesc dstDesc;
    surface2D.getDesc(&dstDesc);

    err = CopyBuffers(dstDesc, xoffset, yoffset, width, height, srcDesc, 0, 0);
    if (__glFailed(err)) {
      __glError(err, "CopyBuffers() failed, err = %d.\r\n", err);
      return;
    }
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->generateMipmaps();
    if (__glFailed(err)) {
      __glError(err, "Texture::generateMipmaps() failed, err = %d.\r\n", err);
      return;
    }
  }

  pTexture->Invalidate();
}

void GLContext::copyTexImage2D(GLenum target, GLint level,
                               GLenum internalformat, GLint x, GLint y,
                               GLsizei width, GLsizei height, GLint border) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setTexImage2D() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level > MAX_TEXTURE_LEVELS)) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setTexImage2D() failed, invalid level parameter: %d.\r\n",
        level);
    return;
  }

  if (border != 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexImage2D() failed, invalid "
              "border parameter: %d.\r\n",
              border);
    return;
  }

  if ((width < 0) || !IsPowerOf2(width)) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setTexImage2D() failed, invalid width parameter: %d.\r\n",
        width);
    return;
  }

  if ((height < 0) || !IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexImage2D() failed, invalid "
              "height parameter: %d.\r\n",
              height);
    return;
  }

  ePixelFormat dstFormat;
  err = ToPixelFormat(&dstFormat, internalformat);
  if (__glFailed(err)) {
    __glError(err, "ToPixelFormat() failed, err = %d.\r\n", err);
    return;
  }

  // Initialize the texture surface
  err = pTexture->InitializeSurface(level, width, height, dstFormat);
  if (__glFailed(err)) {
    __glError(err, "Texture::InitializeSurface() failed, err = %d.\r\n", err);
    return;
  }

  GLSurfaceDesc srcDesc;
  pSurfRead_->getColorDesc(&srcDesc);

  GLSurfaceDesc dstDesc;
  pTexture->getSurface(level).getDesc(&dstDesc);

  err = CopyBuffers(dstDesc, 0, 0, width, height, srcDesc, x, y);
  if (__glFailed(err)) {
    __glError(err, "CopyBuffers() failed, err = %d.\r\n", err);
    return;
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->generateMipmaps();
    if (__glFailed(err)) {
      __glError(err, "Texture::generateMipmaps() failed, err = %d.\r\n", err);
      return;
    }
  }

  pTexture->Invalidate();
}

void GLContext::copyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                                  GLint yoffset, GLint x, GLint y,
                                  GLsizei width, GLsizei height) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setTexSubImage2D() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level > MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexSubImage2D() failed, "
              "invalid level parameter: %d.\r\n",
              level);
    return;
  }

  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, "GLContext::setTexSubImage2D() failed, "
                                "invalid input dimensions.\r\n");
    return;
  }

  const Surface2D &surface2D = pTexture->getSurface(level);

  if ((xoffset < 0) || (yoffset < 0) ||
      (static_cast<uint32_t>(xoffset + width) > surface2D.getWidth()) ||
      (static_cast<uint32_t>(yoffset + height) > surface2D.getHeight())) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setTexSubImage2D() failed, invalid input offsets.\r\n");
    return;
  }

  GLSurfaceDesc srcDesc;
  pSurfRead_->getColorDesc(&srcDesc);

  GLSurfaceDesc dstDesc;
  surface2D.getDesc(&dstDesc);

  err = CopyBuffers(dstDesc, xoffset, yoffset, width, height, srcDesc, x, y);
  if (__glFailed(err)) {
    __glError(err, "CopyBuffers() failed, err = %d.\r\n", err);
    return;
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->generateMipmaps();
    if (__glFailed(err)) {
      __glError(err, "Texture::generateMipmaps() failed, err = %d.\r\n", err);
      return;
    }
  }

  pTexture->Invalidate();
}

void GLContext::readPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                           GLenum format, GLenum type, GLvoid *pPixels) {
  GLenum err;

  assert(pPixels);

  if ((format != GL_RGBA) && (format != IMPLEMENTATION_COLOR_READ_FORMAT_OES)) {
    __glError(GL_INVALID_ENUM,
              "GLContext::readPixels() failed, invalid "
              "format parameter: %d.\r\n",
              format);
    return;
  }

  if ((type != GL_UNSIGNED_BYTE) &&
      (type != IMPLEMENTATION_COLOR_READ_TYPE_OES)) {
    __glError(GL_INVALID_ENUM,
              "GLContext::readPixels() failed, invalid type parameter: %d.\r\n",
              type);
    return;
  }

  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, "GLContext::readPixels() failed, invalid "
                                "width or height parameters.\r\n");
    return;
  }

  ePixelFormat dstFormat;
  uint32_t dstBPP;
  err = ToPixelFormat(&dstFormat, &dstBPP, format, type);
  if (__glFailed(err)) {
    __glError(err, "ToPixelFormat() failed, err = %d.\r\n", err);
    return;
  }

  GLSurfaceDesc srcDesc;
  pSurfRead_->getColorDesc(&srcDesc);

  GLSurfaceDesc dstDesc;
  dstDesc.pBits = reinterpret_cast<uint8_t *>(const_cast<GLvoid *>(pPixels));
  dstDesc.Width = width;
  dstDesc.Height = height;
  dstDesc.Pitch = __align(width * dstBPP, packAlignment_);
  dstDesc.Format = static_cast<uint8_t>(dstFormat);

  err = CopyBuffers(dstDesc, 0, 0, width, height, srcDesc, x, y);
  if (__glFailed(err)) {
    __glError(err, "CopyBuffers() failed, err = %d.\r\n", err);
    return;
  }

  /*#ifndef NDEBUG
      static int s_fileno = 0;
      char filePath[MAX_PATH];
      ::GetModuleFileName( nullptr, filePath, MAX_PATH );
      PathRemoveFileSpec( filePath );
      _sntprintf( filePath, MAX_PATH, "%s%s%d.bmp", filePath,
  "CocoGLESv1_cm", s_fileno++ );
      __glLog( " - Read Surface bitmap dump: %s.\r\n", filePath );
      pSurfRead_->saveBitmap( filePath );
  #endif*/
}

void GLContext::setPixelStorei(GLenum pname, GLint param) {
  switch (pname) {
  case GL_PACK_ALIGNMENT:
    switch (param) {
    case 1:
    case 2:
    case 4:
    case 8:
      packAlignment_ = param;
      break;

    default:
      __glError(GL_INVALID_VALUE,
                "GLContext::setPixelStorei() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    break;

  case GL_UNPACK_ALIGNMENT:
    switch (param) {
    case 1:
    case 2:
    case 4:
    case 8:
      unpackAlignment_ = param;
      break;

    default:
      __glError(GL_INVALID_VALUE,
                "GLContext::setPixelStorei() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setPixelStorei() failed, invalid "
              "pname parameter: %d.\r\n",
              pname);
    return;
  }
}

void GLContext::deleteTextures(GLsizei n, const GLuint *phTextures) {
  assert(phTextures);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::deleteTextures() failed, invalid n parameter: %d.\r\n", n);
    return;
  }

  for (const GLuint *phTex = phTextures, *const phEnd = phTex + n;
       phTex != phEnd; ++phTex) {
    GLuint handle = *phTex;
    if (handle) {
      auto pTexture = reinterpret_cast<Texture *>(handles_.deleteHandle(handle));
      if (pTexture) {
        // Unbind the texture if bound.
        for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
          if (pTexture == this->getTexture(i)) {
            this->setTexture(i, pTexDefault_);
          }
        }

        pTexture->release();
      }
    }
  }
}

GLenum GLContext::bindTexImage(GLSurface *pSurface, bool bGenMipMaps) {
  GLenum err;

  assert(pSurface);

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  err = pTexture->bindSurface(pSurface, bGenMipMaps);
  if (__glFailed(err)) {
    __glLogError("Texture::bindTexImage() failed, err = %d.\r\n", err);
    return err;
  }

  return GL_NO_ERROR;
}

GLenum GLContext::releaseTexImage(GLSurface *pSurface) {
  GLenum err;

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  err = pTexture->releaseSurface(pSurface);
  if (__glFailed(err)) {
    __glLogError("Texture::releaseTexImage() failed, err = %d.\r\n", err);
    return err;
  }

  return GL_NO_ERROR;
}

void GLContext::compressedTexImage2D(GLenum target, GLint level,
                                     GLenum internalformat, GLsizei width,
                                     GLsizei height, GLint border,
                                     GLsizei imageSize, const GLvoid *pData) {
  GLenum err;

  assert(pData);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid target parameter: %d.\r\n",
              target);
    return;
  }

  if ((level < 0) || (level > MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid level parameter: %d.\r\n",
              level);
    return;
  }

  if (border != 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid border parameter: %d.\r\n",
              border);
    return;
  }

  if ((width < 0) || !IsPowerOf2(width)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid width parameter: %d.\r\n",
              width);
    return;
  }

  if ((height < 0) || !IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid height parameter: %d.\r\n",
              height);
    return;
  }

  ePixelFormat pixelformat;
  GLenum type, format;

  switch (internalformat) {
  case GL_PALETTE4_RGB8_OES:
    pixelformat = FORMAT_PAL4_B8G8R8;
    type = GL_UNSIGNED_BYTE;
    format = GL_RGB;
    break;

  case GL_PALETTE4_RGBA8_OES:
    pixelformat = FORMAT_PAL4_A8B8G8R8;
    type = GL_UNSIGNED_BYTE;
    format = GL_RGBA;
    break;

  case GL_PALETTE4_R5_G6_B5_OES:
    pixelformat = FORMAT_PAL4_R5G6B5;
    type = GL_UNSIGNED_SHORT_5_6_5;
    format = GL_RGB;
    break;

  case GL_PALETTE4_RGBA4_OES:
    pixelformat = FORMAT_PAL4_R4G4B4A4;
    type = GL_UNSIGNED_SHORT_4_4_4_4;
    format = GL_RGBA;
    break;

  case GL_PALETTE4_RGB5_A1_OES:
    pixelformat = FORMAT_PAL4_R5G5B5A1;
    type = GL_UNSIGNED_SHORT_5_5_5_1;
    format = GL_RGBA;
    break;

  case GL_PALETTE8_RGB8_OES:
    pixelformat = FORMAT_PAL8_B8G8R8;
    type = GL_UNSIGNED_BYTE;
    format = GL_RGB;
    break;

  case GL_PALETTE8_RGBA8_OES:
    pixelformat = FORMAT_PAL8_A8B8G8R8;
    type = GL_UNSIGNED_BYTE;
    format = GL_RGBA;
    break;

  case GL_PALETTE8_R5_G6_B5_OES:
    pixelformat = FORMAT_PAL8_R5G6B5;
    type = GL_UNSIGNED_SHORT_5_6_5;
    format = GL_RGB;
    break;

  case GL_PALETTE8_RGBA4_OES:
    pixelformat = FORMAT_PAL8_R4G4B4A4;
    type = GL_UNSIGNED_SHORT_4_4_4_4;
    format = GL_RGBA;
    break;

  case GL_PALETTE8_RGB5_A1_OES:
    pixelformat = FORMAT_PAL8_R5G5B5A1;
    type = GL_UNSIGNED_SHORT_5_5_5_1;
    format = GL_RGBA;
    break;

  default:
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid internalformat parameter: "
              "%d.\r\n",
              internalformat);
    return;
  }

  FormatInfo formatInfo = Format::GetInfo(pixelformat);
  int paletteSize = 1 << formatInfo.PaletteBits;
  int _imageSize = ((width * formatInfo.PaletteBits) + 7) / 8 * height +
                   paletteSize * formatInfo.BytePerPixel;
  if (_imageSize != imageSize) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexImage2D() "
              "failed, invalid imageSize parameter: "
              "%d.\r\n",
              imageSize);
    return;
  }

  GLint minLevel, maxLevel;

  if (level >= 0) {
    minLevel = maxLevel = level;
  } else {
    minLevel = 0;
    maxLevel = -level - 1;
  }

  Surface2D surface;

  for (GLint i = minLevel; i <= maxLevel; ++i) {
    // Initialize the texture surface
    err = surface.initialize(width, height, pixelformat, pData);
    if (__glFailed(err)) {
      __glError(err, "Surface2D::initialize() failed, err = %d.\r\n", err);
      return;
    }

    // Copy the texture surface
    this->setTexImage2D(target, i, format, width, height, border, format, type,
                        surface.getBits());

    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }
  }
}

void GLContext::compressedTexSubImage2D(GLenum target, GLint level,
                                        GLint xoffset, GLint yoffset,
                                        GLsizei width, GLsizei height,
                                        GLenum format, GLsizei imageSize,
                                        const GLvoid *pData) {
  GLenum err;

  assert(pData);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexSubImage2D() "
              "failed, invalid target parameter: %d.\r\n",
              target);
    return;
  }

  if ((level < 0) || (level > MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexSubImage2D() "
              "failed, invalid level parameter: %d.\r\n",
              level);
    return;
  }

  if ((width < 0) || !IsPowerOf2(width)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexSubImage2D() "
              "failed, invalid width parameter: %d.\r\n",
              width);
    return;
  }

  if ((height < 0) || !IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexSubImage2D() "
              "failed, invalid height parameter: %d.\r\n",
              height);
    return;
  }

  ePixelFormat pixelformat;
  GLenum type, _format;

  switch (format) {
  case GL_PALETTE4_RGB8_OES:
    pixelformat = FORMAT_PAL4_B8G8R8;
    type = GL_UNSIGNED_BYTE;
    _format = GL_RGB;
    break;

  case GL_PALETTE4_RGBA8_OES:
    pixelformat = FORMAT_PAL4_A8B8G8R8;
    type = GL_UNSIGNED_BYTE;
    _format = GL_RGBA;
    break;

  case GL_PALETTE4_R5_G6_B5_OES:
    pixelformat = FORMAT_PAL4_R5G6B5;
    type = GL_UNSIGNED_SHORT_5_6_5;
    _format = GL_RGB;
    break;

  case GL_PALETTE4_RGBA4_OES:
    pixelformat = FORMAT_PAL4_R4G4B4A4;
    type = GL_UNSIGNED_SHORT_4_4_4_4;
    _format = GL_RGBA;
    break;

  case GL_PALETTE4_RGB5_A1_OES:
    pixelformat = FORMAT_PAL4_R5G5B5A1;
    type = GL_UNSIGNED_SHORT_5_5_5_1;
    _format = GL_RGBA;
    break;

  case GL_PALETTE8_RGB8_OES:
    pixelformat = FORMAT_PAL8_B8G8R8;
    type = GL_UNSIGNED_BYTE;
    _format = GL_RGB;
    break;

  case GL_PALETTE8_RGBA8_OES:
    pixelformat = FORMAT_PAL8_A8B8G8R8;
    type = GL_UNSIGNED_BYTE;
    _format = GL_RGBA;
    break;

  case GL_PALETTE8_R5_G6_B5_OES:
    pixelformat = FORMAT_PAL8_R5G6B5;
    type = GL_UNSIGNED_SHORT_5_6_5;
    _format = GL_RGB;
    break;

  case GL_PALETTE8_RGBA4_OES:
    pixelformat = FORMAT_PAL8_R4G4B4A4;
    type = GL_UNSIGNED_SHORT_4_4_4_4;
    _format = GL_RGBA;
    break;

  case GL_PALETTE8_RGB5_A1_OES:
    pixelformat = FORMAT_PAL8_R5G5B5A1;
    type = GL_UNSIGNED_SHORT_5_5_5_1;
    _format = GL_RGBA;
    break;

  default:
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexSubImage2D() "
              "failed, invalid format parameter: %d.\r\n",
              format);
    return;
  }

  FormatInfo formatInfo = Format::GetInfo(pixelformat);
  int paletteSize = 1 << formatInfo.PaletteBits;
  int _imageSize = ((width * formatInfo.PaletteBits) + 7) / 8 * height +
                   paletteSize * formatInfo.BytePerPixel;
  if (_imageSize != imageSize) {
    __glError(GL_INVALID_VALUE,
              "GLContext::compressedTexSubImage2D() "
              "failed, invalid imageSize parameter: "
              "%d.\r\n",
              imageSize);
    return;
  }

  GLint minLevel, maxLevel;

  if (level >= 0) {
    minLevel = maxLevel = level;
  } else {
    minLevel = 0;
    maxLevel = -level - 1;
  }

  Surface2D surface;

  for (GLint i = minLevel; i <= maxLevel; ++i) {
    // Initialize the texture surface
    err = surface.initialize(width, height, pixelformat, pData);
    if (__glFailed(err)) {
      __glError(err, "Surface2D::initialize() failed, err = %d.\r\n", err);
      return;
    }

    // Copy the texture sub surface
    this->setTexSubImage2D(target, i, xoffset, yoffset, width, height, _format,
                           type, surface.getBits());

    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }
  }
}
