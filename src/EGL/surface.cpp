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

_EGLSurface::_EGLSurface(_EGLDisplay *pDisplay, 
                          EGLint surfaceType,
                         _EGLConfig *pConfig) {
  __profileAPI(" - %s()\n", __FUNCTION__);
  assert(pDisplay);
  assert(pConfig);

  // pDisplay->addRef(); - prevent cyclic reference with display
  pDisplay_ = pDisplay;

  pConfig->addRef();
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

_EGLSurface::~_EGLSurface() {
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
  if (gc_) {
    XFreeGC(pDisplay_->getNativeHandle(), gc_);
  }
  if (pImage_) {
    XDestroyImage(pImage_);
  }
#endif

__safeRelease(pConfig_);
//__safeRelease(pDisplay_);
}

EGLint _EGLSurface::CreateWND(_EGLSurface **ppSurface, 
                              _EGLDisplay *display,
                              EGLint surfaceType, 
                              _EGLConfig *pConfig,
                              EGLNativeWindowType hWnd) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new _EGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError("EGLSurface allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pSurface->addRef();

  // Initialize the surface
  err = pSurface->InitializeWND(hWnd);
  if (__eglFailed(err)) {
    __eglLogError("Surface::initialize() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint _EGLSurface::CreatePXM(_EGLSurface **ppSurface, 
                              _EGLDisplay *display,
                              EGLint surfaceType, 
                              _EGLConfig *pConfig,
                              EGLNativePixmapType hPixmap) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new _EGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError("EGLSurface allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pSurface->addRef();

  // Initialize the surface
  err = pSurface->InitializePXM(hPixmap);
  if (__eglFailed(err)) {
    __eglLogError("Surface::initialize() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint _EGLSurface::CreatePBF(_EGLSurface **ppSurface, 
                              _EGLDisplay *display,
                              EGLint surfaceType, 
                              _EGLConfig *pConfig,
                              EGLint width, 
                              EGLint height,
                              EGLint largestPBuffer, 
                              EGLint texTarget,
                              EGLint texFormat, 
                              EGLint bGenMipMaps) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(ppSurface);

  // Create a new surface object
  auto pSurface = new _EGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError("EGLSurface allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pSurface->addRef();

  // Initialize the surface
  err = pSurface->InitializePBF(width, height, largestPBuffer, texTarget,
                                texFormat, bGenMipMaps);
  if (__eglFailed(err)) {
    __eglLogError("Surface::initialize() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint _EGLSurface::InitializeWND(EGLNativeWindowType hWnd) {
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
  int cBPP = pConfig_->getAttribute(EGL_BUFFER_SIZE);

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
  ::getObject(hBitmap, sizeof(BITMAP), &bitmap);

  // Build the color surface description
  colorDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  colorDesc.Format = Surface::getColorFormat(bitmap.bmBitsPixel);
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

  auto status = XGetGeometry(pDisplay_->getNativeHandle(), hWnd, &root, &x, &y,
                             &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError("::XGetGeometry() failed, status=%d.\r\n", status);
    return EGL_BAD_ALLOC;
  }

  auto image = XGetImage(pDisplay_->getNativeHandle(), hWnd, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError("::XGetImage() failed.\r\n");
    return EGL_BAD_ALLOC;
  }

  gc_ = XCreateGC(pDisplay_->getNativeHandle(), hWnd, 0, nullptr);
  drawable_ = hWnd;
  pImage_ = image;

  colorDesc.Format = _EGLSurface::getColorFormat(image->bits_per_pixel);
  colorDesc.pBits  = reinterpret_cast<uint8_t*>(image->data);
  colorDesc.Width  = image->width;
  colorDesc.Height = image->height;
  colorDesc.Pitch  = image->bytes_per_line;
#endif

  width_ = colorDesc.Width;
  height_ = colorDesc.Height;

  // Build the depth stencil description
  GLSurfaceDesc depthStencilDesc;
  err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                               &depthStencilDesc);
  if (__eglFailed(err)) {
    __eglLogError("_EGLSurface::InitDepthStencil() failed, err = %d.\r\n", err);
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

EGLint _EGLSurface::InitializePXM(EGLNativePixmapType hPixmap) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  GLSurfaceDesc colorDesc;

#if defined(_WIN32)
  // Retrieve the bitmap properties
  BITMAP bitmap;
  if ((0 == ::getObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (nullptr == bitmap.bmBits)) {
    __eglLogError("::getObject() failed, invalid pixmap.\r\n");
    return EGL_BAD_ALLOC;
  }

  // Build the color surface description
  colorDesc.Format = Surface::getColorFormat(bitmap.bmBitsPixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  colorDesc.Pitch = bitmap.bmWidthBytes;
  colorDesc.Width = bitmap.bmWidth;
  colorDesc.Height = bitmap.bmHeight;

  hDC hCompatDC = ::CreateCompatibleDC(pDisplay_ - getNativeHandle());
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

  auto status = XGetGeometry(pDisplay_->getNativeHandle(), hPixmap, &root, &x,
                             &y, &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError("::XGetGeometry() failed, status=%d.\r\n", status);
    return EGL_BAD_ALLOC;
  }

  auto image = XGetImage(pDisplay_->getNativeHandle(), hPixmap, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError("::XGetImage() failed.\r\n");
    return EGL_BAD_ALLOC;
  }

  gc_ = XCreateGC(pDisplay_->getNativeHandle(), hPixmap, 0, nullptr);
  drawable_ = hPixmap;
  pImage_ = image;

  colorDesc.Format = _EGLSurface::getColorFormat(image->bits_per_pixel);
  colorDesc.pBits  = reinterpret_cast<uint8_t *>(image->data);
  colorDesc.Width  = image->width;
  colorDesc.Height = image->height;
  colorDesc.Pitch  = image->bytes_per_line;

  memset(image->data, 0, image->bytes_per_line * image->height);
#endif

  // Check if the config matches the window bpp
  auto cBPP = Format::GetInfo((ePixelFormat)colorDesc.Format).BytePerPixel * 8;
  if (cBPP != pConfig_->getAttribute(EGL_BUFFER_SIZE)) {
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
    __eglLogError("_EGLSurface::InitDepthStencil() failed, err = %d.\r\n", err);
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

EGLint _EGLSurface::InitializePBF(EGLint width, 
                                  EGLint height,
                                  EGLint largestPBuffer, 
                                  EGLint texTarget,
                                  EGLint texFormat, 
                                  EGLint bGenMipMaps) {
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
          (!IsPowerOf2(width))) {
        __eglLogError("Surface::initialize() failed, invalid width "
                      "parameter: %d.\r\n",
                      width);
        return EGL_BAD_ATTRIBUTE;
      }

      if ((height < 0) || (height > MAX_PBUFFER_HEIGHT) ||
          !IsPowerOf2(height)) {
        __eglLogError("Surface::initialize() failed, invalid height "
                      "parameter: %d.\r\n",
                      height);
        return EGL_BAD_ATTRIBUTE;
      }

      if (bGenMipMaps) {
        mipLevels_ = std::max(iLog2(width), iLog2(height));
      }
    }

    // Allocate mipmap array
    ppBuffers_ = new uint8_t *[mipLevels_];
    if (nullptr == ppBuffers_) {
      __eglLogError("Surface::initialize() failed, out of memory.\r\n");
      return EGL_BAD_ALLOC;
    }

    uint32_t nBPP = pConfig_->getAttribute(EGL_BUFFER_SIZE);
    uint32_t mipLevels = mipLevels_;

    for (uint32_t i = 0, _width = width, _height = height; i < mipLevels; ++i) {
      // Allocate mipmap level
      ppBuffers_[i] = new uint8_t[_width * _height * (nBPP / 8)];
      if (nullptr == ppBuffers_[i]) {
        __eglLogError("Surface::initialize() failed, out of memory.\r\n");
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
    colorDesc.Format = _EGLSurface::getColorFormat(nBPP);
    colorDesc.Pitch = width * (nBPP) / 8;
    colorDesc.Width = width;
    colorDesc.Height = height;

    // Build the depth stencil description
    GLSurfaceDesc depthStencilDesc;
    err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                                 &depthStencilDesc);
    if (__eglFailed(err)) {
      __eglLogError("_EGLSurface::InitDepthStencil() failed, err = %d.\r\n",
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
    err = EGLERROR_FROM_HRESULT(__glCreateSurface(nullptr, nullptr, &glSurface_));
    if (__eglFailed(err)) {
      __eglLogError("__glCreateSurface() failed, err = %d.\r\n", err);
      return err;
    }
  }

  return EGL_SUCCESS;
}

EGLint _EGLSurface::InitDepthStencil(uint32_t width, uint32_t height,
                                     GLSurfaceDesc *pSurfaceDesc) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(pSurfaceDesc);

  pSurfaceDesc->Width = width;
  pSurfaceDesc->Height = height;

  auto stride = (pConfig_->getAttribute(EGL_DEPTH_SIZE) +
                 pConfig_->getAttribute(EGL_STENCIL_SIZE)) >>  3;
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
    pDepthStencilBits_ = new uint8_t[stride * pSurfaceDesc->Width * pSurfaceDesc->Height];
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

EGLint _EGLSurface::copyBuffer(EGLNativePixmapType hPixmap) {
  EGLint err;

  GLSurfaceDesc srcDesc;

  if (ppBuffers_) {
    auto mipLevel = std::clamp<uint32_t>(mipLevel_, 0, mipLevels_ - 1);
    uint32_t nBPP = pConfig_->getAttribute(EGL_BUFFER_SIZE);
    srcDesc.pBits = ppBuffers_[mipLevel];
    srcDesc.Format = _EGLSurface::getColorFormat(nBPP);
    srcDesc.Pitch = width_ * (nBPP / 8);
    srcDesc.Width = width_;
    srcDesc.Height = height_;
  } else {
#if defined(_WIN32)
    // Retrieve the bitmap properties
    BITMAP bitmap;
    if ((0 == ::getObject(hBitmap_, sizeof(BITMAP), &bitmap)) ||
        (nullptr == bitmap.bmBits)) {
      __eglLogError("::getObject() failed, invalid pixmap.\r\n");
      return EGL_BAD_ALLOC;
    }

    // Build the color surface description
    srcDesc.Format = Surface::getColorFormat(bitmap.bmBitsPixel);
    srcDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
    srcDesc.Pitch = bitmap.bmWidthBytes;
    srcDesc.Width = bitmap.bmWidth;
    srcDesc.Height = bitmap.bmHeight;
#elif defined(__linux__)
    srcDesc.Format = _EGLSurface::getColorFormat(pImage_->bits_per_pixel);
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
  if ((0 == ::getObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (nullptr == bitmap.bmBits)) {
    __eglLogError("::getObject() failed, invalid pixmap.\r\n");
    return EGL_BAD_ALLOC;
  }

  // Build the color surface description
  dstDesc.Format = Surface::getColorFormat(bitmap.bmBitsPixel);
  dstDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  dstDesc.Pitch = bitmap.bmWidthBytes;
  dstDesc.Width = bitmap.bmWidth;
  dstDesc.Height = bitmap.bmHeight;
#elif defined(__linux__)
  Window root;
  int32_t x, y;
  uint32_t width, height;
  uint32_t border, depth;
  auto status = XGetGeometry(pDisplay_->getNativeHandle(), hPixmap, &root, &x,
                             &y, &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError("::XGetGeometry() failed, status=%d.\r\n", status);
    return EGL_BAD_ALLOC;
  }
  auto image = XGetImage(pDisplay_->getNativeHandle(), hPixmap, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError("::XGetImage() failed.\r\n");
    return EGL_BAD_ALLOC;
  }
  scope_exit auto_destroy([&]() { XDestroyImage(image); });
  dstDesc.Format = _EGLSurface::getColorFormat(image->bits_per_pixel);
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

void _EGLSurface::present() {
#if defined(_WIN32)
  BitBlt(pDisplay_->getDgetNativeHandleC(), 0, 0, width_, height_, hDC_, 0, 0, SRCCOPY);
#elif defined(__linux__)
  XPutImage(pDisplay_->getNativeHandle(), drawable_, gc_, pImage_, 0, 0, 0, 0, width_, height_);
#endif
}

EGLint _EGLSurface::bindTexture() {
  EGLint err;

  // Check if we have a texture bindable surface
  if ((EGL_PBUFFER_BIT != surfaceType_) || (EGL_TEXTURE_2D != texTarget_)) {
    __eglLogError("Surface::bindTexture() failed, the surface is not a "
                  "pbuffer supporting texture binding.\r\n");
    return EGL_BAD_SURFACE;
  }

  if (bBoundTexture_) {
    __eglLogError("Surface::bindTexture() failed, the buffer is "
                  "already bound to a texture.\r\n");
    return EGL_BAD_ACCESS;
  }

  if (EGL_NO_TEXTURE == texFormat_) {
    __eglLogError("Surface::bindTexture() failed, the surface attribute "
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

EGLint _EGLSurface::releaseTexBound() {
  // Check if we have a texture bindable surface
  if ((EGL_PBUFFER_BIT != surfaceType_) || (EGL_TEXTURE_2D != texTarget_)) {
    __eglLogError(
        "Surface::releaseTexBound() failed, the surface is not an EGL "
        "surface, or is not a bound pbuffer surface.\r\n");
    return EGL_BAD_SURFACE;
  }

  // Verify the texture format
  if (EGL_NO_TEXTURE == texFormat_) {
    __eglLogError("Surface::releaseTexBound() failed, the surface attribute "
                  "EGL_TEXTURE_FORMAT is set to EGL_NO_TEXTURE.\r\n");
    return EGL_BAD_MATCH;
  }

  if (bBoundTexture_) {
    __glReleaseTexImage(glSurface_);
    bBoundTexture_ = false;
  }

  return EGL_SUCCESS;
}

EGLint _EGLSurface::setAttribute(EGLint attribute, EGLint value) {
  EGLint err;

  switch (attribute) {
  case EGL_MIPMAP_LEVEL: {
    mipLevel_ = value;

    if (ppBuffers_) {
      GLSurfaceDesc surfDesc;
      this->getPBufferDesc(&surfDesc);

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
    __eglLogError("Surface::setAttribute() failed, invalid attribute: %d.\r\n", attribute);
    return EGL_BAD_ATTRIBUTE;
  }

  return EGL_SUCCESS;
}

EGLint _EGLSurface::getAttribute(EGLint *pValue, EGLint attribute) const {
  EGLint value;

  switch (attribute) {
  case EGL_CONFIG_ID:
    value = pConfig_->getAttribute(EGL_CONFIG_ID);
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
    __eglLogError("Surface::getAttribute() failed, invalid attribute: %d.\r\n", attribute);
    return EGL_BAD_ATTRIBUTE;
  }

  if (pValue) {
    *pValue = value;
  }

  return EGL_SUCCESS;
}

uint8_t _EGLSurface::getColorFormat(uint32_t cBitsPerPixel) {
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

void _EGLSurface::getPBufferDesc(GLSurfaceDesc *pSurfaceDesc) {
  assert(pSurfaceDesc);

  auto mipLevel = std::clamp<int32_t>(mipLevel_, 0, mipLevels_ - 1);
  auto width = width_;
  auto height = height_;

  for (int32_t i = 0; i < mipLevel; ++i) {
    if (width > 1) {
      width >>= 1;
    }
    if (height > 1) {
      height >>= 1;
    }
  }

  auto nBPP = pConfig_->getAttribute(EGL_BUFFER_SIZE);

  pSurfaceDesc->Format = _EGLSurface::getColorFormat(nBPP);
  pSurfaceDesc->pBits  = ppBuffers_[mipLevel];
  pSurfaceDesc->Width  = width;
  pSurfaceDesc->Height = height;
  pSurfaceDesc->Pitch  = width * (nBPP / 8);
}

HRESULT _EGLSurface::saveImage(const char *filename) {
  return __glSaveImage(glSurface_, filename);
}