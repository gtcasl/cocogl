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
#include "rastdata.hpp"
#include "vertarray.hpp"

class CDevice : public CObject {
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

  CDevice() {}
  ~CDevice() {}

  CBuffer *GetBufferObject(GLenum target) const {
    assert((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    return bufferObjects_[target - BUFFER_OBJECTS_FIRST];
  }

  void SetBufferObject(GLenum target, CBuffer *pBuffer) {
    if (pBuffer) {
      pBuffer->AddRef();
    }

    CBuffer *pBufCurr = this->GetBufferObject(target);
    if (pBufCurr) {
      pBufCurr->Release();
    }

    assert((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    bufferObjects_[target - BUFFER_OBJECTS_FIRST] = pBuffer;
  }

  uint32_t GetBufferObjectHandle(GLenum target) const {
    assert((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    CBuffer *pBuffer = bufferObjects_[target - BUFFER_OBJECTS_FIRST];
    return pBuffer ? pBuffer->GetHandle() : static_cast<uint32_t>(HANDLE_NONE);
  }

  uint32_t GetBoundTextureHandle() const {
    auto pTexture = texUnits_[activeTexture_].GetTexture();
    assert(pTexture);
    return pTexture->GetHandle();
  }

  CTexture *GetTexture(uint32_t unit) const {
    assert(unit < MAX_TEXTURES);
    return texUnits_[unit].GetTexture();
  }

  void SetTexture(uint32_t unit, CTexture *pTexture) {
    assert(unit < MAX_TEXTURES);
    texUnits_[unit].SetTexture(pTexture);
  }

  mutable GLenum error_;

  const CDevice *pCtxShared_;
  CGLSurface *pSurfDraw_;
  CGLSurface *pSurfRead_;
  CHandleTable *handles_;

  CBuffer *bufferObjects_[BUFFER_OBJECTS_SIZE];
  CTexture *pTexDefault_;
  CBuffer *pBufDefault_;
  CRasterCache *pRasterCache_;

#ifdef GL_COCOJIT
  CG::CAssembler *pCGAssembler_;
#endif

  uint32_t packAlignment_;
  uint32_t unpackAlignment_;
  SampleCoverage sampleCoverage_;

  TexUnit texUnits_[MAX_TEXTURES];
  uint32_t activeTexture_;
  uint32_t clientActiveTexture_;

  Rect scissor_;
  Rect viewport_;
  DepthRange depthRange_;

  RASTERSTATES rasterStates_;
  VertexStates vertexStates_;
  GLCAPS caps_;
  HINTS hints_;
  DirtyFlags dirtyFlags_;
  DirtyLights dirtyLights_;
};
