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
#include "rastri.inl"

#ifdef GL_PROFILE


void IRasterOp::StartProfile(unsigned numPixels) {
  m_profile.Invocations += 1;
  m_profile.DrawnPixels += numPixels;
}


void IRasterOp::EndProfile(float elapsedTime) {
  m_profile.RenderTime += elapsedTime;
}


void IRasterOp::LogProfile(const RASTERID &rasterID) {
  TCHAR szBuff[512];

  const float fRenderTime = m_profile.RenderTime;
  const float fMPs = m_profile.DrawnPixels / fRenderTime;

  _sntprintf(szBuff, __countof(szBuff), _T("Profile_PS(%d,%d,%d,%d): ")
                                        _T("Calls=%ld, Pixels=%ld, Time=%.6f ")
                                        _T("ms, MPs=%.6f - "),
             rasterID.Flags.Value, rasterID.States.Value,
             rasterID.Textures[0].Value, rasterID.Textures[1].Value,
             m_profile.Invocations, m_profile.DrawnPixels, fRenderTime, fMPs);

  ::OutputDebugString(szBuff);

  rasterID.Flags.DebugPrint();

  ::OutputDebugString(_T("\r\n"));
}

#endif

//////////////////////////////////////////////////////////////////////////////


GLenum CRasterizer::SetupRasterStates(GLenum mode) {
  if (NULL == m_rasterData.pColorBits) {
    __glLogError(_T("CRasterizer::SetupRasterStates() failed, missing color ")
                 _T("buffer.\r\n"));
    return GL_INVALID_OPERATION;
  }

  if (m_dirtyFlags.ScissorRECT) {
    this->UpdateScissorRect();
  }

  m_cullStates.bCullFaceEnabled = m_caps.CullFace ? true : false;

  RASTERID rasterID;

  rasterID.Flags.ColorFormat = m_rasterData.ColorFormat;

  if (m_cColorWriteMask.value != 0xffffffff) {
    rasterID.Flags.ColorWriteMask = 1;

    if (m_dirtyFlags.ColorWriteMask) {
      this->EnsureColorWriteMask();
    }
  }

  if (m_caps.Texture2D) {
    unsigned n = 0;

    for (unsigned i = 0, mask = m_caps.Texture2D; mask; mask >>= 1, ++i) {
      if (mask & 0x1) {
        if (m_texUnits[i].Prepare(&m_rasterData.Samplers[n],
                                  &rasterID.Textures[n])) {
          switch (mode) {
          default:
            __no_default;

          case GL_TRIANGLES:
          case GL_TRIANGLE_STRIP:
          case GL_TRIANGLE_FAN: {
            // Compute texture mip flags
            const unsigned state = rasterID.Textures[n].Value;
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
      if ((ENVMODE_MODULATE == m_texUnits[0].EnvMode) &&
          m_dirtyFlags.DefaultColor && (0 == m_caps.Lighting) &&
          (0 == m_vertexStates.Color)) {
        rasterID.Textures[0].EnvMode = ENVMODE_REPLACE;
      }
    }
  }

  if (!(m_caps.Texture2D & TEXTURE_UNIT0) // tex0 is enabled
      ||
      !((ENVMODE_REPLACE == rasterID.Textures[0].EnvMode) // tex0 replaces color
        &&
        (Format::GetInfo(rasterID.Textures[0].Format).Alpha // Tex0 has alpha
         ||
         !(Format::GetInfo(rasterID.Flags.ColorFormat).Alpha // Alpha not used
           || m_caps.AlphaTest || m_caps.Blend)))) {
    rasterID.Flags.Color = 1;
    rasterID.Flags.InterpolateColor = (m_caps.ShadeModel) ? 1 : 0;
    rasterID.Flags.InterpolateAlpha = rasterID.Flags.InterpolateColor;
  }

  if (m_caps.DepthTest &&
      Format::GetInfo(m_rasterData.DepthStencilFormat).Depth) {
    rasterID.Flags.DepthTest = 1;
    rasterID.Flags.DepthWrite = m_depthWriteMask;
    rasterID.Flags.DepthStencilFormat = m_rasterData.DepthStencilFormat;
    rasterID.States.DepthFunc = m_rasterStates.DepthFunc;
  }

  if (m_caps.StencilTest &&
      Format::GetInfo(m_rasterData.DepthStencilFormat).Stencil) {
    rasterID.Flags.StencilTest = 1;
    rasterID.Flags.StencilWrite = m_stencilWriteMask ? 1 : 0;
    rasterID.Flags.DepthStencilFormat = m_rasterData.DepthStencilFormat;

    rasterID.States.StencilFunc = m_rasterStates.StencilFunc;
    rasterID.States.StencilFail = m_rasterStates.StencilFail;
    rasterID.States.StencilZFail = m_rasterStates.StencilZFail;
    rasterID.States.StencilZPass = m_rasterStates.StencilZPass;

    m_rasterData.StencilWriteMask = static_cast<uint8_t>(m_stencilWriteMask);
  }

  if (m_caps.AlphaTest) {
    rasterID.Flags.AlphaTest = 1;
    rasterID.States.AlphaFunc = m_rasterStates.AlphaFunc;
  }

  if (m_caps.Fog) {
    rasterID.Flags.Fog = m_caps.Fog;

    if (m_dirtyFlags.FogColor) {
      this->EnsureFogColor();
    }
  }

  if (m_caps.Blend) {
    rasterID.Flags.Blend = 1;
    rasterID.States.BlendSrc = m_rasterStates.BlendSrc;
    rasterID.States.BlendDst = m_rasterStates.BlendDst;
  }

  if (m_caps.LogicOp && LOGICOP_COPY != m_rasterStates.LogicFunc) {
    rasterID.Flags.LogicOp = 1;
    rasterID.States.LogicFunc = m_rasterStates.LogicFunc;
  }

  m_rasterID = rasterID;

  ASSERT(0 == memcmp(&m_rasterID, &rasterID, sizeof(RASTERID)));

  return GL_NO_ERROR;
}


void CRasterizer::UpdateScissorRect() {
  if (m_caps.ScissorTest) {
    Rect rect;
    m_pSurfDraw->GetRect(&rect);
    ::IntersectRect(&m_scissorRect, &m_scissor, &rect);
  } else {
    m_pSurfDraw->GetRect(&m_scissorRect);
  }

  m_dirtyFlags.ScissorRECT = 0;
}


bool CRasterizer::GenerateRasterOp() {
  GLenum err;

  std::lock_guard<std::mutex> lock(m_pRasterCache->GetCS());

  IRasterOp *pRasterOp = NULL;
  if (!m_pRasterCache->Lookup(&pRasterOp, m_rasterID)) {
#ifndef NDEBUG
    DbgPrintf(3, _T("RASTERID: %d,%d,%d,%d\r\n"), m_rasterID.Flags.Value,
              m_rasterID.States.Value, m_rasterID.Textures[0].Value,
              m_rasterID.Textures[1].Value);
#endif
    err = COptimizedRasterOp::Create(&pRasterOp, m_rasterID);
    if (__glFailed(err)) {
      if (__GL_NO_DATA == err) {
#ifdef GL_COCOJIT
        err = CJITRasterOp::Create(&pRasterOp, m_pCGAssembler, m_rasterID);
        if (__glFailed(err)) {
          __glLogError(_T("CJITRasterOp::Create() failed, err = %d.\r\n"), err);
          return false;
        }
#else
        err = CGenericRasterOp::Create(&pRasterOp, m_rasterID);
        if (__glFailed(err)) {
          __glLogError(_T("CGenericRasterOp::Create() failed, err = %d.\r\n"),
                       err);
          return false;
        }
#endif
#ifndef NDEBUG
        // Add the unoptimized scanline to the tracking list
        m_pRasterCache->TrackSlowRasterID(m_rasterID);
#endif
      } else {
        __glLogError(_T("COptimizedRasterOp::Create() failed, err = %d.\r\n"),
                     err);
        return false;
      }
    }

    err = GLERROR_FROM_HRESULT(m_pRasterCache->Insert(m_rasterID, pRasterOp));
    if (__glFailed(err)) {
      __safeRelease(pRasterOp);
      __glLogError(_T("CRasterCache::Insert() failed err = %d.\r\n"), err);
      return false;
    }
  }

  // Set the new rasterop
  pRasterOp->AddRef();
  __safeRelease(m_rasterData.pRasterOp);
  m_rasterData.pRasterOp = pRasterOp;

  return true;
}


void CRasterizer::PostRender() {
  // Free the rasterop
  __safeRelease(m_rasterData.pRasterOp);
}


GLenum CRasterizer::RenderPrimitive(GLenum mode, unsigned count) {
  switch (mode) {
  case GL_TRIANGLES:
    for (unsigned i = 2; i < count; i += 3) {
      this->DrawTriangle(i - 2, i - 1, i);
    }
    break;

  case GL_TRIANGLE_STRIP:
    for (unsigned i = 2, p = 0; i < count; ++i, p ^= 1) {
      this->DrawTriangle(i - 2 + p, i - 1 - p, i);
    }
    break;

  case GL_TRIANGLE_FAN:
    for (unsigned i = 2; i < count; ++i) {
      this->DrawTriangle(0, i - 1, i);
    }
    break;

  case GL_LINES:
    for (unsigned i = 1; i < count; i += 2) {
      this->DrawLine(i - 1, i);
    }
    break;

  case GL_LINE_STRIP:
    for (unsigned i = 1; i < count; ++i) {
      this->DrawLine(i - 1, i);
    }
    break;

  case GL_LINE_LOOP:
    if (count > 1) {
      for (unsigned i = 1; i < count; ++i) {
        this->DrawLine(i - 1, i);
      }

      this->DrawLine(count - 1, 0);
    }
    break;

  case GL_POINTS:
    for (unsigned i = 0; i < count; ++i) {
      this->DrawPoint(i);
    }
    break;

  default:
    __glLogError(_T("CGLContext::RenderPrimitive() failed, invalid mode ")
                 _T("parameter: %d.\r\n"),
                 mode);
    return GL_INVALID_ENUM;
  }

  return GL_NO_ERROR;
}
