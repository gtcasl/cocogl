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

void CGLContext::GenTextures(GLsizei n, GLuint *phTextures) {
  GLenum err;

  assert(phTextures);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::GenTextures() failed, invalid n parameter: %d.\r\n"),
        n);
    return;
  }

  for (GLuint *phTex = phTextures, *const phEnd = phTex + n; phTex != phEnd;
       ++phTex) {
    CTexture *pTexture;
    err = CTexture::Create(&pTexture);
    if (__glFailed(err)) {
      __glError(err, _T("CTexture::Create() failed, err = %d.\r\n"), err);
      return;
    }

    uint32_t dwHandle;
    err = GLERROR_FROM_HRESULT(
        handles_->Insert(&dwHandle, pTexture, HANDLE_TEXTURE, this));
    if (__glFailed(err)) {
      __safeRelease(pTexture);
      __glError(err, _T("CHandleTable::Insert() failed, err = %d.\r\n"), err);
      return;
    }

    pTexture->SetHandle(dwHandle);

    *phTex = dwHandle;
  }
}

void CGLContext::BindTexture(GLenum target, GLuint texture) {
  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::BindTexture() failed, invalid ")
              _T("target parameter: %d.\r\n"),
              target);
    return;
  }

  CTexture *pTexture = nullptr;
  if (texture) {
    // First lookup owned textures
    pTexture = reinterpret_cast<CTexture *>(handles_->GetObject(texture, this));
    if ((nullptr == pTexture) && pCtxShared_) {
      // Second lookup the shared context textures
      pTexture = reinterpret_cast<CTexture *>(
          handles_->GetObject(texture, pCtxShared_));
    }

    if (nullptr == pTexture) {
      __glError(GL_INVALID_VALUE,
                _T("CGLContext::BindTexture() failed, ")
                _T("Invalid texture parameter: %d\r\n"),
                texture);
      return;
    }
  } else {
    pTexture = pTexDefault_;
  }

  this->SetTexture(activeTexture_, pTexture);
}

void CGLContext::TexImage2D(GLenum target, GLint level, GLint internalformat,
                            GLsizei width, GLsizei height, GLint border,
                            GLenum format, GLenum type, const GLvoid *pPixels) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TexImage2D() failed, invalid ")
              _T("target parameter: %d.\r\n"),
              target);
    return;
  }

  auto pTexture = this->GetTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level >= MAX_TEXTURE_LEVELS)) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::TexImage2D() failed, invalid level parameter: %d.\r\n"),
        level);
    return;
  }

  if (border != 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexImage2D() failed, invalid ")
              _T("border parameter: %d.\r\n"),
              border);
    return;
  }

  if ((width < 0) || !Math::IsPowerOf2(width)) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::TexImage2D() failed, invalid width parameter: %d.\r\n"),
        width);
    return;
  }

  if ((height < 0) || !Math::IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexImage2D() failed, invalid ")
              _T("height parameter: %d.\r\n"),
              height);
    return;
  }

  if ((GLenum)internalformat != format) {
    __glError(GL_INVALID_OPERATION, _T("CGLContext::TexImage2D() failed, the ")
                                    _T("internalformat and format are ")
                                    _T("different.\r\n"));
    return;
  }

  ePixelFormat dstFormat;
  err = ToPixelFormat(&dstFormat, internalformat);
  if (__glFailed(err)) {
    __glError(err, _T("ToPixelFormat() failed, err = %d.\r\n"), err);
    return;
  }

  // Initialize the texture surface
  err = pTexture->InitializeSurface(level, width, height, dstFormat);
  if (__glFailed(err)) {
    __glError(err, _T("CTexture::InitializeSurface() failed, err = %d.\r\n"),
              err);
    return;
  }

  ePixelFormat srcFormat;
  uint32_t srcBPP;
  err = ToPixelFormat(&srcFormat, &srcBPP, format, type);
  if (__glFailed(err)) {
    __glError(err, _T("ToPixelFormat() failed, err = %d.\r\n"), err);
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
    pTexture->GetSurface(level).GetDesc(&dstDesc);

    err = CopyBuffers(dstDesc, 0, 0, width, height, srcDesc, 0, 0);
    if (__glFailed(err)) {
      __glError(err, _T("CSurface2D::Copy() failed, err = %d.\r\n"), err);
      return;
    }
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->GenerateMipmaps();
    if (__glFailed(err)) {
      __glError(err, _T("CTexture::GenerateMipmaps() failed, err = %d.\r\n"),
                err);
      return;
    }
  }

  pTexture->Invalidate();
}

void CGLContext::TexSubImage2D(GLenum target, GLint level, GLint xoffset,
                               GLint yoffset, GLsizei width, GLsizei height,
                               GLenum format, GLenum type,
                               const GLvoid *pPixels) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TexSubImage2D() failed, ")
              _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  auto pTexture = this->GetTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level >= MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexSubImage2D() failed, ")
              _T("invalid level parameter: %d.\r\n"),
              level);
    return;
  }

  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::TexSubImage2D() failed, ")
                                _T("invalid input dimensions.\r\n"));
    return;
  }

  const CSurface2D &surface2D = pTexture->GetSurface(level);

  if ((xoffset < 0) || (yoffset < 0) ||
      (static_cast<uint32_t>(xoffset + width) > surface2D.GetWidth()) ||
      (static_cast<uint32_t>(yoffset + height) > surface2D.GetHeight())) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::TexSubImage2D() failed, invalid input offsets.\r\n"));
    return;
  }

  ePixelFormat srcFormat;
  uint32_t srcBPP;
  err = ToPixelFormat(&srcFormat, &srcBPP, format, type);
  if (__glFailed(err)) {
    __glError(err, _T("ToPixelFormat() failed, err = %d.\r\n"), err);
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
    surface2D.GetDesc(&dstDesc);

    err = CopyBuffers(dstDesc, xoffset, yoffset, width, height, srcDesc, 0, 0);
    if (__glFailed(err)) {
      __glError(err, _T("CSurface2D::Copy() failed, err = %d.\r\n"), err);
      return;
    }
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->GenerateMipmaps();
    if (__glFailed(err)) {
      __glError(err, _T("CTexture::GenerateMipmaps() failed, err = %d.\r\n"),
                err);
      return;
    }
  }

  pTexture->Invalidate();
}

void CGLContext::CopyTexImage2D(GLenum target, GLint level,
                                GLenum internalformat, GLint x, GLint y,
                                GLsizei width, GLsizei height, GLint border) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TexImage2D() failed, invalid ")
              _T("target parameter: %d.\r\n"),
              target);
    return;
  }

  auto pTexture = this->GetTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level >= MAX_TEXTURE_LEVELS)) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::TexImage2D() failed, invalid level parameter: %d.\r\n"),
        level);
    return;
  }

  if (border != 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexImage2D() failed, invalid ")
              _T("border parameter: %d.\r\n"),
              border);
    return;
  }

  if ((width < 0) || !Math::IsPowerOf2(width)) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::TexImage2D() failed, invalid width parameter: %d.\r\n"),
        width);
    return;
  }

  if ((height < 0) || !Math::IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexImage2D() failed, invalid ")
              _T("height parameter: %d.\r\n"),
              height);
    return;
  }

  ePixelFormat dstFormat;
  err = ToPixelFormat(&dstFormat, internalformat);
  if (__glFailed(err)) {
    __glError(err, _T("ToPixelFormat() failed, err = %d.\r\n"), err);
    return;
  }

  // Initialize the texture surface
  err = pTexture->InitializeSurface(level, width, height, dstFormat);
  if (__glFailed(err)) {
    __glError(err, _T("CTexture::InitializeSurface() failed, err = %d.\r\n"),
              err);
    return;
  }

  GLSurfaceDesc srcDesc;
  pSurfRead_->GetColorDesc(&srcDesc);

  GLSurfaceDesc dstDesc;
  pTexture->GetSurface(level).GetDesc(&dstDesc);

  err = CopyBuffers(dstDesc, 0, 0, width, height, srcDesc, x, y);
  if (__glFailed(err)) {
    __glError(err, _T("CSurface2D::Copy() failed, err = %d.\r\n"), err);
    return;
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->GenerateMipmaps();
    if (__glFailed(err)) {
      __glError(err, _T("CTexture::GenerateMipmaps() failed, err = %d.\r\n"),
                err);
      return;
    }
  }

  pTexture->Invalidate();
}

void CGLContext::CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                                   GLint yoffset, GLint x, GLint y,
                                   GLsizei width, GLsizei height) {
  GLenum err;

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TexSubImage2D() failed, ")
              _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  auto pTexture = this->GetTexture(activeTexture_);
  assert(pTexture);

  if ((level < 0) || (level >= MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexSubImage2D() failed, ")
              _T("invalid level parameter: %d.\r\n"),
              level);
    return;
  }

  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::TexSubImage2D() failed, ")
                                _T("invalid input dimensions.\r\n"));
    return;
  }

  const CSurface2D &surface2D = pTexture->GetSurface(level);

  if ((xoffset < 0) || (yoffset < 0) ||
      (static_cast<uint32_t>(xoffset + width) > surface2D.GetWidth()) ||
      (static_cast<uint32_t>(yoffset + height) > surface2D.GetHeight())) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::TexSubImage2D() failed, invalid input offsets.\r\n"));
    return;
  }

  GLSurfaceDesc srcDesc;
  pSurfRead_->GetColorDesc(&srcDesc);

  GLSurfaceDesc dstDesc;
  surface2D.GetDesc(&dstDesc);

  err = CopyBuffers(dstDesc, xoffset, yoffset, width, height, srcDesc, x, y);
  if (__glFailed(err)) {
    __glError(err, _T("CSurface2D::Copy() failed, err = %d.\r\n"), err);
    return;
  }

  if ((0 == level) && pTexture->bGenMipMaps) {
    err = pTexture->GenerateMipmaps();
    if (__glFailed(err)) {
      __glError(err, _T("CTexture::GenerateMipmaps() failed, err = %d.\r\n"),
                err);
      return;
    }
  }

  pTexture->Invalidate();
}

void CGLContext::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                            GLenum format, GLenum type, GLvoid *pPixels) {
  GLenum err;

  assert(pPixels);

  if ((format != GL_RGBA) && (format != IMPLEMENTATION_COLOR_READ_FORMAT_OES)) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::ReadPixels() failed, invalid ")
              _T("format parameter: %d.\r\n"),
              format);
    return;
  }

  if ((type != GL_UNSIGNED_BYTE) &&
      (type != IMPLEMENTATION_COLOR_READ_TYPE_OES)) {
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::ReadPixels() failed, invalid type parameter: %d.\r\n"),
        type);
    return;
  }

  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::ReadPixels() failed, invalid ")
                                _T("width or height parameters.\r\n"));
    return;
  }

  ePixelFormat dstFormat;
  uint32_t dstBPP;
  err = ToPixelFormat(&dstFormat, &dstBPP, format, type);
  if (__glFailed(err)) {
    __glError(err, _T("ToPixelFormat() failed, err = %d.\r\n"), err);
    return;
  }

  GLSurfaceDesc srcDesc;
  pSurfRead_->GetColorDesc(&srcDesc);

  GLSurfaceDesc dstDesc;
  dstDesc.pBits = reinterpret_cast<uint8_t *>(const_cast<GLvoid *>(pPixels));
  dstDesc.Width = width;
  dstDesc.Height = height;
  dstDesc.Pitch = __align(width * dstBPP, unpackAlignment_);
  dstDesc.Format = static_cast<uint8_t>(dstFormat);

  err = CopyBuffers(dstDesc, 0, 0, width, height, srcDesc, x, y);
  if (__glFailed(err)) {
    __glError(err, _T("CSurface2D::Copy() failed, err = %d.\r\n"), err);
    return;
  }

  /*#ifndef NDEBUG
      static int s_fileno = 0;
      TCHAR szFilePath[MAX_PATH];
      ::GetModuleFileName( nullptr, szFilePath, MAX_PATH );
      PathRemoveFileSpec( szFilePath );
      _sntprintf( szFilePath, MAX_PATH, _T("%s%s%d.bmp"), szFilePath,
  _T("CocoGLESv1_cm"), s_fileno++ );
      __glLog( _T(" - Read Surface bitmap dump: %s.\r\n"), szFilePath );
      pSurfRead_->SaveBitmap( szFilePath );
  #endif*/
}

void CGLContext::PixelStorei(GLenum pname, GLint param) {
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
                _T("CGLContext::PixelStorei() failed, ")
                _T("invalid param parameter: %d.\r\n"),
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
                _T("CGLContext::PixelStorei() failed, ")
                _T("invalid param parameter: %d.\r\n"),
                param);
      return;
    }

    break;

  default:
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::PixelStorei() failed, invalid ")
              _T("pname parameter: %d.\r\n"),
              pname);
    return;
  }
}

void CGLContext::DeleteTextures(GLsizei n, const GLuint *phTextures) {
  assert(phTextures);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::DeleteTextures() failed, invalid n parameter: %d.\r\n"),
        n);
    return;
  }

  for (const GLuint *phTex = phTextures, *const phEnd = phTex + n;
       phTex != phEnd; ++phTex) {
    GLuint handle = *phTex;
    if (handle) {
      auto pTexture =
          reinterpret_cast<CTexture *>(handles_->Delete(handle, this));
      if (pTexture) {
        // Unbind the texture if bound.
        for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
          if (pTexture == this->GetTexture(i)) {
            this->SetTexture(i, pTexDefault_);
          }
        }

        pTexture->Release();
      }
    }
  }
}

GLenum CGLContext::BindTexImage(CGLSurface *pSurface, bool bGenMipMaps) {
  GLenum err;

  assert(pSurface);

  auto pTexture = this->GetTexture(activeTexture_);
  assert(pTexture);

  err = pTexture->BindSurface(pSurface, bGenMipMaps);
  if (__glFailed(err)) {
    __glLogError(_T("CTexture::BindTexImage() failed, err = %d.\r\n"), err);
    return err;
  }

  return GL_NO_ERROR;
}

GLenum CGLContext::ReleaseTexImage(CGLSurface *pSurface) {
  GLenum err;

  auto pTexture = this->GetTexture(activeTexture_);
  assert(pTexture);

  err = pTexture->ReleaseSurface(pSurface);
  if (__glFailed(err)) {
    __glLogError(_T("CTexture::ReleaseTexImage() failed, err = %d.\r\n"), err);
    return err;
  }

  return GL_NO_ERROR;
}

void CGLContext::CompressedTexImage2D(GLenum target, GLint level,
                                      GLenum internalformat, GLsizei width,
                                      GLsizei height, GLint border,
                                      GLsizei imageSize, const GLvoid *pData) {
  GLenum err;

  assert(pData);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  if ((level > 0) || (-level > MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid level parameter: %d.\r\n"),
              level);
    return;
  }

  if (border != 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid border parameter: %d.\r\n"),
              border);
    return;
  }

  if ((width < 0) || !Math::IsPowerOf2(width)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid width parameter: %d.\r\n"),
              width);
    return;
  }

  if ((height < 0) || !Math::IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid height parameter: %d.\r\n"),
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
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid internalformat parameter: ")
              _T("%d.\r\n"),
              internalformat);
    return;
  }

  FormatInfo formatInfo = Format::GetInfo(pixelformat);
  int paletteSize = 1 << formatInfo.PaletteBits;
  int _imageSize = ((width * formatInfo.PaletteBits) + 7) / 8 * height +
                   paletteSize * formatInfo.BytePerPixel;
  if (_imageSize != imageSize) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexImage2D() ")
              _T("failed, invalid imageSize parameter: ")
              _T("%d.\r\n"),
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

  CSurface2D surface;

  for (GLint i = minLevel; i <= maxLevel; ++i) {
    // Initialize the texture surface
    err = surface.Initialize(width, height, pixelformat, pData);
    if (__glFailed(err)) {
      __glError(err, _T("CSurface2D::Initialize() failed, err = %d.\r\n"), err);
      return;
    }

    // Copy the texture surface
    this->TexImage2D(target, i, format, width, height, border, format, type,
                     surface.GetBits());

    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }
  }
}

void CGLContext::CompressedTexSubImage2D(GLenum target, GLint level,
                                         GLint xoffset, GLint yoffset,
                                         GLsizei width, GLsizei height,
                                         GLenum format, GLsizei imageSize,
                                         const GLvoid *pData) {
  GLenum err;

  assert(pData);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexSubImage2D() ")
              _T("failed, invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  if ((level > 0) || (-level > MAX_TEXTURE_LEVELS)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexSubImage2D() ")
              _T("failed, invalid level parameter: %d.\r\n"),
              level);
    return;
  }

  if ((width < 0) || !Math::IsPowerOf2(width)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexSubImage2D() ")
              _T("failed, invalid width parameter: %d.\r\n"),
              width);
    return;
  }

  if ((height < 0) || !Math::IsPowerOf2(height)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexSubImage2D() ")
              _T("failed, invalid height parameter: %d.\r\n"),
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
              _T("CGLContext::CompressedTexSubImage2D() ")
              _T("failed, invalid format parameter: %d.\r\n"),
              format);
    return;
  }

  FormatInfo formatInfo = Format::GetInfo(pixelformat);
  int paletteSize = 1 << formatInfo.PaletteBits;
  int _imageSize = ((width * formatInfo.PaletteBits) + 7) / 8 * height +
                   paletteSize * formatInfo.BytePerPixel;
  if (_imageSize != imageSize) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::CompressedTexSubImage2D() ")
              _T("failed, invalid imageSize parameter: ")
              _T("%d.\r\n"),
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

  CSurface2D surface;

  for (GLint i = minLevel; i <= maxLevel; ++i) {
    // Initialize the texture surface
    err = surface.Initialize(width, height, pixelformat, pData);
    if (__glFailed(err)) {
      __glError(err, _T("CSurface2D::Initialize() failed, err = %d.\r\n"), err);
      return;
    }

    // Copy the texture sub surface
    this->TexSubImage2D(target, i, xoffset, yoffset, width, height, _format,
                        type, surface.GetBits());

    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }
  }
}
