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

void GLContext::clearColor(floatf red, floatf green, floatf blue,
                           floatf alpha) {
  vClearColor_.x = blue;
  vClearColor_.y = green;
  vClearColor_.z = red;
  vClearColor_.w = alpha;

  dirtyFlags_.ClearColor = 1;
}

void GLContext::clearDepth(floatf depth) {
  fClearDepth_ = depth;
  dirtyFlags_.ClearDepth = 1;
}

void GLContext::clearStencil(GLint stencil) {
  clearStencil_ = stencil;
}

void GLContext::setColorMask(GLboolean red, GLboolean green, GLboolean blue,
                             GLboolean alpha) {
  cColorWriteMask_.r = red ? 0xff : 0x0;
  cColorWriteMask_.g = green ? 0xff : 0x0;
  cColorWriteMask_.b = blue ? 0xff : 0x0;
  cColorWriteMask_.a = alpha ? 0xff : 0x0;

  dirtyFlags_.ColorWriteMask = 1;
}

void GLContext::setDepthMask(GLboolean flag) {
  depthWriteMask_ = flag ? 0xffff : 0;
}

void GLContext::setStencilMask(GLuint mask) {
  stencilWriteMask_ = mask;
}

void GLContext::clear(GLbitfield mask) {
  if (mask &
      ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
    __glError(GL_INVALID_VALUE,
              "GLContextc:Clear() failed, invalid mask parameter: %d.\r\n",
              mask);
    return;
  }

  if (dirtyFlags_.ScissorRECT) {
    this->updateScissorRect();
  }

  if (mask & GL_COLOR_BUFFER_BIT) {
    mask &= ~GL_COLOR_BUFFER_BIT;

    if (dirtyFlags_.ClearColor) {
      this->ensureClearColor();
    }

    if (dirtyFlags_.ColorWriteMask) {
      this->ensureColorWriteMask();
    }

    pSurfDraw_->clearColor(clearColor_, rasterData_.ColorWriteMask,
                           scissorRect_);
  }

  if (GL_DEPTH_BUFFER_BIT == mask) {
    if (dirtyFlags_.ClearDepth) {
      this->ensureClearDepth();
    }

    pSurfDraw_->clearDepth(clearDepth_, depthWriteMask_, scissorRect_);
  } else if ((GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) == mask) {
    if (dirtyFlags_.ClearDepth) {
      this->ensureClearDepth();
    }

    pSurfDraw_->clearDepthStencil(clearDepth_, depthWriteMask_, clearStencil_,
                                  stencilWriteMask_, scissorRect_);
  } else if (GL_STENCIL_BUFFER_BIT == mask) {
    pSurfDraw_->clearStencil(clearStencil_, stencilWriteMask_, scissorRect_);
  }

#ifdef FRAME_TRACE
  if (frames_ == (FRAME_TRACE+1)) {
    frameTrace_.save("frame.cgltrace");
  }
#endif

  // increment frames
  ++frames_;
}

void GLContext::drawArrays(GLenum mode, GLint first, GLsizei count) {
  GLenum err;

  if (count < 0) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::drawArrays() failed, invalid count parameter: %d.\r\n",
        count);
    goto L_EXIT;
  }

  // Begin rendering
  err = this->beginRender(mode, first, count);
  if (__glFailed(err)) {
    __glLogError("Rasterizers::beginRender() failed, err = %d.\r\n", err);
    goto L_EXIT;
  }

  // Process input vertices
  this->processVertices(count);

  // Render primitives
  err = this->renderPrimitive(mode, count);
  if (__glFailed(err)) {
    __glError(err, "Rasterizer::renderPrimitive() failed, err = %d.\r\n", err);
    goto L_EXIT;
  }

L_EXIT:

  // End rendering
  this->endRender();
}

void GLContext::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *pIndices) {
  {
    GLenum err;

    if (count < 0) {
      __glError(GL_INVALID_VALUE,
                "GLContext::drawElements() failed, "
                "invalid count parameter: %d.\r\n",
                count);
      goto L_EXIT;
    }

    uint32_t vmin, vmax;

    // Prepare index buffer
    err = this->prepareIndices(type, &pIndices, count, &vmin, &vmax);
    if (__glFailed(err)) {
      __glError(err, "GLContext::prepareIndices() failed, err = %d.\r\n", err);
      goto L_EXIT;
    }

    uint32_t vertexCount = vmax - vmin + 1;

    // Begin rendering
    err = this->beginRender(mode, vmin, vertexCount);
    if (__glFailed(err)) {
      __glLogError("Rasterizer::beginRender() failed, err = %d.\r\n",
                   err);
      goto L_EXIT;
    }

    // Process input vertices
    this->processVertices(vertexCount);

    switch (type) {
    case GL_UNSIGNED_SHORT:
      // Render word indexed primitives
      err = this->renderIndexedPrimitive<uint16_t>(
          mode, reinterpret_cast<const uint16_t *>(pIndices), count, vmin);
      if (__glFailed(err)) {
        __glError(err,
                  "Rasterizer::renderIndexedPrimitive<uint16_t>() failed, "
                  "err = %d.\r\n",
                  err);
      }
      break;

    case GL_UNSIGNED_BYTE:
      // Render byte indexed primitives
      err = this->renderIndexedPrimitive<uint8_t>(
          mode, reinterpret_cast<const uint8_t *>(pIndices), count, vmin);
      if (__glFailed(err)) {
        __glError(err,
                  "Rasterizer::renderIndexedPrimitive<uint8_t>() failed, "
                  "err = %d.\r\n",
                  err);
      }
      break;

    default:
      __glError(GL_INVALID_ENUM,
                "GLContext::drawElements() failed, invalid "
                "type parameter: %d.\r\n",
                type);
    }
  }

L_EXIT:

  // End rendering
  this->endRender();
}

void GLContext::flush() {
  //--
}

void GLContext::finish() {
  //--
}
