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

class CConfig;
class CDisplay;

class CEGLSurface : public CObject {
public:
  static EGLint CreateWND(CEGLSurface **ppSurface, CDisplay *display,
                          EGLint surfaceType, CConfig *pConfig,
                          EGLNativeWindowType hWnd);

  static EGLint CreatePXM(CEGLSurface **ppSurface, CDisplay *display,
                          EGLint surfaceType, CConfig *pConfig,
                          EGLNativePixmapType hPixmap);

  static EGLint CreatePBF(CEGLSurface **ppSurface, CDisplay *display,
                          EGLint surfaceType, CConfig *pConfig, EGLint width,
                          EGLint height, EGLint largestPBuffer,
                          EGLint texTarget, EGLint texFormat,
                          EGLint bGenMipMaps);

  EGLint GetAttribute(EGLint *pValue, EGLint name) const;

  EGLint SetAttribute(EGLint name, EGLint value);

  EGLint CopyBuffer(EGLNativePixmapType hPixmap);

  CConfig *GetConfig() const { return pConfig_; }

  EGLint GetType() const { return surfaceType_; }

  __GLSurface GetNativeData() const { return glSurface_; }

  EGLint BindTexture();

  EGLint ReleaseTexBound();

  HRESULT SaveBitmap(const char *filename);

  void Present();

private:
  enum {
    MAX_WIDTH = 480,
    MAX_HEIGHT = 480,
  };

  CEGLSurface(CDisplay *pDisplay, EGLint surfaceType, CConfig *pConfig);
  ~CEGLSurface();

  EGLint InitializeWND(EGLNativeWindowType hWnd);

  EGLint InitializePXM(EGLNativePixmapType hPixmap);

  EGLint InitializePBF(EGLint width, EGLint height, EGLint largestPBuffer,
                       EGLint texTarget, EGLint texFormat, EGLint bGenMipMaps);

  static uint8_t GetColorFormat(uint32_t cBitsPerPixel);

  EGLint InitDepthStencil(uint32_t width, uint32_t height,
                          GLSurfaceDesc *pSurfaceDesc);

  void GetPBufferDesc(GLSurfaceDesc *pSurfaceDesc);

  CDisplay *pDisplay_;
  CConfig *pConfig_;
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
