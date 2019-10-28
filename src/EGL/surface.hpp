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


class CEGLSurface : public CObject {
public:
  static EGLint CreateWND(CEGLSurface **ppSurface, EGLint surfaceType,
                       CConfig *pConfig, EGLNativeWindowType hWnd);

  static EGLint CreatePXM(CEGLSurface **ppSurface, EGLint surfaceType,
                       CConfig *pConfig, EGLNativePixmapType hPixmap);

  static EGLint CreatePBF(CEGLSurface **ppSurface, EGLint surfaceType,
                       CConfig *pConfig, EGLint width, EGLint height,
                       EGLint largestPBuffer, EGLint texTarget,
                       EGLint texFormat, EGLint bGenMipMaps);

  EGLint GetAttribute(EGLint *pValue, EGLint name) const;
  EGLint SetAttribute(EGLint name, EGLint value);

  EGLint CopyBuffer(EGLNativePixmapType hPixmap);

  CConfig *GetConfig() const { return m_pConfig; }

  EGLint GetType() const { return m_surfaceType; }

  EGLNativeDisplayType GetDC() const { return m_hDC; }

  __GLSurface GetNativeData() const { return m_glSurface; }

  EGLint GetWidth() const { return m_width; }

  EGLint GetHeight() const { return m_height; }

  bool IsValid() const;

  EGLint BindTexture();

  EGLint ReleaseTexBound();

  GLenum SaveBitmap(LPCTSTR lpszFilename);

private:
  enum {
    MAX_WIDTH = 480,
    MAX_HEIGHT = 480,
  };

  CEGLSurface(EGLint surfaceType, CConfig *pConfig);
  ~CEGLSurface();

  EGLint InitializeWND(EGLNativeWindowType hWnd);

  EGLint InitializePXM(EGLNativePixmapType hPixmap);

  EGLint InitializePBF(EGLint width, EGLint height, EGLint largestPBuffer,
                    EGLint texTarget, EGLint texFormat, EGLint bGenMipMaps);

  static uint8_t GetColorFormat(unsigned cBitsPerPixel);

  EGLint InitDepthStencil(unsigned width, unsigned height,
                          GLSurfaceDesc *pSurfaceDesc);

  void GetColorDesc(GLSurfaceDesc *pSurfaceDesc);

  EGLint GetSurfaceDesc(GLSurfaceDesc *pSurfaceDesc, EGLNativePixmapType hPixmap);

  CConfig *m_pConfig;
  EGLint m_surfaceType;
  EGLNativeDisplayType m_hDC;

  bool m_bBoundTexture;
  EGLint m_width;
  EGLint m_height;
  EGLint m_largestPBuffer;
  GLuint m_texFormat;
  EGLint m_texTarget;
  EGLint m_mipTexture;
  EGLint m_mipLevel;

  EGLNativePixmapType m_hPixmap;
  uint8_t *m_pDepthStencilBits;
  bool m_bExternalBitmap;
  uint8_t **m_ppBuffers;
  unsigned m_mipLevels;
  __GLSurface m_glSurface;
};
