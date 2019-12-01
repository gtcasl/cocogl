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

  typedef void (*PfnColorFill)(const GLSurfaceDesc &surfDesc, uint32_t value,
                                uint32_t mask, const Rect &rect);

  static GLenum Create(CGLSurface **ppSurface, const GLSurfaceDesc *pColorDesc,
                       const GLSurfaceDesc *pDepthStencilDesc);

  void GetColorDesc(GLSurfaceDesc *pSurfDesc) const {
    assert(pSurfDesc);
    *pSurfDesc = pColorDesc_;
  }

  void GetDepthStencilDesc(GLSurfaceDesc *pSurfDesc) const {
    assert(pSurfDesc);
    *pSurfDesc = pDepthStencilDesc_;
  }

  void GetRect(Rect *pRect) const {
    assert(pRect);
    pRect->left = 0;
    pRect->top = 0;
    pRect->right = pColorDesc_.Width;
    pRect->bottom = pColorDesc_.Height;
  }

  uint32_t GetColorFormat() const { return pColorDesc_.Format; }

  uint32_t GetDepthStencilFormat() const { return pDepthStencilDesc_.Format; }

  uint32_t GetAlignment() const { return 1; }

  GLint GetAttribute(GLint name) const {
    assert(name >= ATTRIBUTES_FIRST && name <= ATTRIBUTES_LAST);
    return attributes_[name - ATTRIBUTES_FIRST];
  }

  void SetAttribute(GLint name, GLint value) {
    assert(name >= ATTRIBUTES_FIRST && name <= ATTRIBUTES_LAST);
    attributes_[name - ATTRIBUTES_FIRST] = value;
  }

  uint32_t GetWidth() const { return pColorDesc_.Width; }

  uint32_t GetHeight() const { return pColorDesc_.Height; }

  const GLSurfaceDesc &GetColorDesc() const { return pColorDesc_; }

  const GLSurfaceDesc &GetDepthStencilDesc() const {
    return pDepthStencilDesc_;
  }

  GLenum Update(const GLSurfaceDesc *pColorDesc,
                const GLSurfaceDesc *pDepthStencilDesc) {
    return this->Initialize(pColorDesc, pDepthStencilDesc);
  }

  void ConvertColor(void *pOut, const Color4 &color) const {
    (pfnColorConv_)(pOut, color);
  }

  void ClearColor(uint32_t colorValue, uint32_t colorMask, const Rect &rect) {
    (pfnColorFill_)(pColorDesc_, colorValue, colorMask, rect);
  }

  void ClearDepth(uint32_t depthValue, uint32_t depthMask, const Rect &rect) {
    (pfnDepthStencilFill_)(pDepthStencilDesc_, depthValue, depthMask, rect);
  }

  void ClearStencil(uint32_t clearStencil, uint32_t stencilMask,
                    const Rect &rect) {
    uint32_t value = clearStencil << 16;
    uint32_t mask = stencilMask << 16;
    (pfnDepthStencilFill_)(pDepthStencilDesc_, value, mask, rect);
  }

  void ClearDepthStencil(uint32_t depthValue, uint32_t depthMask,
                         uint32_t stencilValue, uint32_t stencilMask,
                         const Rect &rect) {
    uint32_t value = 0xff000000 | (stencilValue << 16) | depthValue;
    uint32_t mask = 0xff000000 | (stencilMask << 16) | depthMask;
    (pfnDepthStencilFill_)(pDepthStencilDesc_, value, mask, rect);
  }

  GLenum SaveBitmap(const char *filename);

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

  GLint attributes_[ATTRIBUTES_SIZE];

  Format::pfn_convert_to pfnColorConv_;

  PfnColorFill pfnColorFill_;
  PfnColorFill pfnDepthStencilFill_;

  GLSurfaceDesc pColorDesc_;
  GLSurfaceDesc pDepthStencilDesc_;
};
