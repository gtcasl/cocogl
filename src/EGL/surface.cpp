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


CEGLSurface::CEGLSurface(EGLint surfaceType, CConfig *pConfig) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  ASSERT(pConfig);
  pConfig->AddRef();
  m_pConfig = pConfig;

  m_surfaceType = surfaceType;

  m_hDC = NULL;

  m_width = 0;
  m_height = 0;
  m_largestPBuffer = EGL_FALSE;
  m_texFormat = EGL_NO_TEXTURE;
  m_texTarget = EGL_NO_TEXTURE;
  m_mipTexture = EGL_FALSE;
  m_mipLevel = 0;
  m_bBoundTexture = false;

  m_hPixmap = 0;
  m_bExternalBitmap = false;
  m_pDepthStencilBits = NULL;
  m_ppBuffers = NULL;
  m_mipLevels = 0;
  m_glSurface = NULL;
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

  if (m_hPixmap && !m_bExternalBitmap) {
    DestroyPixmap(m_hPixmap);
  }

  if (m_hDC) {
    ReleaseDisplay(m_hDC);
  }

  __safeRelease(m_pConfig);
}


EGLint CEGLSurface::CreateWND(CEGLSurface **ppSurface, EGLint surfaceType,
                           CConfig *pConfig, EGLNativeWindowType hWnd) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppSurface);

  // Create a new surface object
  CEGLSurface *pSurface = new CEGLSurface(surfaceType, pConfig);
  if (NULL == pSurface) {
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

EGLint CEGLSurface::CreatePXM(CEGLSurface **ppSurface, EGLint surfaceType,
                           CConfig *pConfig, EGLNativePixmapType hPixmap) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppSurface);

  // Create a new surface object
  CEGLSurface *pSurface = new CEGLSurface(surfaceType, pConfig);
  if (NULL == pSurface) {
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


EGLint CEGLSurface::CreatePBF(CEGLSurface **ppSurface, EGLint surfaceType,
                           CConfig *pConfig, EGLint width, EGLint height,
                           EGLint largestPBuffer, EGLint texTarget,
                           EGLint texFormat, EGLint bGenMipMaps) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppSurface);

  // Create a new surface object
  CEGLSurface *pSurface = new CEGLSurface(surfaceType, pConfig);
  if (NULL == pSurface) {
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

EGLint CEGLSurface::GetSurfaceDesc(GLSurfaceDesc *pSurfaceDesc, EGLNativePixmapType hPixmap) {
  EGLint err;  

#if defined (_WIN32)
  // Retrieve the bitmap properties
  BITMAP bitmap;
  if ((0 == ::GetObject(hPixmap, sizeof(BITMAP), &bitmap)) ||
      (NULL == bitmap.bmBits)) {
    __eglLogError(_T("::GetObject() failed, invalid pixmap.\r\n"));
    return EGL_BAD_NATIVE_PIXMAP;
  }

  // Build the color surface description  
  colorDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
  pSurfaceDesc->pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);  
  pSurfaceDesc->Pitch = static_cast<uint16_t>(bitmap.bmWidthBytes);
  pSurfaceDesc->Width = static_cast<uint16_t>(bitmap.bmWidth);
  pSurfaceDesc->Height = static_cast<uint16_t>(bitmap.bmHeight);
#elif defined(__linux__)
 //TODO:
#endif 

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
    return EGL_BAD_NATIVE_WINDOW;
  }

  m_width = rect.right - rect.left;
  m_height = rect.bottom - rect.top;

  // Retrieve the window device context
  const HDC hDC = ::GetWindowDC(hWnd);
  m_hDC = ::CreateCompatibleDC(hDC);
  if (NULL == m_hDC) {
    ::ReleaseDisplay(hWnd, hDC);
    __eglLogError(_T("::CreateCompatibleDC() failed, hr = %x.\r\n"),
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_NATIVE_WINDOW;
  }

  // Release the device context
  ::ReleaseDisplay(hWnd, hDC);

  // Retrieve the config bits per pixels
  const int cBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);

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
  m_hPixmap =
      ::CreateDIBSection(m_hDC, reinterpret_cast<BITMAPINFO *>(&bmpInfo),
                         DIB_RGB_COLORS, &pBits, NULL, 0);
  if (NULL == m_hPixmap) {
    __eglLogError(_T("::CreateDIBSection() failed, hr = %x.\r\n"),
                  HRESULT_FROM_WIN32(::GetLastError()));
    return EGL_BAD_ALLOC;
  }

  // Assign the bitmap to the DC
  ::SelectObject(m_hDC, m_hPixmap);

  // Retrieve the BITMAP properties
  BITMAP bitmap;
  ::GetObject(m_hPixmap, sizeof(BITMAP), &bitmap);

  // Build the color surface description  
  colorDesc.pBits = reinterpret_cast<uint8_t *>(bitmap.bmBits);
  colorDesc.Format = CEGLSurface::GetColorFormat(bitmap.bmBitsPixel);
  colorDesc.Pitch = static_cast<uint16_t>(bitmap.bmWidthBytes);
  colorDesc.Width = static_cast<uint16_t>(bitmap.bmWidth);
  colorDesc.Height = static_cast<uint16_t>(bitmap.bmHeight);
#elif defined(__linux__)
  //TODO:
#endif

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

  err = this->GetSurfaceDesc(&colorDesc, hPixmap);
  if (__eglFailed(err)) {
    __eglLogError(_T("CEGLSurface::GetSurfaceDesc() failed, err = %d.\r\n"), err);
    return err;
  }

  // Check if the config matches the window bpp
  auto cBPP = Format::GetInfo(colorDesc.Format).BytePerPixel * 8;
  if (cBPP != m_pConfig->GetAttribute(EGL_BUFFER_SIZE)) {
    __eglLogError(_T("The config's buffer size doesn't match the window's bits per pixel.\r\n"));
    return EGL_BAD_MATCH;
  }

  m_width = colorDesc.Width;
  m_height = colorDesc.Height;
  m_hPixmap = hPixmap;
  m_bExternalBitmap = true;

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
    if (NULL == m_ppBuffers) {
      __eglLogError(_T("CEGLSurface::Initialize() failed, out of memory.\r\n"));
      return EGL_BAD_ALLOC;
    }

    const unsigned nBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);
    const unsigned mipLevels = m_mipLevels;

    for (unsigned i = 0, _width = width, _height = height; i < mipLevels; ++i) {
      // Allocate mipmap level
      m_ppBuffers[i] = new uint8_t[_width * _height * (nBPP/8)];
      if (NULL == m_ppBuffers[i]) {
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
    colorDesc.Pitch = static_cast<uint16_t>(width * (nBPP)/8);
    colorDesc.Width = static_cast<uint16_t>(width);
    colorDesc.Height = static_cast<uint16_t>(height);

    // Build the depth stencil description
    GLSurfaceDesc depthStencilDesc;
    err = this->InitDepthStencil(colorDesc.Width, colorDesc.Height, &depthStencilDesc);
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
    err = EGLERROR_FROM_GLERROR(__glCreateSurface(NULL, NULL, &m_glSurface));
    if (__eglFailed(err)) {
      __eglLogError(_T("__glCreateSurface() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  return EGL_SUCCESS;
}


bool CEGLSurface::IsValid() const {
  if ((EGL_WINDOW_BIT == m_surfaceType) || (EGL_PIXMAP_BIT == m_surfaceType)) {
    return (m_hPixmap != 0);
  }
  return true;
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
      GLSurfaceDesc colorDesc;
      this->GetColorDesc(&colorDesc);

      // Update the GL surface
      err = EGLERROR_FROM_GLERROR(
          __glUpdateSurface(m_glSurface, &colorDesc, NULL));
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

  GLSurfaceDesc dstDesc;

  err = this->GetSurfaceDesc(&dstDesc, hPixmap);
  if (__eglFailed(err)) {
    __eglLogError(_T("CEGLSurface::GetSurfaceDesc() failed, err = %d.\r\n"), err);
    return err;
  }

  GLSurfaceDesc srcDesc;

  if (m_ppBuffers) {
    const unsigned mipLevel =
        Math::TClamp<unsigned>(m_mipLevel, 0, m_mipLevels - 1);
    const unsigned nBPP = m_pConfig->GetAttribute(EGL_BUFFER_SIZE);
    srcDesc.pBits = m_ppBuffers[mipLevel];
    srcDesc.Format = CEGLSurface::GetColorFormat(nBPP);
    srcDesc.Pitch = static_cast<uint16_t>(m_width * (nBPP/8));
    srcDesc.Width = static_cast<uint16_t>(m_width);
    srcDesc.Height = static_cast<uint16_t>(m_height);
  } else {
    err = this->GetSurfaceDesc(&dstDesc, m_hPixmap);
    if (__eglFailed(err)) {
      __eglLogError(_T("CEGLSurface::GetSurfaceDesc() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  // Copy buffers using GL
  err = EGLERROR_FROM_GLERROR(__glCopyBuffers(&dstDesc, &srcDesc));
  if (__eglFailed(err)) {
    __eglLogError(_T("__glCopySurface() failed, err = %d.\r\n"), err);
    return err;
  }

  return EGL_SUCCESS;
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

  pSurfaceDesc->Width = static_cast<uint16_t>(width);
  pSurfaceDesc->Height = static_cast<uint16_t>(height);

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
    if (NULL == m_pDepthStencilBits) {
      __eglLogError(
          _T("Depth stencil buffer allocation failed, out of memory.\r\n"));
      return EGL_BAD_ALLOC;
    }

    pSurfaceDesc->pBits = m_pDepthStencilBits;
  } else {
    pSurfaceDesc->pBits = NULL;
  }

  pSurfaceDesc->Pitch = static_cast<uint16_t>(stride * pSurfaceDesc->Width);

  return EGL_SUCCESS;
}


void CEGLSurface::GetColorDesc(GLSurfaceDesc *pSurfaceDesc) {
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
  pSurfaceDesc->Width = static_cast<uint16_t>(width);
  pSurfaceDesc->Height = static_cast<uint16_t>(height);
  pSurfaceDesc->Pitch = static_cast<uint16_t>(width * (nBPP / 8));
}


GLenum CEGLSurface::SaveBitmap(LPCTSTR lpszFilename) {
  return __glSaveBitmap(m_glSurface, lpszFilename);
}
