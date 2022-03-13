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
#include "raster.hpp"
#include "rasterop.hpp"
#include "rasterop_gen.hpp"
#include "rasterop_opt.hpp"

#ifdef COCOGL_RASTER_PROFILE

void IRasterOp::startProfile(uint32_t numPixels) {
  profile_.Invocations += 1;
  profile_.DrawnPixels += numPixels;
}

void IRasterOp::endProfile(float elapsedTime) {
  profile_.RenderTime += elapsedTime;
}

void IRasterOp::logProfile(const RASTERID &rasterID) {

  float fRenderTime = profile_.RenderTime;
  float fMPs = profile_.DrawnPixels / fRenderTime;

  DbgPrintf(1,
            "Profile_PS(%d,%d,%d,%d): Calls=%ld, Pixels=%ld, Time=%.6f ms, MPs=%.6f - ",
            rasterID.Flags.Value, rasterID.States.Value,
            rasterID.Textures[0].Value, rasterID.Textures[1].Value,
            profile_.Invocations, profile_.DrawnPixels, fRenderTime, fMPs);
  rasterID.Flags.DebugPrint();
  DbgPrintf(1, "\r\n");
}

#endif

//////////////////////////////////////////////////////////////////////////////

GLenum Rasterizer::setupRasterStates(GLenum mode) {
  if (nullptr == rasterData_.pColorBits) {
    __glLogError("Rasterizer::setupRasterStates() failed, missing color "
                 "buffer.\r\n");
    return GL_INVALID_OPERATION;
  }

  if (dirtyFlags_.ScissorRECT) {
    this->updateScissorRect();
  }

  cullStates_.bCullFaceEnabled = caps_.CullFace ? true : false;

  RASTERID rasterID(0, 0, 0, 0);

  rasterID.Flags.ColorFormat = rasterData_.ColorFormat;

  if (cColorWriteMask_.value != 0xffffffff) {
    rasterID.Flags.ColorWriteMask = 1;

    if (dirtyFlags_.ColorWriteMask) {
      this->ensureColorWriteMask();
    }
  }

  if (caps_.Texture2D) {
    uint32_t n = 0;

    for (uint32_t i = 0, mask = caps_.Texture2D; mask; mask >>= 1, ++i) {
      if (mask & 0x1) {
        if (texUnits_[i].prepare(&rasterData_.Samplers[n],
                                 &rasterID.Textures[n])) {
          switch (mode) {
          default:
            __no_default;

          case GL_TRIANGLES:
          case GL_TRIANGLE_STRIP:
          case GL_TRIANGLE_FAN: {
            // Compute texture mip flags
            uint32_t state = rasterID.Textures[n].Value;
            rasterID.Flags.TextureMips |=
                ((FILTER_NONE !=
                  __get_bitfield(state, TEXTURESTATES::MIPFILTER)) ||
                 (__get_bitfield(state, TEXTURESTATES::MINFILTER) !=
                  __get_bitfield(state, TEXTURESTATES::MAGFILTER)))
                    ? (1 << n)
                    : 0;
          } break;

          case GL_LINES:
          case GL_LINE_STRIP:
          case GL_LINE_LOOP:
          case GL_POINTS:
            // Turn off trilinear filtering for lines and points
            rasterID.Textures[n].MipFilter = FILTER_NONE;
            rasterID.Textures[n].MinFilter = rasterID.Textures[n].MagFilter;
            break;
          }

          ++n;
        }
      }
    }

    if (n) {
      rasterID.Flags.NumTextures = n;

      // Turn off MODULATE envMode if not needed
      if ((ENVMODE_MODULATE == texUnits_[0].EnvMode) &&
          dirtyFlags_.DefaultColor && (0 == caps_.Lighting) &&
          (0 == vertexStates_.Color)) {
        rasterID.Textures[0].EnvMode = ENVMODE_REPLACE;
      }
    }
  }

  if (!(caps_.Texture2D & TEXTURE_UNIT0) // tex0 is enabled
      ||
      !((ENVMODE_REPLACE == rasterID.Textures[0].EnvMode) // tex0 replaces color
        &&
        (Format::GetInfo((ePixelFormat)rasterID.Textures[0].Format).Alpha // Tex0 has alpha
         ||
         !(Format::GetInfo((ePixelFormat)rasterID.Flags.ColorFormat).Alpha // Alpha not used
           || caps_.AlphaTest || caps_.Blend)))) {
    rasterID.Flags.Color = 1;
    rasterID.Flags.InterpolateColor = (caps_.ShadeModel) ? 1 : 0;
    rasterID.Flags.InterpolateAlpha = rasterID.Flags.InterpolateColor;
  }

  if (caps_.DepthTest &&
      Format::GetInfo((ePixelFormat)rasterData_.DepthStencilFormat).Depth) {
    rasterID.Flags.DepthTest = 1;
    rasterID.Flags.DepthWrite = depthWriteMask_;
    rasterID.Flags.DepthStencilFormat = rasterData_.DepthStencilFormat;
    rasterID.States.DepthFunc = rasterStates_.DepthFunc;
  }

  if (caps_.StencilTest &&
      Format::GetInfo((ePixelFormat)rasterData_.DepthStencilFormat).Stencil) {
    rasterID.Flags.StencilTest = 1;
    rasterID.Flags.StencilWrite = stencilWriteMask_ ? 1 : 0;
    rasterID.Flags.DepthStencilFormat = rasterData_.DepthStencilFormat;

    rasterID.States.StencilFunc = rasterStates_.StencilFunc;
    rasterID.States.StencilFail = rasterStates_.StencilFail;
    rasterID.States.StencilZFail = rasterStates_.StencilZFail;
    rasterID.States.StencilZPass = rasterStates_.StencilZPass;

    rasterData_.StencilWriteMask = static_cast<uint8_t>(stencilWriteMask_);
  }

  if (caps_.AlphaTest) {
    rasterID.Flags.AlphaTest = 1;
    rasterID.States.AlphaFunc = rasterStates_.AlphaFunc;
  }

  if (caps_.Fog) {
    rasterID.Flags.Fog = caps_.Fog;

    if (dirtyFlags_.FogColor) {
      this->ensureFogColor();
    }
  }

  if (caps_.Blend) {
    rasterID.Flags.Blend = 1;
    rasterID.States.BlendSrc = rasterStates_.BlendSrc;
    rasterID.States.BlendDst = rasterStates_.BlendDst;
  }

  if (caps_.LogicOp && LOGICOP_COPY != rasterStates_.LogicFunc) {
    rasterID.Flags.LogicOp = 1;
    rasterID.States.LogicFunc = rasterStates_.LogicFunc;
  }

  rasterID_ = rasterID;

  assert(0 == memcmp(&rasterID_, &rasterID, sizeof(RASTERID)));

  return GL_NO_ERROR;
}

void Rasterizer::updateScissorRect() {
  if (caps_.ScissorTest) {
    Rect rect;
    pSurfDraw_->getRect(&rect);
    Intersect(&scissorRect_, &scissor_, &rect);
  } else {
    pSurfDraw_->getRect(&scissorRect_);
  }

  dirtyFlags_.ScissorRECT = 0;
}

bool Rasterizer::generateRasterOp() {
  GLenum err;

  IRasterOp *pRasterOp = nullptr;
  if (!pRasterCache_->lookup(rasterID_, &pRasterOp)) {
#ifndef NDEBUG
    DbgPrintf(3, "RASTERID: %d,%d,%d,%d\r\n", rasterID_.Flags.Value,
              rasterID_.States.Value, rasterID_.Textures[0].Value,
              rasterID_.Textures[1].Value);
#endif
    err = OptimizedRasterOp::Create(&pRasterOp, rasterID_);
    if (__glFailed(err)) {
      if (__GL_NO_DATA == err) {
        err = GenericRasterOp::Create(&pRasterOp, rasterID_);
        if (__glFailed(err)) {
          __glLogError("GenericRasterOp::Create() failed, err = %d.\r\n", err);
          return false;
        }
#ifndef NDEBUG
        // Add the unoptimized scanline to the tracking list
        pRasterCache_->trackSlowRasterID(rasterID_);
#endif
      } else {
        __glLogError("OptimizedRasterOp::Create() failed, err = %d.\r\n", err);
        return false;
      }
    }

    pRasterCache_->insert(rasterID_, pRasterOp);
  }

  // Set the new rasterop
  pRasterOp->addRef();
  __safeRelease(rasterData_.pRasterOp);
  rasterData_.pRasterOp = pRasterOp;

  return true;
}

GLenum Rasterizer::beginRender(GLenum mode, int first, uint32_t count) {
  GLenum err;

  // Setup rasterizer states
  err = this->setupRasterStates(mode);
  if (__glFailed(err)) {
    __glLogError("Rasterizers::setupRasterStates() failed, err = %d.\r\n", err);
    return err;
  }

  // Setup TNL states
  err = this->setupTNLStates(mode, first, count);
  if (__glFailed(err)) {
    __glLogError("TNL::setupTNLStates() failed, err = %d.\r\n", err);
    return err;
  }

#ifdef FRAME_TRACE
  if (frames_ == (FRAME_TRACE+1)) {
    frameTrace_.beginTrace(rasterID_, rasterData_, viewport_);
  }
#endif

  return GL_NO_ERROR;
}

void Rasterizer::endRender() {
  
#ifdef FRAME_TRACE
  if (frames_ == (FRAME_TRACE+1)) {
    frameTrace_.endTrace();
  }
#endif

  // Free the rasterop
  __safeRelease(rasterData_.pRasterOp);  
}

GLenum Rasterizer::renderPrimitive(GLenum mode, uint32_t count) {
  switch (mode) {
  case GL_TRIANGLES:
    for (uint32_t i = 2; i < count; i += 3) {
      this->drawTriangle(i - 2, i - 1, i);
    }
    break;

  case GL_TRIANGLE_STRIP:
    for (uint32_t i = 2, p = 0; i < count; ++i, p ^= 1) {
      this->drawTriangle(i - 2 + p, i - 1 - p, i);
    }
    break;

  case GL_TRIANGLE_FAN:
    for (uint32_t i = 2; i < count; ++i) {
      this->drawTriangle(0, i - 1, i);
    }
    break;

  case GL_LINES:
    for (uint32_t i = 1; i < count; i += 2) {
      this->drawLine(i - 1, i);
    }
    break;

  case GL_LINE_STRIP:
    for (uint32_t i = 1; i < count; ++i) {
      this->drawLine(i - 1, i);
    }
    break;

  case GL_LINE_LOOP:
    if (count > 1) {
      for (uint32_t i = 1; i < count; ++i) {
        this->drawLine(i - 1, i);
      }

      this->drawLine(count - 1, 0);
    }
    break;

  case GL_POINTS:
    for (uint32_t i = 0; i < count; ++i) {
      this->drawPoint(i);
    }
    break;

  default:
    __glLogError("GLContext::renderPrimitive() failed, invalid mode "
                 "parameter: %d.\r\n",
                 mode);
    return GL_INVALID_ENUM;
  }

  return GL_NO_ERROR;
}
