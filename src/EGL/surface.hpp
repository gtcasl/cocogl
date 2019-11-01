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

  CConfig *GetConfig() const { return m_pConfig; }

  EGLint GetType() const { return m_surfaceType; }

  __GLSurface GetNativeData() const { return m_glSurface; }

  EGLint BindTexture();

  EGLint ReleaseTexBound();

  GLenum SaveBitmap(LPCTSTR lpszFilename);

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

  CDisplay *m_pDisplay;
  CConfig *m_pConfig;
  EGLint m_surfaceType;

  bool m_bBoundTexture;
  EGLint m_width;
  EGLint m_height;
  EGLint m_largestPBuffer;
  GLuint m_texFormat;
  EGLint m_texTarget;
  EGLint m_mipTexture;
  EGLint m_mipLevel;

#if defined(_WIN32)
  HDC m_hDC;
  HBITMAP m_hBitmap;
  bool m_bExternalBitmap;
#elif defined(__linux__)
  XImage *m_pImage;
  Drawable m_drawable;
  GC m_gc;
#endif

  uint8_t *m_pDepthStencilBits;
  uint8_t **m_ppBuffers;
  uint32_t m_mipLevels;
  __GLSurface m_glSurface;
};
