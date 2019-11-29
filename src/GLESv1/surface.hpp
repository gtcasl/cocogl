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
#pragma once

class CGLSurface : public CObject {
public:
  enum {
    ATTRIBUTES_FIRST = GL_SUBPIXEL_BITS,
    ATTRIBUTES_LAST = GL_STENCIL_BITS,
    ATTRIBUTES_SIZE = (ATTRIBUTES_LAST - ATTRIBUTES_FIRST) + 1,
  };

  typedef void (*PFN_ColorFill)(const GLSurfaceDesc &surfDesc, uint32_t value,
                                uint32_t mask, const Rect &rect);

  static GLenum Create(CGLSurface **ppSurface, const GLSurfaceDesc *pColorDesc,
                       const GLSurfaceDesc *pDepthStencilDesc);

  void GetColorDesc(GLSurfaceDesc *pSurfDesc) const {
    ASSERT(pSurfDesc);
    *pSurfDesc = m_pColorDesc;
  }

  void GetDepthStencilDesc(GLSurfaceDesc *pSurfDesc) const {
    ASSERT(pSurfDesc);
    *pSurfDesc = m_pDepthStencilDesc;
  }

  void GetRect(Rect *pRect) const {
    ASSERT(pRect);
    pRect->left = 0;
    pRect->top = 0;
    pRect->right = m_pColorDesc.Width;
    pRect->bottom = m_pColorDesc.Height;
  }

  uint32_t GetColorFormat() const { return m_pColorDesc.Format; }

  uint32_t GetDepthStencilFormat() const { return m_pDepthStencilDesc.Format; }

  uint32_t GetAlignment() const { return 1; }

  GLint GetAttribute(GLint name) const {
    ASSERT(name >= ATTRIBUTES_FIRST && name <= ATTRIBUTES_LAST);
    return m_attributes[name - ATTRIBUTES_FIRST];
  }

  void SetAttribute(GLint name, GLint value) {
    ASSERT(name >= ATTRIBUTES_FIRST && name <= ATTRIBUTES_LAST);
    m_attributes[name - ATTRIBUTES_FIRST] = value;
  }

  uint32_t GetWidth() const { return m_pColorDesc.Width; }

  uint32_t GetHeight() const { return m_pColorDesc.Height; }

  const GLSurfaceDesc &GetColorDesc() const { return m_pColorDesc; }

  const GLSurfaceDesc &GetDepthStencilDesc() const {
    return m_pDepthStencilDesc;
  }

  GLenum Update(const GLSurfaceDesc *pColorDesc,
                const GLSurfaceDesc *pDepthStencilDesc) {
    return this->Initialize(pColorDesc, pDepthStencilDesc);
  }

  void ConvertColor(void *pOut, const Color4 &color) const {
    (m_pfnColorConv)(pOut, color);
  }

  void ClearColor(uint32_t colorValue, uint32_t colorMask, const Rect &rect) {
    (m_pfnColorFill)(m_pColorDesc, colorValue, colorMask, rect);
  }

  void ClearDepth(uint32_t depthValue, uint32_t depthMask, const Rect &rect) {
    (m_pfnDepthStencilFill)(m_pDepthStencilDesc, depthValue, depthMask, rect);
  }

  void ClearStencil(uint32_t clearStencil, uint32_t stencilMask,
                    const Rect &rect) {
    uint32_t value = clearStencil << 16;
    uint32_t mask = stencilMask << 16;
    (m_pfnDepthStencilFill)(m_pDepthStencilDesc, value, mask, rect);
  }

  void ClearDepthStencil(uint32_t depthValue, uint32_t depthMask,
                         uint32_t stencilValue, uint32_t stencilMask,
                         const Rect &rect) {
    uint32_t value = 0xff000000 | (stencilValue << 16) | depthValue;
    uint32_t mask = 0xff000000 | (stencilMask << 16) | depthMask;
    (m_pfnDepthStencilFill)(m_pDepthStencilDesc, value, mask, rect);
  }

  GLenum SaveBitmap(LPCTSTR lpszFilename);

private:
  enum {
    SUBPIXEL_BITS = 4,
  };

  CGLSurface();
  ~CGLSurface();

  GLenum Initialize(const GLSurfaceDesc *pColorDesc,
                    const GLSurfaceDesc *pDepthStencilDesc);

  template <class T>
  static void TColorFill(const GLSurfaceDesc &surfDesc, uint32_t value,
                         uint32_t mask, const Rect &rect);

  static void ColorFillNoop(const GLSurfaceDesc &surfDesc, uint32_t value,
                            uint32_t mask, const Rect &rect) {
    __unreferenced(surfDesc);
    __unreferenced(value);
    __unreferenced(mask);
    __unreferenced(rect);
  }

  static void ColorConvNoop(void *pColorOut, const Color4 &color) {
    __unreferenced(pColorOut);
    __unreferenced(color);
  }

  GLint m_attributes[ATTRIBUTES_SIZE];

  Format::PFN_CONVERTTO m_pfnColorConv;

  PFN_ColorFill m_pfnColorFill;
  PFN_ColorFill m_pfnDepthStencilFill;

  GLSurfaceDesc m_pColorDesc;
  GLSurfaceDesc m_pDepthStencilDesc;
};
