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
#pragma once

class _EGLConfig;
class _EGLDisplay;

class _EGLSurface : public Object {
public:
  static EGLint CreateWND(_EGLSurface **ppSurface, _EGLDisplay *display,
                          EGLint surfaceType, _EGLConfig *pConfig,
                          EGLNativeWindowType hWnd);

  static EGLint CreatePXM(_EGLSurface **ppSurface, _EGLDisplay *display,
                          EGLint surfaceType, _EGLConfig *pConfig,
                          EGLNativePixmapType hPixmap);

  static EGLint CreatePBF(_EGLSurface **ppSurface, _EGLDisplay *display,
                          EGLint surfaceType, _EGLConfig *pConfig, EGLint width,
                          EGLint height, EGLint largestPBuffer,
                          EGLint texTarget, EGLint texFormat,
                          EGLint bGenMipMaps);

  EGLint getAttribute(EGLint *pValue, EGLint name) const;

  EGLint setAttribute(EGLint name, EGLint value);

  EGLint copyBuffer(EGLNativePixmapType hPixmap);

  auto getConfig() const { return pConfig_; }

  auto getType() const { return surfaceType_; }

  auto getNativeData() const { return glSurface_; }

  EGLint bindTexture();

  EGLint releaseTexBound();

  HRESULT saveBitmap(const char *filename);

  void present();

private:
  enum {
    MAX_WIDTH = 480,
    MAX_HEIGHT = 480,
  };

  _EGLSurface(_EGLDisplay *pDisplay, EGLint surfaceType, _EGLConfig *pConfig);

  ~_EGLSurface();

  EGLint InitializeWND(EGLNativeWindowType hWnd);

  EGLint InitializePXM(EGLNativePixmapType hPixmap);

  EGLint InitializePBF(EGLint width, EGLint height, EGLint largestPBuffer,
                       EGLint texTarget, EGLint texFormat, EGLint bGenMipMaps);

  EGLint InitDepthStencil(uint32_t width, uint32_t height,
                          GLSurfaceDesc *pSurfaceDesc);

  void getPBufferDesc(GLSurfaceDesc *pSurfaceDesc);

  static uint8_t getColorFormat(uint32_t cBitsPerPixel);

  _EGLDisplay *pDisplay_;
  _EGLConfig *pConfig_;
  EGLint surfaceType_;

  bool bBoundTexture_;
  EGLint width_;
  EGLint height_;
  EGLint largestPBuffer_;
  EGLint texFormat_;
  EGLint texTarget_;
  EGLint mipTexture_;
  EGLint mipLevel_;

#if defined(_WIN32)
  HDC hDC_;
  HBITMAP hBitmap_;
  bool bExternalBitmap_;
#elif defined(__linux__)
  XImage *pImage_;
  Drawable drawable_;
  GC gc_;
#endif

  uint8_t *pDepthStencilBits_;
  uint8_t **ppBuffers_;
  uint32_t mipLevels_;
  __GLSurface glSurface_;
};
