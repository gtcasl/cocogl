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
#include "vertarray.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "rastdata.hpp"

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
    ASSERT((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    return m_bufferObjects[target - BUFFER_OBJECTS_FIRST];
  }

  void SetBufferObject(GLenum target, CBuffer *pBuffer) {
    if (pBuffer) {
      pBuffer->AddRef();
    }

    CBuffer *pBufCurr = this->GetBufferObject(target);
    if (pBufCurr) {
      pBufCurr->Release();
    }

    ASSERT((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    m_bufferObjects[target - BUFFER_OBJECTS_FIRST] = pBuffer;
  }

  uint32_t GetBufferObjectHandle(GLenum target) const {
    ASSERT((target >= BUFFER_OBJECTS_FIRST) && (target <= BUFFER_OBJECTS_LAST));
    CBuffer *pBuffer = m_bufferObjects[target - BUFFER_OBJECTS_FIRST];
    return pBuffer ? pBuffer->GetHandle() : static_cast<uint32_t>(HANDLE_NONE);
  }

  uint32_t GetBoundTextureHandle() const {
    CTexture *const pTexture = m_texUnits[m_activeTexture].GetTexture();
    ASSERT(pTexture);
    return pTexture->GetHandle();
  }

  CTexture *GetTexture(uint32_t unit) const {
    ASSERT(unit < MAX_TEXTURES);
    return m_texUnits[unit].GetTexture();
  }

  void SetTexture(uint32_t unit, CTexture *pTexture) {
    ASSERT(unit < MAX_TEXTURES);
    m_texUnits[unit].SetTexture(pTexture);
  }

  mutable GLenum m_error;

  const CDevice *m_pCtxShared;
  CGLSurface *m_pSurfDraw;
  CGLSurface *m_pSurfRead;
  CHandleTable *m_pHandles;

  CBuffer *m_bufferObjects[BUFFER_OBJECTS_SIZE];
  CTexture *m_pTexDefault;
  CBuffer *m_pBufDefault;
  CRasterCache *m_pRasterCache;

#ifdef GL_COCOJIT
  CG::CAssembler *m_pCGAssembler;
#endif

  uint32_t m_packAlignment;
  uint32_t m_unpackAlignment;
  SampleCoverage m_sampleCoverage;

  TexUnit m_texUnits[MAX_TEXTURES];
  uint32_t m_activeTexture;
  uint32_t m_clientActiveTexture;

  Rect m_scissor;
  Rect m_viewport;
  DepthRange m_depthRange;

  RASTERSTATES m_rasterStates;
  VertexStates m_vertexStates;
  GLCAPS m_caps;
  HINTS m_hints;
  DirtyFlags m_dirtyFlags;
  DirtyLights m_dirtyLights;
};
