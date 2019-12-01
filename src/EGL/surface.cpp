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
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#include "stdafx.h"
#include "surface.hpp"
#include "config.hpp"
#include "display.hpp"

CEGLSurface::CEGLSurface(CDisplay *pDisplay, EGLint surfaceType,
                         CConfig *pConfig) {
  __profileAPI(" - %s()\n", __FUNCTION__);
  assert(pDisplay);
  assert(pConfig);

  // pDisplay->AddRef(); - prevent cyclic reference with display
  pDisplay_ = pDisplay;

  pConfig->AddRef();
  pConfig_ = pConfig;

  surfaceType_ = surfaceType;

  width_ = 0;
  height_ = 0;
  largestPBuffer_ = EGL_FALSE;
  texFormat_ = EGL_NO_TEXTURE;
  texTarget_ = EGL_NO_TEXTURE;
  mipTexture_ = EGL_FALSE;
  mipLevel_ = 0;
  bBoundTexture_ = false;

#if defined(_WIN32)
  hDC_ = nullptr;
  hBitmap_ = nullptr;
  bool bExternalBitmap_ = false;
#elif defined(__linux__)
  pImage_ = nullptr;
  drawable_ = 0;
  gc_ = 0;
#endif

  pDepthStencilBits_ = nullptr;
  ppBuffers_ = nullptr;
  mipLevels_ = 0;
  glSurface_ = nullptr;
}

CEGLSurface::~CEGLSurface() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeDeleteArray(pDepthStencilBits_);

  if (ppBuffers_) {
    for (uint32_t i = 0; i < mipLevels_; ++i) {
      delete[] ppBuffers_[i];
    }

    delete[] ppBuffers_;
  }

  if (glSurface_) {
    __glDestroySurface(glSurface_);
  }

#if defined(_WIN32)
  if (hDC_) {
    DeleteDC(hDC_);
  }
  if (hBitmap_ && bExternalBitmap_)
    DeletObject(hBitmap_);
}
#elif defined(__linux__)
  if (pImage_) {
    XDestroyImage(pImage_);
  }
  if (gc_) {
    XFreeGC(pDisplay_->GetNativeHandle(), gc_);
  }
#endif

__safeRelease(pConfig_);
}

EGLint CEGLSurface::CreateWND(CEGLSurface **ppSurface, CDisplay *display,
                              EGLint surfaceType, CConfig *pConfig,
                              EGLNativeWindowType hWnd) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new CEGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError("CEGLSurface allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->InitializeWND(hWnd);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglLogError("CEGLSurface::Initialize() failed, err = %d.\r\n", err);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::CreatePXM(CEGLSurface **ppSurface, CDisplay *display,
                              EGLint surfaceType, CConfig *pConfig,
                              EGLNativePixmapType hPixmap) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new CEGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError("CEGLSurface allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->InitializePXM(hPixmap);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglLogError("CEGLSurface::Initialize() failed, err = %d.\r\n", err);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::CreatePBF(CEGLSurface **ppSurface, CDisplay *display,
                              EGLint surfaceType, CConfig *pConfig,
                              EGLint width, EGLint height,
                              EGLint largestPBuffer, EGLint texTarget,
                              EGLint texFormat, EGLint bGenMipMaps) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new CEGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError("CEGLSurface allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->InitializePBF(width, height, largestPBuffer, texTarget,
                                texFormat, bGenMipMaps);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglLogError("CEGLSurface::Initialize() failed, err = %d.\r\n", err);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::InitializeWND(EGLNativeWindowType hWnd) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  GLSurfaceDesc colorDesc;

#if defined(_WIN32)
  // Query the window dimensions
  Rect rect;
  if (!::GetClientRect(hWnd, &rect)) {
    __eglLogError("::GetClientRect() failed, hr = %x.\r\n",
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  width_ = rect.right - rect.left;
  height_ = rect.bottom - rect.top;

  // Retrieve the window device context
  HDC hDC = ::GetWindowDC(hWnd);
  hDC hCompatDC = ::CreateCompatibleDC(hDC);
  if (nullptr == hCompatDC) {
    ::ReleaseDC(hWnd, hDC);
    __eglLogError("::CreateCompatibleDC() failed, hr = %x.\r\n",
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  // Release the device context
  ::ReleaseDC(hWnd, hDC);

  // Retrieve the config bits per pixels
  int cBPP = pConfig_->GetAttribute(EGL_BUFFER_SIZE);

  struct BitmapInfo {
    BITMAPINFOHEADER bmiHeader;
    uint32_t bmiColors[3];
  } bmpInfo;

  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = width_;
  bmpInfo.bmiHeader.biHeight = height_;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biBitCount = static_cast<uint16_t>(cBPP);
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage = width_ * height_ * (cBPP >> 3);
  bmpInfo.bmiHeader.biXPelsPerMeter = 0;
  bmpInfo.bmiHeader.biYPelsPerMeter = 0;
  bmpInfo.bmiHeader.biClrUsed = 0;
  bmpInfo.bmiHeader.biClrImportant = 0;

  if (16 == cBPP) {
    bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;
    bmpInfo.bmiColors[0] = 0xF800;
    bmpInfo.bmiColors[1] = 0x07E0;
    bmpInfo.bmiColors[2] = 0x001F;
  }

  // Create the font DIB bitmap
  void *pBits;
  auto hBitmap =
      ::CreateDIBSection(hCompactDC, reinterpret_cast<BITMAPINFO *>(&bmpInfo),
                         DIB_RGB_COLORS, &pBits, nullptr, 0);
  if (nullptr == hBitmap) {
    ::DeleteDC(hCompactDC);
    __eglLogError("::CreateDIBSection() failed, hr = %x.\r\n",
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  // Assign the bitmap to the DC
  ::SelectObject(hCompatDC, hBitmap);

  // Retrieve the BITMAP properties
  BITMAP bitmap;
  ::GetObject(hBitmap, sizeof(BITMAP), &bitmap);

  // Build the color surface description
  colorDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  colorDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
  colorDesc.Pitch = bitmap.bmWidthBytes;
  colorDesc.Width = bitmap.bmWidth;
  colorDesc.Height = bitmap.bmHeight;

  hBitmap_ = hBitmap;
  hDC_ = hCompatDC;
  bExternalBitmap_ = true;
#elif defined(__linux__)
  Window root;
  int32_t x, y;
  uint32_t width, height;
  uint32_t border, depth;

  auto status = XGetGeometry(pDisplay_->GetNativeHandle(), hWnd, &root, &x, &y,
                             &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError("::XGetGeometry() failed, status=%d.\r\n", status);
    return EGL_BAD_ALLOC;
  }

  auto image = XGetImage(pDisplay_->GetNativeHandle(), hWnd, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError("::XGetImage() failed.\r\n");
    return EGL_BAD_ALLOC;
  }

  gc_ = XCreateGC(pDisplay_->GetNativeHandle(), hWnd, 0, nullptr);
  drawable_ = hWnd;
  pImage_ = image;

  colorDesc.Format = CEGLSurface::GetColorFormat(image->bits_per_pixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(image->data) +
                    image->bytes_per_line * (image->height - 1);
  colorDesc.Width = image->width;
  colorDesc.Height = image->height;
  colorDesc.Pitch = -image->bytes_per_line;
#endif

  width_ = colorDesc.Width;
  height_ = colorDesc.Height;

  // Build the depth stencil description
  GLSurfaceDesc depthStencilDesc;
  err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                               &depthStencilDesc);
  if (__eglFailed(err)) {
    __eglLogError("CEGLSurface::InitDepthStencil() failed, err = %d.\r\n",
                  err);
    return err;
  }

  // Create the GL surface
  err = EGLERROR_FROM_HRESULT(
      __glCreateSurface(&colorDesc, &depthStencilDesc, &glSurface_));
  if (__eglFailed(err)) {
    __eglLogError("__glCreateSurface() failed, err = %d.\r\n", err);
    return err;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::InitializePXM(EGLNativePixmapType hPixmap) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  GLSurfaceDesc colorDesc;

#if defined(_WIN32)
  // Retrieve the bitmap properties
  BITMAP bitmap;
  if ((0 == ::GetObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (nullptr == bitmap.bmBits)) {
    __eglLogError("::GetObject() failed, invalid pixmap.\r\n");
    return EGL_BAD_ALLOC;
  }

  // Build the color surface description
  colorDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  colorDesc.Pitch = bitmap.bmWidthBytes;
  colorDesc.Width = bitmap.bmWidth;
  colorDesc.Height = bitmap.bmHeight;

  hDC hCompatDC = ::CreateCompatibleDC(pDisplay_ - GetNativeHandle());
  if (nullptr == hCompatDC) {
    __eglLogError("::CreateCompatibleDC() failed, hr = %x.\r\n",
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  ::SelectObject(hCompatDC, hPixmap);
  hDC_ = hCompatDC;
  hBitmap_ = hPixmap;
#elif defined(__linux__)
  Window root;
  int32_t x, y;
  uint32_t width, height;
  uint32_t border, depth;

  auto status = XGetGeometry(pDisplay_->GetNativeHandle(), hPixmap, &root, &x,
                             &y, &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError("::XGetGeometry() failed, status=%d.\r\n", status);
    return EGL_BAD_ALLOC;
  }

  auto image = XGetImage(pDisplay_->GetNativeHandle(), hPixmap, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError("::XGetImage() failed.\r\n");
    return EGL_BAD_ALLOC;
  }

  gc_ = XCreateGC(pDisplay_->GetNativeHandle(), hPixmap, 0, nullptr);
  drawable_ = hPixmap;
  pImage_ = image;

  colorDesc.Format = CEGLSurface::GetColorFormat(image->bits_per_pixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(image->data) +
                    image->bytes_per_line * (image->height - 1);
  ;
  colorDesc.Width = image->width;
  colorDesc.Height = image->height;
  colorDesc.Pitch = -image->bytes_per_line;
#endif

  // Check if the config matches the window bpp
  auto cBPP = Format::GetInfo(colorDesc.Format).BytePerPixel * 8;
  if (cBPP != pConfig_->GetAttribute(EGL_BUFFER_SIZE)) {
    __eglLogError("The config's buffer size doesn't match the window's "
                  "bits per pixel.\r\n");
    return EGL_BAD_MATCH;
  }

  width_ = colorDesc.Width;
  height_ = colorDesc.Height;

  // Build the depth stencil description
  GLSurfaceDesc depthStencilDesc;
  err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                               &depthStencilDesc);
  if (__eglFailed(err)) {
    __eglLogError("CEGLSurface::InitDepthStencil() failed, err = %d.\r\n",
                  err);
    return err;
  }

  // Create the GL surface
  err = EGLERROR_FROM_HRESULT(
      __glCreateSurface(&colorDesc, &depthStencilDesc, &glSurface_));
  if (__eglFailed(err)) {
    __eglLogError("__glCreateSurface() failed, err = %d.\r\n", err);
    return err;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::InitializePBF(EGLint width, EGLint height,
                                  EGLint largestPBuffer, EGLint texTarget,
                                  EGLint texFormat, EGLint bGenMipMaps) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  width_ = width;
  height_ = height;
  texTarget_ = texTarget;
  texFormat_ = texFormat;
  largestPBuffer_ = largestPBuffer; // Not applicable for software renderers
  mipLevels_ = 0;

  if (width && height) {
    mipLevels_ = 1;

    if (EGL_TEXTURE_2D == texTarget) {
      if ((width < 0) || (width > MAX_PBUFFER_WIDTH) ||
          (!Math::IsPowerOf2(width))) {
        __eglLogError("CEGLSurface::Initialize() failed, invalid width "
                      "parameter: %d.\r\n",
                      width);
        return EGL_BAD_ATTRIBUTE;
      }

      if ((height < 0) || (height > MAX_PBUFFER_HEIGHT) ||
          !Math::IsPowerOf2(height)) {
        __eglLogError("CEGLSurface::Initialize() failed, invalid height "
                      "parameter: %d.\r\n",
                      height);
        return EGL_BAD_ATTRIBUTE;
      }

      if (bGenMipMaps) {
        mipLevels_ = Math::TMax(Math::iLog2(width), Math::iLog2(height));
      }
    }

    // Allocate mipmap array
    ppBuffers_ = new uint8_t *[mipLevels_];
    if (nullptr == ppBuffers_) {
      __eglLogError("CEGLSurface::Initialize() failed, out of memory.\r\n");
      return EGL_BAD_ALLOC;
    }

    uint32_t nBPP = pConfig_->GetAttribute(EGL_BUFFER_SIZE);
    uint32_t mipLevels = mipLevels_;

    for (uint32_t i = 0, _width = width, _height = height; i < mipLevels; ++i) {
      // Allocate mipmap level
      ppBuffers_[i] = new uint8_t[_width * _height * (nBPP / 8)];
      if (nullptr == ppBuffers_[i]) {
        __eglLogError(
            "CEGLSurface::Initialize() failed, out of memory.\r\n");
        return EGL_BAD_ALLOC;
      }

      if (_width > 1) {
        _width >>= 1;
      }

      if (_height > 1) {
        _height >>= 1;
      }
    }

    // Build the color surface description
    GLSurfaceDesc colorDesc;
    colorDesc.pBits = reinterpret_cast<uint8_t *>(ppBuffers_[0]);
    colorDesc.Format = CEGLSurface::GetColorFormat(nBPP);
    colorDesc.Pitch = width * (nBPP) / 8;
    colorDesc.Width = width;
    colorDesc.Height = height;

    // Build the depth stencil description
    GLSurfaceDesc depthStencilDesc;
    err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                                 &depthStencilDesc);
    if (__eglFailed(err)) {
      __eglLogError("CEGLSurface::InitDepthStencil() failed, err = %d.\r\n",
                    err);
      return err;
    }

    // Create the GL surface
    err = EGLERROR_FROM_HRESULT(
        __glCreateSurface(&colorDesc, &depthStencilDesc, &glSurface_));
    if (__eglFailed(err)) {
      __eglLogError("__glCreateSurface() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    // Create the GL surface
    err =
        EGLERROR_FROM_HRESULT(__glCreateSurface(nullptr, nullptr, &glSurface_));
    if (__eglFailed(err)) {
      __eglLogError("__glCreateSurface() failed, err = %d.\r\n", err);
      return err;
    }
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::BindTexture() {
  EGLint err;

  // Check if we have a texture bindable surface
  if ((EGL_PBUFFER_BIT != surfaceType_) || (EGL_TEXTURE_2D != texTarget_)) {
    __eglLogError("CEGLSurface::BindTexture() failed, the surface is not a "
                  "pbuffer supporting texture binding.\r\n");
    return EGL_BAD_SURFACE;
  }

  if (bBoundTexture_) {
    __eglLogError("CEGLSurface::BindTexture() failed, the buffer is "
                  "already bound to a texture.\r\n");
    return EGL_BAD_ACCESS;
  }

  if (EGL_NO_TEXTURE == texFormat_) {
    __eglLogError(
        "CEGLSurface::BindTexture() failed, the surface attribute "
        "EGL_TEXTURE_FORMAT is set to EGL_NO_TEXTURE.\r\n");
    return EGL_BAD_MATCH;
  }

  bool bGenMipmaps = (0 == mipLevel_) && (mipLevels_ > 1);

  err = EGLERROR_FROM_HRESULT(__glBindTexImage(glSurface_, bGenMipmaps));
  if (__eglFailed(err)) {
    __eglLogError("__glBindTexImage() failed, err = %d.\r\n", err);
    return err;
  }

  bBoundTexture_ = true;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::ReleaseTexBound() {
  // Check if we have a texture bindable surface
  if ((EGL_PBUFFER_BIT != surfaceType_) || (EGL_TEXTURE_2D != texTarget_)) {
    __eglLogError(
        "CEGLSurface::ReleaseTexBound() failed, the surface is not an EGL "
        "surface, or is not a bound pbuffer surface.\r\n");
    return EGL_BAD_SURFACE;
  }

  // Verify the texture format
  if (EGL_NO_TEXTURE == texFormat_) {
    __eglLogError(
        "CEGLSurface::ReleaseTexBound() failed, the surface attribute "
        "EGL_TEXTURE_FORMAT is set to EGL_NO_TEXTURE.\r\n");
    return EGL_BAD_MATCH;
  }

  if (bBoundTexture_) {
    __glReleaseTexImage(glSurface_);
    bBoundTexture_ = false;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::GetAttribute(EGLint *pValue, EGLint attribute) const {
  EGLint value;

  switch (attribute) {
  case EGL_CONFIG_ID:
    value = pConfig_->GetAttribute(EGL_CONFIG_ID);
    break;

  case EGL_HEIGHT:
    value = height_;
    break;

  case EGL_WIDTH:
    value = width_;
    break;

  case EGL_LARGEST_PBUFFER:
    value = largestPBuffer_;
    break;

  case EGL_TEXTURE_FORMAT:
    value = texFormat_;
    break;

  case EGL_MIPMAP_TEXTURE:
    value = mipTexture_;
    break;

  case EGL_MIPMAP_LEVEL:
    value = mipLevel_;
    break;

  default:
    __eglLogError(
        "CEGLSurface::GetAttribute() failed, invalid attribute: %d.\r\n",
        attribute);
    return EGL_BAD_ATTRIBUTE;
  }

  if (pValue) {
    *pValue = value;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::SetAttribute(EGLint attribute, EGLint value) {
  EGLint err;

  switch (attribute) {
  case EGL_MIPMAP_LEVEL: {
    mipLevel_ = value;

    if (ppBuffers_) {
      GLSurfaceDesc surfDesc;
      this->GetPBufferDesc(&surfDesc);

      // Update the GL surface
      err = EGLERROR_FROM_HRESULT(
          __glUpdateSurface(glSurface_, &surfDesc, nullptr));
      if (__eglFailed(err)) {
        __eglLogError("__glCreateSurface() failed, err = %d.\r\n", err);
        return err;
      }
    }
  }

  break;

  default:
    __eglLogError(
        "CEGLSurface::SetAttribute() failed, invalid attribute: %d.\r\n",
        attribute);
    return EGL_BAD_ATTRIBUTE;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::CopyBuffer(EGLNativePixmapType hPixmap) {
  EGLint err;

  GLSurfaceDesc srcDesc;

  if (ppBuffers_) {
    auto mipLevel = Math::TClamp<uint32_t>(mipLevel_, 0, mipLevels_ - 1);
    uint32_t nBPP = pConfig_->GetAttribute(EGL_BUFFER_SIZE);
    srcDesc.pBits = ppBuffers_[mipLevel];
    srcDesc.Format = CEGLSurface::GetColorFormat(nBPP);
    srcDesc.Pitch = width_ * (nBPP / 8);
    srcDesc.Width = width_;
    srcDesc.Height = height_;
  } else {
#if defined(_WIN32)
    // Retrieve the bitmap properties
    BITMAP bitmap;
    if ((0 == ::GetObject(hBitmap_, sizeof(BITMAP), &bitmap)) ||
        (nullptr == bitmap.bmBits)) {
      __eglLogError("::GetObject() failed, invalid pixmap.\r\n");
      return EGL_BAD_ALLOC;
    }

    // Build the color surface description
    srcDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
    srcDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
    srcDesc.Pitch = bitmap.bmWidthBytes;
    srcDesc.Width = bitmap.bmWidth;
    srcDesc.Height = bitmap.bmHeight;
#elif defined(__linux__)
    srcDesc.Format = CEGLSurface::GetColorFormat(pImage_->bits_per_pixel);
    srcDesc.pBits = reinterpret_cast<uint8_t *>(pImage_->data);
    srcDesc.Width = pImage_->width;
    srcDesc.Height = pImage_->height;
    srcDesc.Pitch = pImage_->bytes_per_line;
#endif
  }

  GLSurfaceDesc dstDesc;

#if defined(_WIN32)
  // Retrieve the bitmap properties
  BITMAP bitmap;
  if ((0 == ::GetObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (nullptr == bitmap.bmBits)) {
    __eglLogError("::GetObject() failed, invalid pixmap.\r\n");
    return EGL_BAD_ALLOC;
  }

  // Build the color surface description
  dstDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
  dstDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  dstDesc.Pitch = bitmap.bmWidthBytes;
  dstDesc.Width = bitmap.bmWidth;
  dstDesc.Height = bitmap.bmHeight;
#elif defined(__linux__)
  Window root;
  int32_t x, y;
  uint32_t width, height;
  uint32_t border, depth;
  auto status = XGetGeometry(pDisplay_->GetNativeHandle(), hPixmap, &root, &x,
                             &y, &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError("::XGetGeometry() failed, status=%d.\r\n", status);
    return EGL_BAD_ALLOC;
  }
  auto image = XGetImage(pDisplay_->GetNativeHandle(), hPixmap, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError("::XGetImage() failed.\r\n");
    return EGL_BAD_ALLOC;
  }
  scope_exit auto_destroy([&]() { XDestroyImage(image); });
  dstDesc.Format = CEGLSurface::GetColorFormat(image->bits_per_pixel);
  dstDesc.pBits = reinterpret_cast<uint8_t *>(image->data);
  dstDesc.Width = image->width;
  dstDesc.Height = image->height;
  dstDesc.Pitch = image->bytes_per_line;
#endif

  // Copy buffers using GL
  err = EGLERROR_FROM_HRESULT(__glCopyBuffers(&dstDesc, &srcDesc));
  if (__eglFailed(err)) {
    __eglLogError("__glCopySurface() failed, err = %d.\r\n", err);
    return err;
  }

  return EGL_SUCCESS;
}

void CEGLSurface::Present() {
#if defined(_WIN32)
  ::BitBlt(pDisplay_->GetDGetNativeHandleC(), 0, 0, width_, height_, hDC_, 0, 0,
           SRCCOPY);
#elif defined(__linux__)
  XPutImage(pDisplay_->GetNativeHandle(), drawable_, gc_, pImage_, 0, 0, 0, 0,
            width_, height_);
#endif
}

HRESULT CEGLSurface::SaveBitmap(const char *filename) {
  return __glSaveBitmap(glSurface_, filename);
}

uint8_t CEGLSurface::GetColorFormat(uint32_t cBitsPerPixel) {
  switch (cBitsPerPixel) {
  case 16:
    return FORMAT_R5G6B5;
  case 24:
    return FORMAT_R8G8B8;
  case 32:
    return FORMAT_A8R8G8B8;
  }
  return FORMAT_UNKNOWN;
}

EGLint CEGLSurface::InitDepthStencil(uint32_t width, uint32_t height,
                                     GLSurfaceDesc *pSurfaceDesc) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(pSurfaceDesc);

  pSurfaceDesc->Width = width;
  pSurfaceDesc->Height = height;

  uint32_t stride = (pConfig_->GetAttribute(EGL_DEPTH_SIZE) +
                     pConfig_->GetAttribute(EGL_STENCIL_SIZE)) >>
                    3;
  switch (stride) {
  case 3:
    pSurfaceDesc->Format = FORMAT_X8S8D16;
    stride = 4;
    break;
  case 2:
    pSurfaceDesc->Format = FORMAT_D16;
    break;
  default:
    pSurfaceDesc->Format = FORMAT_UNKNOWN;
    break;
  }

  if (pSurfaceDesc->Format != FORMAT_UNKNOWN) {
    pDepthStencilBits_ =
        new uint8_t[stride * pSurfaceDesc->Width * pSurfaceDesc->Height];
    if (nullptr == pDepthStencilBits_) {
      __eglLogError(
          "Depth stencil buffer allocation failed, out of memory.\r\n");
      return EGL_BAD_ALLOC;
    }

    pSurfaceDesc->pBits = pDepthStencilBits_;
  } else {
    pSurfaceDesc->pBits = nullptr;
  }

  pSurfaceDesc->Pitch = stride * pSurfaceDesc->Width;

  return EGL_SUCCESS;
}

void CEGLSurface::GetPBufferDesc(GLSurfaceDesc *pSurfaceDesc) {
  assert(pSurfaceDesc);

  auto mipLevel = Math::TClamp<uint32_t>(mipLevel_, 0, mipLevels_ - 1);
  uint32_t width = width_;
  uint32_t height = height_;

  for (uint32_t i = 0; i < mipLevel; ++i) {
    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }
  }

  uint32_t nBPP = pConfig_->GetAttribute(EGL_BUFFER_SIZE);

  pSurfaceDesc->Format = CEGLSurface::GetColorFormat(nBPP);
  pSurfaceDesc->pBits = ppBuffers_[mipLevel];
  pSurfaceDesc->Width = width;
  pSurfaceDesc->Height = height;
  pSurfaceDesc->Pitch = width * (nBPP / 8);
}