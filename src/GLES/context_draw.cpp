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
#include "stdafx.h"
#include "context.hpp"
#include "raster.inl"

void CGLContext::ClearColor(floatf red, floatf green, floatf blue,
                            floatf alpha) {
  m_vClearColor.x = blue;
  m_vClearColor.y = green;
  m_vClearColor.z = red;
  m_vClearColor.w = alpha;

  m_dirtyFlags.ClearColor = 1;
}


void CGLContext::ClearDepth(floatf depth) {
  m_fClearDepth = depth;

  m_dirtyFlags.ClearDepth = 1;
}


void CGLContext::ClearStencil(GLint stencil) { m_clearStencil = stencil; }


void CGLContext::ColorMask(GLboolean red, GLboolean green, GLboolean blue,
                           GLboolean alpha) {
  m_cColorWriteMask.r = red ? 0xff : 0x0;
  m_cColorWriteMask.g = green ? 0xff : 0x0;
  m_cColorWriteMask.b = blue ? 0xff : 0x0;
  m_cColorWriteMask.a = alpha ? 0xff : 0x0;

  m_dirtyFlags.ColorWriteMask = 1;
}


void CGLContext::DepthMask(GLboolean flag) {
  m_depthWriteMask = flag ? 0xffff : 0;
}


void CGLContext::StencilMask(GLuint mask) { m_stencilWriteMask = mask; }


void CGLContext::Clear(GLbitfield mask) {
  if (mask &
      ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::Clear() failed, invalid mask parameter: %d.\r\n"),
              mask);
    return;
  }

  if (m_dirtyFlags.ScissorRECT) {
    this->UpdateScissorRect();
  }

  if (mask & GL_COLOR_BUFFER_BIT) {
    mask &= ~GL_COLOR_BUFFER_BIT;

    if (m_dirtyFlags.ClearColor) {
      this->EnsureClearColor();
    }

    if (m_dirtyFlags.ColorWriteMask) {
      this->EnsureColorWriteMask();
    }

    m_pSurfDraw->ClearColor(m_clearColor, m_rasterData.ColorWriteMask,
                            m_scissorRect);
  }

  if (GL_DEPTH_BUFFER_BIT == mask) {
    if (m_dirtyFlags.ClearDepth) {
      this->EnsureClearDepth();
    }

    m_pSurfDraw->ClearDepth(m_clearDepth, m_depthWriteMask, m_scissorRect);
  } else if ((GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) == mask) {
    if (m_dirtyFlags.ClearDepth) {
      this->EnsureClearDepth();
    }

    m_pSurfDraw->ClearDepthStencil(m_clearDepth, m_depthWriteMask,
                                   m_clearStencil, m_stencilWriteMask,
                                   m_scissorRect);
  } else if (GL_STENCIL_BUFFER_BIT == mask) {
    m_pSurfDraw->ClearStencil(m_clearStencil, m_stencilWriteMask,
                              m_scissorRect);
  }
}


void CGLContext::DrawArrays(GLenum mode, GLint first, GLsizei count) {
  GLenum err;

  if (count < 0) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::DrawArrays() failed, invalid count parameter: %d.\r\n"),
        count);
    goto L_EXIT;
  }

  // Setup the raster states
  err = this->SetupRasterStates(mode);
  if (__glFailed(err)) {
    __glLogError(_T("CRasterizer::SetupRasterStates() failed, err = %d.\r\n"),
                 err);
    goto L_EXIT;
  }

  // Setup TNL states
  err = this->SetupTNLStates(mode, first, count);
  if (__glFailed(err)) {
    __glLogError(_T("CTNL::SetupTNLStates() failed, err = %d.\r\n"), err);
    goto L_EXIT;
  }

  // Process input vertices
  this->ProcessVertices(count);

  // Render primitives
  err = this->RenderPrimitive(mode, count);
  if (__glFailed(err)) {
    __glError(err, _T("CRasterizer::RenderPrimitive() failed, err = %d.\r\n"),
              err);
    goto L_EXIT;
  }

L_EXIT:

  // CLose rendering
  this->PostRender();
}


void CGLContext::DrawElements(GLenum mode, GLsizei count, GLenum type,
                              const GLvoid *pIndices) {
  {
    GLenum err;

    if (count < 0) {
      __glError(GL_INVALID_VALUE, _T("CGLContext::DrawElements() failed, ")
                                  _T("invalid count parameter: %d.\r\n"),
                count);
      goto L_EXIT;
    }

    // Setup the raster states
    err = this->SetupRasterStates(mode);
    if (__glFailed(err)) {
      __glLogError(_T("CRasterizer::SetupRasterStates() failed, err = %d.\r\n"),
                  err);
      goto L_EXIT;
    }

    unsigned vmin, vmax;

    // Prepare index buffer
    err = this->PrepareIndices(type, &pIndices, count, &vmin, &vmax);
    if (__glFailed(err)) {
      __glError(err, _T("CGLContext::PrepareIndices() failed, err = %d.\r\n"),
                err);
      goto L_EXIT;
    }

    const unsigned vertexCount = vmax - vmin + 1;

    // Setup TNL states
    err = this->SetupTNLStates(mode, vmin, vertexCount);
    if (__glFailed(err)) {
      __glLogError(_T("CTNL::SetupTNLStates() failed, err = %d.\r\n"), err);
      goto L_EXIT;
    }

    // Process input vertices
    this->ProcessVertices(vertexCount);

    switch (type) {
    case GL_UNSIGNED_SHORT:
      // Render word indexed primitives
      err = this->TRenderIndexedPrimitive<uint16_t>(
          mode, reinterpret_cast<const uint16_t *>(pIndices), count, vmin);
      if (__glFailed(err)) {
        __glError(err, _T("CRasterizer::TRenderIndexedPrimitive<uint16_t>() failed, ")
                      _T("err = %d.\r\n"),
                  err);
      }
      break;

    case GL_UNSIGNED_BYTE:
      // Render byte indexed primitives
      err = this->TRenderIndexedPrimitive<uint8_t>(
          mode, reinterpret_cast<const uint8_t *>(pIndices), count, vmin);
      if (__glFailed(err)) {
        __glError(err, _T("CRasterizer::TRenderIndexedPrimitive<uint8_t>() failed, ")
                      _T("err = %d.\r\n"),
                  err);
      }
      break;

    default:
      __glError(GL_INVALID_ENUM, _T("CGLContext::DrawElements() failed, invalid ")
                                _T("type parameter: %d.\r\n"),
                type);
    }
  }

L_EXIT:

  // CLose rendering
  this->PostRender();
}


void CGLContext::Flush() {
  //--
}


void CGLContext::Finish() {
  //--
}
