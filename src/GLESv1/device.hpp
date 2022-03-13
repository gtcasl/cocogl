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

#include "buffer.hpp"
#include "light.hpp"
#include "matstack.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "vertarray.hpp"
#include "rastdata.hpp"
#include "frametrace.hpp"

class Device : public Object {
public:

  template <typename T>
  inline T getObject(uint32_t handle) const {
    return reinterpret_cast<T>(handles_.getObject(handle));
  }

  GLenum registerObject(uint32_t *phandle, void *pObject, uint8_t type) {
    return GLERROR_FROM_HRESULT(handles_.insert(phandle, pObject, type));
  }

  template <typename T>
  inline T unregisterObject(void *handle) {
    return reinterpret_cast<T>(
        handles_.deleteHandle(reinterpret_cast<intptr_t>(handle)));
  }

protected:
  enum {
    BUFFER_OBJECTS_FIRST = GL_ARRAY_BUFFER,
    BUFFER_OBJECTS_LAST = GL_ELEMENT_ARRAY_BUFFER,
    BUFFER_OBJECTS_SIZE = (BUFFER_OBJECTS_LAST - BUFFER_OBJECTS_FIRST) + 1,
    MODELVIEW_STACK_SIZE = 16,
    PROJECTION_STACK_SIZE = 2,
    TEXTURE_STACK_SIZE = 2,
    MATERIALS_SIZE = 2,
    CLIP_BUFFER_SIZE = CLIP_PLANES + 3,
  };

  Device() : frames_(0) {}
  ~Device() {}

  GLBuffer *getBufferObject(GLenum target) const {
    assert((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    return bufferObjects_[target - BUFFER_OBJECTS_FIRST];
  }

  void setBufferObject(GLenum target, GLBuffer *pBuffer) {
    if (pBuffer) {
      pBuffer->addRef();
    }

    GLBuffer *pBufCurr = this->getBufferObject(target);
    if (pBufCurr) {
      pBufCurr->release();
    }

    assert((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    bufferObjects_[target - BUFFER_OBJECTS_FIRST] = pBuffer;
  }

  uint32_t getBufferObjectHandle(GLenum target) const {
    assert((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    GLBuffer *pBuffer = bufferObjects_[target - BUFFER_OBJECTS_FIRST];
    return pBuffer ? pBuffer->getHandle() : static_cast<uint32_t>(HANDLE_NONE);
  }

  uint32_t getBoundTextureHandle() const {
    auto pTexture = texUnits_[activeTexture_].getTexture();
    assert(pTexture);
    return pTexture->getHandle();
  }

  Texture *getTexture(uint32_t unit) const {
    assert(unit < MAX_TEXTURES);
    return texUnits_[unit].getTexture();
  }

  void setTexture(uint32_t unit, Texture *pTexture) {
    assert(unit < MAX_TEXTURES);
    texUnits_[unit].setTexture(pTexture);
  }

  mutable GLenum error_;

  HandleTable handles_;

  const Device *pCtxShared_;
  GLSurface *pSurfDraw_;
  GLSurface *pSurfRead_;
  
  GLBuffer *bufferObjects_[BUFFER_OBJECTS_SIZE];
  Texture *pTexDefault_;
  GLBuffer *pBufDefault_;
  RasterCache *pRasterCache_;

  uint32_t packAlignment_;
  uint32_t unpackAlignment_;
  SampleCoverage sampleCoverage_;

  TexUnit texUnits_[MAX_TEXTURES];
  uint32_t activeTexture_;
  uint32_t clientActiveTexture_;

  Rect scissor_;
  Viewport viewport_;

  RASTERSTATES rasterStates_;
  VertexStates vertexStates_;
  GLCAPS caps_;
  HINTS hints_;
  DirtyFlags dirtyFlags_;
  DirtyLights dirtyLights_;

  uint64_t frames_;

#ifdef FRAME_TRACE
  FrameTrace frameTrace_;
#endif
};
