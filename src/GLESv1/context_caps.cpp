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
              "CGLContext::Hint() failed, invalid mode parameter: %d.\r\n",
              mode);
    return;
  }

  uint32_t value = HintFromEnum(mode);

  switch (target) {
  case GL_PERSPECTIVE_CORRECTION_HINT:
    hints_.Perspective = value;
    break;

  case GL_POINT_SMOOTH_HINT:
    hints_.PointSmooth = value;
    break;

  case GL_LINE_SMOOTH_HINT:
    hints_.LineSmooth = value;
    break;

  case GL_FOG_HINT:
    hints_.Fog = value;
    break;

  case GL_GENERATE_MIPMAP_HINT:
    hints_.GenerateMipmap = value;
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::Hint() failed, invalid target parameter: %d.\r\n",
        target);
    return;
  }
}

void CGLContext::Activate(GLenum cap, bool bValue) {
  switch (cap) {
  case GL_FOG:
    caps_.Fog = bValue;
    break;

  case GL_LIGHTING:
    caps_.Lighting = bValue;
    break;

  case GL_TEXTURE_2D:
    if (bValue) {
      caps_.Texture2D |= (1 << activeTexture_);
    } else {
      caps_.Texture2D &= ~(1 << activeTexture_);
    }

    break;

  case GL_CULL_FACE:
    caps_.CullFace = bValue;
    break;

  case GL_ALPHA_TEST:
    caps_.AlphaTest = bValue;
    break;

  case GL_BLEND:
    caps_.Blend = bValue;
    break;

  case GL_COLOR_LOGIC_OP:
    caps_.LogicOp = bValue;
    break;

  case GL_DITHER:
    caps_.Dither = bValue;
    break;

  case GL_STENCIL_TEST:
    caps_.StencilTest = bValue;
    break;

  case GL_DEPTH_TEST:
    caps_.DepthTest = bValue;
    break;

  case GL_POINT_SMOOTH:
    caps_.PointSmooth = bValue;
    break;

  case GL_POINT_SPRITE_OES:
    caps_.PointSprite = bValue;
    break;

  case GL_LINE_SMOOTH:
    caps_.LineSmooth = bValue;
    break;

  case GL_SCISSOR_TEST:
    caps_.ScissorTest = bValue;
    dirtyFlags_.ScissorRECT = 1;
    break;

  case GL_COLOR_MATERIAL:
    caps_.ColorMaterial = bValue;
    break;

  case GL_NORMALIZE:
    caps_.Normalize = bValue;
    break;

  case GL_CLIP_PLANE0:
  case GL_CLIP_PLANE1:
  case GL_CLIP_PLANE2:
  case GL_CLIP_PLANE3:
  case GL_CLIP_PLANE4:
  case GL_CLIP_PLANE5: {
    uint32_t mask = 1 << (cap - GL_CLIP_PLANE0);
    if (bValue) {
      caps_.ClipPlanes |= mask;
    } else {
      caps_.ClipPlanes &= ~mask;
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
    uint32_t mask = 1 << (cap - GL_LIGHT0);
    if (bValue) {
      caps_.Lights |= mask;
      dirtyLights_.Ambient |= mask;
      dirtyLights_.Diffuse |= mask;
      dirtyLights_.Specular |= mask;
    } else {
      caps_.Lights &= ~mask;
    }

    dirtyFlags_.Lights = 1;
  }

  break;

  case GL_RESCALE_NORMAL:
    caps_.RescaleNormal = bValue;
    dirtyFlags_.ModelViewInvT33 = 1;
    break;

  case GL_POLYGON_OFFSET_FILL:
    caps_.PolygonOffsetFill = bValue;
    break;

  case GL_MULTISAMPLE:
    caps_.MultiSample = bValue;
    break;

  case GL_SAMPLE_ALPHA_TO_COVERAGE:
    caps_.SampleAlphaToCoverage = bValue;
    break;

  case GL_SAMPLE_ALPHA_TO_ONE:
    caps_.SampleAlphaToOne = bValue;
    break;

  case GL_SAMPLE_COVERAGE:
    caps_.SampleCoverage = bValue;
    break;

  // GL Extensions
  case GL_MATRIX_PALETTE_OES:
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::Activate() failed, invalid cap parameter: %d.\r\n",
        cap);
    return;
  }
}
