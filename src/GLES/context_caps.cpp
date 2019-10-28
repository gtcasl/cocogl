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

void CGLContext::Hint(GLenum target, GLenum mode) {
  if ((mode != GL_DONT_CARE) && (mode != GL_FASTEST) && (mode != GL_NICEST)) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::Hint() failed, invalid mode parameter: %d.\r\n"),
              mode);
    return;
  }

  const unsigned value = HintFromEnum(mode);

  switch (target) {
  case GL_PERSPECTIVE_CORRECTION_HINT:
    m_hints.Perspective = value;
    break;

  case GL_POINT_SMOOTH_HINT:
    m_hints.PointSmooth = value;
    break;

  case GL_LINE_SMOOTH_HINT:
    m_hints.LineSmooth = value;
    break;

  case GL_FOG_HINT:
    m_hints.Fog = value;
    break;

  case GL_GENERATE_MIPMAP_HINT:
    m_hints.GenerateMipmap = value;
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::Hint() failed, invalid target parameter: %d.\r\n"),
        target);
    return;
  }
}


void CGLContext::Activate(GLenum cap, bool bValue) {
  switch (cap) {
  case GL_FOG:
    m_caps.Fog = bValue;
    break;

  case GL_LIGHTING:
    m_caps.Lighting = bValue;
    break;

  case GL_TEXTURE_2D:
    if (bValue) {
      m_caps.Texture2D |= (1 << m_activeTexture);
    } else {
      m_caps.Texture2D &= ~(1 << m_activeTexture);
    }

    break;

  case GL_CULL_FACE:
    m_caps.CullFace = bValue;
    break;

  case GL_ALPHA_TEST:
    m_caps.AlphaTest = bValue;
    break;

  case GL_BLEND:
    m_caps.Blend = bValue;
    break;

  case GL_COLOR_LOGIC_OP:
    m_caps.LogicOp = bValue;
    break;

  case GL_DITHER:
    m_caps.Dither = bValue;
    break;

  case GL_STENCIL_TEST:
    m_caps.StencilTest = bValue;
    break;

  case GL_DEPTH_TEST:
    m_caps.DepthTest = bValue;
    break;

  case GL_POINT_SMOOTH:
    m_caps.PointSmooth = bValue;
    break;

  case GL_POINT_SPRITE_OES:
    m_caps.PointSprite = bValue;
    break;

  case GL_LINE_SMOOTH:
    m_caps.LineSmooth = bValue;
    break;

  case GL_SCISSOR_TEST:
    m_caps.ScissorTest = bValue;
    m_dirtyFlags.ScissorRECT = 1;
    break;

  case GL_COLOR_MATERIAL:
    m_caps.ColorMaterial = bValue;
    break;

  case GL_NORMALIZE:
    m_caps.Normalize = bValue;
    break;

  case GL_CLIP_PLANE0:
  case GL_CLIP_PLANE1:
  case GL_CLIP_PLANE2:
  case GL_CLIP_PLANE3:
  case GL_CLIP_PLANE4:
  case GL_CLIP_PLANE5: {
    const unsigned mask = 1 << (cap - GL_CLIP_PLANE0);
    if (bValue) {
      m_caps.ClipPlanes |= mask;
    } else {
      m_caps.ClipPlanes &= ~mask;
    }
  }

  break;

  case GL_LIGHT0:
  case GL_LIGHT1:
  case GL_LIGHT2:
  case GL_LIGHT3:
  case GL_LIGHT4:
  case GL_LIGHT5:
  case GL_LIGHT6:
  case GL_LIGHT7: {
    const unsigned mask = 1 << (cap - GL_LIGHT0);
    if (bValue) {
      m_caps.Lights |= mask;
      m_dirtyLights.Ambient |= mask;
      m_dirtyLights.Diffuse |= mask;
      m_dirtyLights.Specular |= mask;
    } else {
      m_caps.Lights &= ~mask;
    }

    m_dirtyFlags.Lights = 1;
  }

  break;

  case GL_RESCALE_NORMAL:
    m_caps.RescaleNormal = bValue;
    m_dirtyFlags.ModelViewInvT33 = 1;
    break;

  case GL_POLYGON_OFFSET_FILL:
    m_caps.PolygonOffsetFill = bValue;
    break;

  case GL_MULTISAMPLE:
    m_caps.MultiSample = bValue;
    break;

  case GL_SAMPLE_ALPHA_TO_COVERAGE:
    m_caps.SampleAlphaToCoverage = bValue;
    break;

  case GL_SAMPLE_ALPHA_TO_ONE:
    m_caps.SampleAlphaToOne = bValue;
    break;

  case GL_SAMPLE_COVERAGE:
    m_caps.SampleCoverage = bValue;
    break;

  // GL Extensions
  case GL_MATRIX_PALETTE_OES:
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::Activate() failed, invalid cap parameter: %d.\r\n"),
        cap);
    return;
  }
}
