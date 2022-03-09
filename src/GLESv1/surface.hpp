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

class GLSurface : public Object {
public:
  enum {
    ATTRIBUTES_FIRST = GL_SUBPIXEL_BITS,
    ATTRIBUTES_LAST = GL_STENCIL_BITS,
    ATTRIBUTES_SIZE = (ATTRIBUTES_LAST - ATTRIBUTES_FIRST) + 1,
  };

  typedef void (*PfnColorFill)(const GLSurfaceDesc &surfDesc, uint32_t value,
                               uint32_t mask, const Rect &rect);

  static GLenum Create(GLSurface **ppSurface, const GLSurfaceDesc *pColorDesc,
                       const GLSurfaceDesc *pDepthStencilDesc);

  void getColorDesc(GLSurfaceDesc *pSurfDesc) const {
    assert(pSurfDesc);
    *pSurfDesc = colorDesc_;
  }

  void getDepthStencilDesc(GLSurfaceDesc *pSurfDesc) const {
    assert(pSurfDesc);
    *pSurfDesc = depthStencilDesc_;
  }

  void getRect(Rect *pRect) const {
    assert(pRect);
    pRect->left = 0;
    pRect->top = 0;
    pRect->right = colorDesc_.Width;
    pRect->bottom = colorDesc_.Height;
  }

  uint32_t getColorFormat() const {
    return colorDesc_.Format;
  }

  uint32_t getDepthStencilFormat() const {
    return depthStencilDesc_.Format;
  }

  uint32_t getAlignment() const {
    return 1;
  }

  GLint getAttribute(GLint name) const {
    assert(name >= ATTRIBUTES_FIRST && name <= ATTRIBUTES_LAST);
    return attributes_[name - ATTRIBUTES_FIRST];
  }

  void setAttribute(GLint name, GLint value) {
    assert(name >= ATTRIBUTES_FIRST && name <= ATTRIBUTES_LAST);
    attributes_[name - ATTRIBUTES_FIRST] = value;
  }

  uint32_t getWidth() const {
    return colorDesc_.Width;
  }

  uint32_t getHeight() const {
    return colorDesc_.Height;
  }

  const GLSurfaceDesc &getColorDesc() const {
    return colorDesc_;
  }

  const GLSurfaceDesc &getDepthStencilDesc() const {
    return depthStencilDesc_;
  }

  GLenum update(const GLSurfaceDesc *pColorDesc,
                const GLSurfaceDesc *pDepthStencilDesc) {
    return this->initialize(pColorDesc, pDepthStencilDesc);
  }

  void convertColor(void *pOut, const ColorARGB &color) const {
    (pfnColorConv_)(pOut, color);
  }

  void clearColor(uint32_t colorValue, uint32_t colorMask, const Rect &rect) {
    (pfnColorFill_)(colorDesc_, colorValue, colorMask, rect);
  }

  void clearDepth(uint32_t depthValue, uint32_t depthMask, const Rect &rect) {
    (pfnDepthStencilFill_)(depthStencilDesc_, depthValue, depthMask, rect);
  }

  void clearStencil(uint32_t clearStencil, uint32_t stencilMask,
                    const Rect &rect) {
    uint32_t value = clearStencil << 16;
    uint32_t mask = stencilMask << 16;
    (pfnDepthStencilFill_)(depthStencilDesc_, value, mask, rect);
  }

  void clearDepthStencil(uint32_t depthValue, uint32_t depthMask,
                         uint32_t stencilValue, uint32_t stencilMask,
                         const Rect &rect) {
    uint32_t value = 0xff000000 | (stencilValue << 16) | depthValue;
    uint32_t mask = 0xff000000 | (stencilMask << 16) | depthMask;
    (pfnDepthStencilFill_)(depthStencilDesc_, value, mask, rect);
  }

  GLenum saveImage(const char *filename);

private:
  enum {
    SUBPIXEL_BITS = 4,
  };

  GLSurface();

  ~GLSurface();

  GLenum initialize(const GLSurfaceDesc *pColorDesc,
                    const GLSurfaceDesc *pDepthStencilDesc);

  template <typename T>
  static void colorFill(const GLSurfaceDesc &surfDesc, uint32_t value,
                        uint32_t mask, const Rect &rect);

  static void colorFillNoop(const GLSurfaceDesc &surfDesc, uint32_t value,
                            uint32_t mask, const Rect &rect) {
    __unused(surfDesc);
    __unused(value);
    __unused(mask);
    __unused(rect);
  }

  static void colorConvNoop(void *pColorOut, const ColorARGB &color) {
    __unused(pColorOut);
    __unused(color);
  }

  GLint attributes_[ATTRIBUTES_SIZE];

  Format::pfn_convert_to pfnColorConv_;

  PfnColorFill pfnColorFill_;
  PfnColorFill pfnDepthStencilFill_;

  GLSurfaceDesc colorDesc_;
  GLSurfaceDesc depthStencilDesc_;
};
