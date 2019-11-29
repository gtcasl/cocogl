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

void CGLContext::SetError(GLenum error) { m_error = error; }

GLenum CGLContext::GetError() const {
  GLenum error = m_error;
  m_error = GL_NO_ERROR;
  return error;
}

const GLubyte *CGLContext::GetString(GLenum name) {
  switch (name) {
  case GL_VENDOR:
    return reinterpret_cast<const GLubyte*>(EGL_CONFIG_VENDOR);
  case GL_VERSION:
    return reinterpret_cast<const GLubyte*>(EGL_CONFIG_VERSION);
  case GL_RENDERER:
    return reinterpret_cast<const GLubyte*>(EGL_CONFIG_RENDERER);
  case GL_EXTENSIONS:
    return reinterpret_cast<const GLubyte*>(EGL_CONFIG_EXTENSIONS);
  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::GetString() failed, invalid name parameter: %d.\r\n"),
        name);
    return nullptr;
  }
}

void CGLContext::GetPointer(void **ppParams, GLenum pname) {
  ASSERT(ppParams);

  switch (pname) {
  case GL_VERTEX_ARRAY_POINTER:
    ppParams[0] = const_cast<void *>(m_positionArray.pPointer);
    break;

  case GL_NORMAL_ARRAY_POINTER:
    ppParams[0] = const_cast<void *>(m_normalArray.pPointer);
    break;

  case GL_COLOR_ARRAY_POINTER:
    ppParams[0] = const_cast<void *>(m_colorArray.pPointer);
    break;

  case GL_TEXTURE_COORD_ARRAY_POINTER:
    ppParams[0] =
        const_cast<void *>(m_texCoordArrays[m_clientActiveTexture].pPointer);
    break;

  case GL_POINT_SIZE_ARRAY_OES:
    ppParams[0] = const_cast<void *>(m_pointSizeArray.pPointer);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::GetPointer() failed, invalid pname parameter: %d.\r\n"),
        pname);
    return;
  }
}

bool CGLContext::IsEnabled(GLenum cap) {
  switch (cap) {
  case GL_VERTEX_ARRAY:
    return m_vertexStates.Position;

  case GL_NORMAL_ARRAY:
    return m_vertexStates.Normal;

  case GL_COLOR_ARRAY:
    return m_vertexStates.Color;

  case GL_TEXTURE_COORD_ARRAY:
    return (m_vertexStates.TexCoords >> m_clientActiveTexture) & 0x1;

  case GL_FOG:
    return m_caps.Fog;

  case GL_LIGHTING:
    return m_caps.Lighting;

  case GL_TEXTURE_2D:
    return (m_caps.Texture2D >> m_activeTexture) & 0x1;

  case GL_CULL_FACE:
    return m_caps.CullFace;

  case GL_ALPHA_TEST:
    return m_caps.AlphaTest;

  case GL_BLEND:
    return m_caps.Blend;

  case GL_COLOR_LOGIC_OP:
    return m_caps.LogicOp;

  case GL_DITHER:
    return m_caps.Dither;

  case GL_STENCIL_TEST:
    return m_caps.StencilTest;

  case GL_DEPTH_TEST:
    return m_caps.DepthTest;

  case GL_POINT_SMOOTH:
    return m_caps.PointSmooth;

  case GL_POINT_SPRITE_OES:
    return m_caps.PointSprite;

  case GL_LINE_SMOOTH:
    return m_caps.LineSmooth;

  case GL_SCISSOR_TEST:
    return m_caps.ScissorTest;

  case GL_COLOR_MATERIAL:
    return m_caps.ColorMaterial;

  case GL_NORMALIZE:
    return m_caps.Normalize;

  case GL_CLIP_PLANE0:
  case GL_CLIP_PLANE1:
  case GL_CLIP_PLANE2:
  case GL_CLIP_PLANE3:
  case GL_CLIP_PLANE4:
  case GL_CLIP_PLANE5:
    return (m_caps.ClipPlanes >> (cap - GL_CLIP_PLANE0)) & 0x1;

  case GL_LIGHT0:
  case GL_LIGHT1:
  case GL_LIGHT2:
  case GL_LIGHT3:
  case GL_LIGHT4:
  case GL_LIGHT5:
  case GL_LIGHT6:
  case GL_LIGHT7:
    return (m_caps.Lights >> (cap - GL_LIGHT0)) & 0x1;

  case GL_RESCALE_NORMAL:
    return m_caps.RescaleNormal;

  case GL_POLYGON_OFFSET_FILL:
    return m_caps.PolygonOffsetFill;

  case GL_MULTISAMPLE:
    return m_caps.MultiSample;

  case GL_SAMPLE_ALPHA_TO_COVERAGE:
    return m_caps.SampleAlphaToCoverage;

  case GL_SAMPLE_ALPHA_TO_ONE:
    return m_caps.SampleAlphaToOne;

  case GL_SAMPLE_COVERAGE:
    return m_caps.SampleCoverage;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::IsEnabled() failed, invalid cap parameter: %d.\r\n"),
        cap);
    return false;
  }
}

bool CGLContext::IsBuffer(GLuint buffer) {
  return (HANDLE_BUFFER == m_pHandles->GetType(buffer));
}

bool CGLContext::IsTexture(GLuint texture) {
  return (HANDLE_TEXTURE == m_pHandles->GetType(texture));
}
