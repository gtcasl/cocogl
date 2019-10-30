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
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
  ASSERT(pDisplay);
  ASSERT(pConfig);

  // pDisplay->AddRef(); - prevent cyclic reference with display
  m_pDisplay = pDisplay;

  pConfig->AddRef();
  m_pConfig = pConfig;

  m_surfaceType = surfaceType;

  m_width = 0;
  m_height = 0;
  m_largestPBuffer = EGL_FALSE;
  m_texFormat = EGL_NO_TEXTURE;
  m_texTarget = EGL_NO_TEXTURE;
  m_mipTexture = EGL_FALSE;
  m_mipLevel = 0;
  m_bBoundTexture = false;

#if defined(_WIN32)
  m_hDC = nullptr;
  m_hBitmap = nullptr;
  bool m_bExternalBitmap = false;
#elif defined(__linux__)
  m_pImage = nullptr;
  m_drawable = 0;
  m_gc = 0;
#endif

  m_pDepthStencilBits = nullptr;
  m_ppBuffers = nullptr;
  m_mipLevels = 0;
  m_glSurface = nullptr;
}

CEGLSurface::~CEGLSurface() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  __safeDeleteArray(m_pDepthStencilBits);

  if (m_ppBuffers) {
    for (unsigned i = 0; i < m_mipLevels; ++i) {
      delete[] m_ppBuffers[i];
    }

    delete[] m_ppBuffers;
  }

  if (m_glSurface) {
    __glDestroySurface(m_glSurface);
  }

#if defined(_WIN32)
  if (m_hDC) {
    DeleteDC(m_hDC);
  }
  if (m_hBitmap && m_bExternalBitmap)
    DeletObject(m_hBitmap);
}
#elif defined(__linux__)
  if (m_pImage) {
    XDestroyImage(m_pImage);
  }
  if (m_gc) {
    XFreeGC(m_pDisplay->GetNativeHandle(), m_gc);
  }
#endif

__safeRelease(m_pConfig);
}

EGLint CEGLSurface::CreateWND(CEGLSurface **ppSurface, CDisplay *display,
                              EGLint surfaceType, CConfig *pConfig,
                              EGLNativeWindowType hWnd) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppSurface);

  // Create a new surface object
  CEGLSurface *pSurface = new CEGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError(_T("CEGLSurface allocation failed, out of memory.\r\n"));
    return EGL_BAD_ALLOC;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->InitializeWND(hWnd);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglLogError(_T("CEGLSurface::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::CreatePXM(CEGLSurface **ppSurface, CDisplay *display,
                              EGLint surfaceType, CConfig *pConfig,
                              EGLNativePixmapType hPixmap) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppSurface);

  // Create a new surface object
  CEGLSurface *pSurface = new CEGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError(_T("CEGLSurface allocation failed, out of memory.\r\n"));
    return EGL_BAD_ALLOC;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->InitializePXM(hPixmap);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglLogError(_T("CEGLSurface::Initialize() failed, err = %d.\r\n"), err);
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
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppSurface);

  // Create a new surface object
  CEGLSurface *pSurface = new CEGLSurface(display, surfaceType, pConfig);
  if (nullptr == pSurface) {
    __eglLogError(_T("CEGLSurface allocation failed, out of memory.\r\n"));
    return EGL_BAD_ALLOC;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->InitializePBF(width, height, largestPBuffer, texTarget,
                                texFormat, bGenMipMaps);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglLogError(_T("CEGLSurface::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  *ppSurface = pSurface;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::InitializeWND(EGLNativeWindowType hWnd) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  GLSurfaceDesc colorDesc;

#if defined(_WIN32)
  // Query the window dimensions
  Rect rect;
  if (!::GetClientRect(hWnd, &rect)) {
    __eglLogError(_T("::GetClientRect() failed, hr = %x.\r\n"),
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  m_width = rect.right - rect.left;
  m_height = rect.bottom - rect.top;

  // Retrieve the window device context
  HDC hDC = ::GetWindowDC(hWnd);
  hDC hCompatDC = ::CreateCompatibleDC(hDC);
  if (nullptr == hCompatDC) {
    ::ReleaseDC(hWnd, hDC);
    __eglLogError(_T("::CreateCompatibleDC() failed, hr = %x.\r\n"),
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  // Release the device context
  ::ReleaseDC(hWnd, hDC);

  // Retrieve the config bits per pixels
  int cBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);

  struct BitmapInfo {
    BITMAPINFOHEADER bmiHeader;
    uint32_t bmiColors[3];
  } bmpInfo;

  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = m_width;
  bmpInfo.bmiHeader.biHeight = m_height;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biBitCount = static_cast<uint16_t>(cBPP);
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage = m_width * m_height * (cBPP >> 3);
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
    __eglLogError(_T("::CreateDIBSection() failed, hr = %x.\r\n"),
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

  m_hBitmap = hBitmap;
  m_hDC = hCompatDC;
  m_bExternalBitmap = true;
#elif defined(__linux__)
  Window root;
  int32_t x, y;
  uint32_t width, height;
  uint32_t border, depth;
  auto status = XGetGeometry(m_pDisplay->GetNativeHandle(), hWnd, &root, &x, &y,
                             &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError(_T("::XGetGeometry() failed, status=%d.\r\n"), status);
    return EGL_BAD_ALLOC;
  }
  auto image = XGetImage(m_pDisplay->GetNativeHandle(), hWnd, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError(_T("::XGetImage() failed.\r\n"));
    return EGL_BAD_ALLOC;
  }

  m_gc = XCreateGC(m_pDisplay->GetNativeHandle(), hWnd, 0, nullptr);
  m_drawable = hWnd;
  m_pImage = image;

  colorDesc.Format = CEGLSurface::GetColorFormat(image->bits_per_pixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(image->data);
  colorDesc.Width = image->width;
  colorDesc.Height = image->height;
  colorDesc.Pitch = image->bytes_per_line;
#endif

  m_width = colorDesc.Width;
  m_height = colorDesc.Height;

  // Build the depth stencil description
  GLSurfaceDesc depthStencilDesc;
  err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                               &depthStencilDesc);
  if (__eglFailed(err)) {
    __eglLogError(_T("CEGLSurface::InitDepthStencil() failed, err = %d.\r\n"),
                  err);
    return err;
  }

  // Create the GL surface
  err = EGLERROR_FROM_GLERROR(
      __glCreateSurface(&colorDesc, &depthStencilDesc, &m_glSurface));
  if (__eglFailed(err)) {
    __eglLogError(_T("__glCreateSurface() failed, err = %d.\r\n"), err);
    return err;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::InitializePXM(EGLNativePixmapType hPixmap) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  GLSurfaceDesc colorDesc;

#if defined(_WIN32)
  // Retrieve the bitmap properties
  BITMAP bitmap;
  if ((0 == ::GetObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (nullptr == bitmap.bmBits)) {
    __eglLogError(_T("::GetObject() failed, invalid pixmap.\r\n"));
    return EGL_BAD_ALLOC;
  }

  // Build the color surface description
  colorDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  colorDesc.Pitch = bitmap.bmWidthBytes;
  colorDesc.Width = bitmap.bmWidth;
  colorDesc.Height = bitmap.bmHeight;

  hDC hCompatDC = ::CreateCompatibleDC(m_pDisplay - GetNativeHandle());
  if (nullptr == hCompatDC) {
    __eglLogError(_T("::CreateCompatibleDC() failed, hr = %x.\r\n"),
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  ::SelectObject(hCompatDC, hPixmap);
  m_hDC = hCompatDC;
  m_hBitmap = hPixmap;
#elif defined(__linux__)
  Window root;
  int32_t x, y;
  uint32_t width, height;
  uint32_t border, depth;
  auto status = XGetGeometry(m_pDisplay->GetNativeHandle(), hPixmap, &root, &x,
                             &y, &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError(_T("::XGetGeometry() failed, status=%d.\r\n"), status);
    return EGL_BAD_ALLOC;
  }
  auto image = XGetImage(m_pDisplay->GetNativeHandle(), hPixmap, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError(_T("::XGetImage() failed.\r\n"));
    return EGL_BAD_ALLOC;
  }

  m_gc = XCreateGC(m_pDisplay->GetNativeHandle(), hPixmap, 0, nullptr);
  m_drawable = hPixmap;
  m_pImage = image;

  colorDesc.Format = CEGLSurface::GetColorFormat(image->bits_per_pixel);
  colorDesc.pBits = reinterpret_cast<uint8_t *>(image->data);
  colorDesc.Width = image->width;
  colorDesc.Height = image->height;
  colorDesc.Pitch = image->bytes_per_line;
#endif

  // Check if the config matches the window bpp
  auto cBPP = Format::GetInfo(colorDesc.Format).BytePerPixel * 8;
  if (cBPP != m_pConfig->GetAttribute(EGL_BUFFER_SIZE)) {
    __eglLogError(_T("The config's buffer size doesn't match the window's ")
                  _T("bits per pixel.\r\n"));
    return EGL_BAD_MATCH;
  }

  m_width = colorDesc.Width;
  m_height = colorDesc.Height;

  // Build the depth stencil description
  GLSurfaceDesc depthStencilDesc;
  err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                               &depthStencilDesc);
  if (__eglFailed(err)) {
    __eglLogError(_T("CEGLSurface::InitDepthStencil() failed, err = %d.\r\n"),
                  err);
    return err;
  }

  // Create the GL surface
  err = EGLERROR_FROM_GLERROR(
      __glCreateSurface(&colorDesc, &depthStencilDesc, &m_glSurface));
  if (__eglFailed(err)) {
    __eglLogError(_T("__glCreateSurface() failed, err = %d.\r\n"), err);
    return err;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::InitializePBF(EGLint width, EGLint height,
                                  EGLint largestPBuffer, EGLint texTarget,
                                  EGLint texFormat, EGLint bGenMipMaps) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  m_width = width;
  m_height = height;
  m_texTarget = texTarget;
  m_texFormat = texFormat;
  m_largestPBuffer = largestPBuffer; // Not applicable for software renderers
  m_mipLevels = 0;

  if (width && height) {
    m_mipLevels = 1;

    if (EGL_TEXTURE_2D == texTarget) {
      if ((width < 0) || (width > MAX_PBUFFER_WIDTH) ||
          (!Math::IsPowerOf2(width))) {
        __eglLogError(_T("CEGLSurface::Initialize() failed, invalid width ")
                      _T("parameter: %d.\r\n"),
                      width);
        return EGL_BAD_ATTRIBUTE;
      }

      if ((height < 0) || (height > MAX_PBUFFER_HEIGHT) ||
          !Math::IsPowerOf2(height)) {
        __eglLogError(_T("CEGLSurface::Initialize() failed, invalid height ")
                      _T("parameter: %d.\r\n"),
                      height);
        return EGL_BAD_ATTRIBUTE;
      }

      if (bGenMipMaps) {
        m_mipLevels = Math::TMax(Math::iLog2(width), Math::iLog2(height));
      }
    }

    // Allocate mipmap array
    m_ppBuffers = new uint8_t *[m_mipLevels];
    if (nullptr == m_ppBuffers) {
      __eglLogError(_T("CEGLSurface::Initialize() failed, out of memory.\r\n"));
      return EGL_BAD_ALLOC;
    }

    const unsigned nBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);
    const unsigned mipLevels = m_mipLevels;

    for (unsigned i = 0, _width = width, _height = height; i < mipLevels; ++i) {
      // Allocate mipmap level
      m_ppBuffers[i] = new uint8_t[_width * _height * (nBPP / 8)];
      if (nullptr == m_ppBuffers[i]) {
        __eglLogError(
            _T("CEGLSurface::Initialize() failed, out of memory.\r\n"));
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
    colorDesc.pBits = reinterpret_cast<uint8_t *>(m_ppBuffers[0]);
    colorDesc.Format = CEGLSurface::GetColorFormat(nBPP);
    colorDesc.Pitch = width * (nBPP) / 8;
    colorDesc.Width = width;
    colorDesc.Height = height;

    // Build the depth stencil description
    GLSurfaceDesc depthStencilDesc;
    err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height,
                                 &depthStencilDesc);
    if (__eglFailed(err)) {
      __eglLogError(_T("CEGLSurface::InitDepthStencil() failed, err = %d.\r\n"),
                    err);
      return err;
    }

    // Create the GL surface
    err = EGLERROR_FROM_GLERROR(
        __glCreateSurface(&colorDesc, &depthStencilDesc, &m_glSurface));
    if (__eglFailed(err)) {
      __eglLogError(_T("__glCreateSurface() failed, err = %d.\r\n"), err);
      return err;
    }
  } else {
    // Create the GL surface
    err = EGLERROR_FROM_GLERROR(__glCreateSurface(nullptr, nullptr, &m_glSurface));
    if (__eglFailed(err)) {
      __eglLogError(_T("__glCreateSurface() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::BindTexture() {
  EGLint err;

  // Check if we have a texture bindable surface
  if ((EGL_PBUFFER_BIT != m_surfaceType) || (EGL_TEXTURE_2D != m_texTarget)) {
    __eglLogError(_T("CEGLSurface::BindTexture() failed, the surface is not a ")
                  _T("pbuffer supporting texture binding.\r\n"));
    return EGL_BAD_SURFACE;
  }

  if (m_bBoundTexture) {
    __eglLogError(_T("CEGLSurface::BindTexture() failed, the buffer is ")
                  _T("already bound to a texture.\r\n"));
    return EGL_BAD_ACCESS;
  }

  if (EGL_NO_TEXTURE == m_texFormat) {
    __eglLogError(
        _T("CEGLSurface::BindTexture() failed, the surface attribute ")
        _T("EGL_TEXTURE_FORMAT is set to EGL_NO_TEXTURE.\r\n"));
    return EGL_BAD_MATCH;
  }

  const bool bGenMipmaps = (0 == m_mipLevel) && (m_mipLevels > 1);

  err = EGLERROR_FROM_GLERROR(__glBindTexImage(m_glSurface, bGenMipmaps));
  if (__eglFailed(err)) {
    __eglLogError(_T("__glBindTexImage() failed, err = %d.\r\n"), err);
    return err;
  }

  m_bBoundTexture = true;

  return EGL_SUCCESS;
}

EGLint CEGLSurface::ReleaseTexBound() {
  // Check if we have a texture bindable surface
  if ((EGL_PBUFFER_BIT != m_surfaceType) || (EGL_TEXTURE_2D != m_texTarget)) {
    __eglLogError(
        _T("CEGLSurface::ReleaseTexBound() failed, the surface is not an EGL ")
        _T("surface, or is not a bound pbuffer surface.\r\n"));
    return EGL_BAD_SURFACE;
  }

  // Verify the texture format
  if (EGL_NO_TEXTURE == m_texFormat) {
    __eglLogError(
        _T("CEGLSurface::ReleaseTexBound() failed, the surface attribute ")
        _T("EGL_TEXTURE_FORMAT is set to EGL_NO_TEXTURE.\r\n"));
    return EGL_BAD_MATCH;
  }

  if (m_bBoundTexture) {
    __glReleaseTexImage(m_glSurface);
    m_bBoundTexture = false;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::GetAttribute(EGLint *pValue, EGLint attribute) const {
  EGLint value;

  switch (attribute) {
  case EGL_CONFIG_ID:
    value = m_pConfig->GetAttribute(EGL_CONFIG_ID);
    break;

  case EGL_HEIGHT:
    value = m_height;
    break;

  case EGL_WIDTH:
    value = m_width;
    break;

  case EGL_LARGEST_PBUFFER:
    value = m_largestPBuffer;
    break;

  case EGL_TEXTURE_FORMAT:
    value = m_texFormat;
    break;

  case EGL_MIPMAP_TEXTURE:
    value = m_mipTexture;
    break;

  case EGL_MIPMAP_LEVEL:
    value = m_mipLevel;
    break;

  default:
    __eglLogError(
        _T("CEGLSurface::GetAttribute() failed, invalid attribute: %d.\r\n"),
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
    m_mipLevel = value;

    if (m_ppBuffers) {
      GLSurfaceDesc surfDesc;
      this->GetPBufferDesc(&surfDesc);

      // Update the GL surface
      err = EGLERROR_FROM_GLERROR(
          __glUpdateSurface(m_glSurface, &surfDesc, nullptr));
      if (__eglFailed(err)) {
        __eglLogError(_T("__glCreateSurface() failed, err = %d.\r\n"), err);
        return err;
      }
    }
  }

  break;

  default:
    __eglLogError(
        _T("CEGLSurface::SetAttribute() failed, invalid attribute: %d.\r\n"),
        attribute);
    return EGL_BAD_ATTRIBUTE;
  }

  return EGL_SUCCESS;
}

EGLint CEGLSurface::CopyBuffer(EGLNativePixmapType hPixmap) {
  EGLint err;

  GLSurfaceDesc srcDesc;

  if (m_ppBuffers) {
    const unsigned mipLevel =
        Math::TClamp<unsigned>(m_mipLevel, 0, m_mipLevels - 1);
    const unsigned nBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);
    srcDesc.pBits = m_ppBuffers[mipLevel];
    srcDesc.Format = CEGLSurface::GetColorFormat(nBPP);
    srcDesc.Pitch = m_width * (nBPP / 8);
    srcDesc.Width = m_width;
    srcDesc.Height = m_height;
  } else {
#if defined(_WIN32)
    // Retrieve the bitmap properties
    BITMAP bitmap;
    if ((0 == ::GetObject(m_hBitmap, sizeof(BITMAP), &bitmap)) ||
        (nullptr == bitmap.bmBits)) {
      __eglLogError(_T("::GetObject() failed, invalid pixmap.\r\n"));
      return EGL_BAD_ALLOC;
    }

    // Build the color surface description
    srcDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
    srcDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
    srcDesc.Pitch = bitmap.bmWidthBytes;
    srcDesc.Width = bitmap.bmWidth;
    srcDesc.Height = bitmap.bmHeight;
#elif defined(__linux__)
    srcDesc.Format = CEGLSurface::GetColorFormat(m_pImage->bits_per_pixel);
    srcDesc.pBits = reinterpret_cast<uint8_t *>(m_pImage->data);
    srcDesc.Width = m_pImage->width;
    srcDesc.Height = m_pImage->height;
    srcDesc.Pitch = m_pImage->bytes_per_line;
#endif
  }

  GLSurfaceDesc dstDesc;

#if defined(_WIN32)
  // Retrieve the bitmap properties
  BITMAP bitmap;
  if ((0 == ::GetObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (nullptr == bitmap.bmBits)) {
    __eglLogError(_T("::GetObject() failed, invalid pixmap.\r\n"));
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
  auto status = XGetGeometry(m_pDisplay->GetNativeHandle(), hPixmap, &root, &x,
                             &y, &width, &height, &border, &depth);
  if (0 == status) {
    __eglLogError(_T("::XGetGeometry() failed, status=%d.\r\n"), status);
    return EGL_BAD_ALLOC;
  }
  auto image = XGetImage(m_pDisplay->GetNativeHandle(), hPixmap, 0, 0, width,
                         height, AllPlanes, ZPixmap);
  if (nullptr == image) {
    __eglLogError(_T("::XGetImage() failed.\r\n"));
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
  err = EGLERROR_FROM_GLERROR(__glCopyBuffers(&dstDesc, &srcDesc));
  if (__eglFailed(err)) {
    __eglLogError(_T("__glCopySurface() failed, err = %d.\r\n"), err);
    return err;
  }

  return EGL_SUCCESS;
}

void CEGLSurface::Present() {
#if defined(_WIN32)
  ::BitBlt(m_pDisplay->GetDGetNativeHandleC(), 0, 0, m_width, m_height, m_hDC,
           0, 0, SRCCOPY);
#elif defined(__linux__)
  XPutImage(m_pDisplay->GetNativeHandle(), m_drawable, m_gc, m_pImage, 0, 0, 0,
            0, m_width, m_height);
#endif
}

GLenum CEGLSurface::SaveBitmap(LPCTSTR lpszFilename) {
  return __glSaveBitmap(m_glSurface, lpszFilename);
}

uint8_t CEGLSurface::GetColorFormat(unsigned cBitsPerPixel) {
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

EGLint CEGLSurface::InitDepthStencil(unsigned width, unsigned height,
                                     GLSurfaceDesc *pSurfaceDesc) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  ASSERT(pSurfaceDesc);

  pSurfaceDesc->Width = width;
  pSurfaceDesc->Height = height;

  unsigned stride = (m_pConfig->GetAttribute(EGL_DEPTH_SIZE) +
                     m_pConfig->GetAttribute(EGL_STENCIL_SIZE)) >>
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
    m_pDepthStencilBits =
        new uint8_t[stride * pSurfaceDesc->Width * pSurfaceDesc->Height];
    if (nullptr == m_pDepthStencilBits) {
      __eglLogError(
          _T("Depth stencil buffer allocation failed, out of memory.\r\n"));
      return EGL_BAD_ALLOC;
    }

    pSurfaceDesc->pBits = m_pDepthStencilBits;
  } else {
    pSurfaceDesc->pBits = nullptr;
  }

  pSurfaceDesc->Pitch = stride * pSurfaceDesc->Width;

  return EGL_SUCCESS;
}

void CEGLSurface::GetPBufferDesc(GLSurfaceDesc *pSurfaceDesc) {
  ASSERT(pSurfaceDesc);

  const unsigned mipLevel =
      Math::TClamp<unsigned>(m_mipLevel, 0, m_mipLevels - 1);

  unsigned width = m_width;
  unsigned height = m_height;

  for (unsigned i = 0; i < mipLevel; ++i) {
    if (width > 1) {
      width >>= 1;
    }

    if (height > 1) {
      height >>= 1;
    }
  }

  const unsigned nBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);

  pSurfaceDesc->Format = CEGLSurface::GetColorFormat(nBPP);
  pSurfaceDesc->pBits = m_ppBuffers[mipLevel];
  pSurfaceDesc->Width = width;
  pSurfaceDesc->Height = height;
  pSurfaceDesc->Pitch = width * (nBPP / 8);
}