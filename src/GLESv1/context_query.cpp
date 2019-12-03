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

void GLContext::setError(GLenum error) { error_ = error; }

GLenum GLContext::getError() const {
  GLenum error = error_;
  error_ = GL_NO_ERROR;
  return error;
}

const GLubyte *GLContext::getString(GLenum name) {
  switch (name) {
  case GL_VENDOR:
    return reinterpret_cast<const GLubyte *>(EGL_CONFIG_VENDOR);
  case GL_VERSION:
    return reinterpret_cast<const GLubyte *>(EGL_CONFIG_VERSION);
  case GL_RENDERER:
    return reinterpret_cast<const GLubyte *>(EGL_CONFIG_RENDERER);
  case GL_EXTENSIONS:
    return reinterpret_cast<const GLubyte *>(EGL_CONFIG_EXTENSIONS);
  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::getString() failed, invalid name parameter: %d.\r\n",
              name);
    return nullptr;
  }
}

void GLContext::getPointer(void **ppParams, GLenum pname) {
  assert(ppParams);

  switch (pname) {
  case GL_VERTEX_ARRAY_POINTER:
    ppParams[0] = const_cast<void *>(positionArray_.pPointer);
    break;

  case GL_NORMAL_ARRAY_POINTER:
    ppParams[0] = const_cast<void *>(normalArray_.pPointer);
    break;

  case GL_COLOR_ARRAY_POINTER:
    ppParams[0] = const_cast<void *>(colorArray_.pPointer);
    break;

  case GL_TEXTURE_COORD_ARRAY_POINTER:
    ppParams[0] =
        const_cast<void *>(texCoordArrays_[clientActiveTexture_].pPointer);
    break;

  case GL_POINT_SIZE_ARRAY_OES:
    ppParams[0] = const_cast<void *>(pointSizeArray_.pPointer);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "GLContext::getPointer() failed, invalid pname parameter: %d.\r\n",
        pname);
    return;
  }
}

bool GLContext::isEnabled(GLenum cap) {
  switch (cap) {
  case GL_VERTEX_ARRAY:
    return vertexStates_.Position;

  case GL_NORMAL_ARRAY:
    return vertexStates_.Normal;

  case GL_COLOR_ARRAY:
    return vertexStates_.Color;

  case GL_TEXTURE_COORD_ARRAY:
    return (vertexStates_.TexCoords >> clientActiveTexture_) & 0x1;

  case GL_FOG:
    return caps_.Fog;

  case GL_LIGHTING:
    return caps_.Lighting;

  case GL_TEXTURE_2D:
    return (caps_.Texture2D >> activeTexture_) & 0x1;

  case GL_CULL_FACE:
    return caps_.CullFace;

  case GL_ALPHA_TEST:
    return caps_.AlphaTest;

  case GL_BLEND:
    return caps_.Blend;

  case GL_COLOR_LOGIC_OP:
    return caps_.LogicOp;

  case GL_DITHER:
    return caps_.Dither;

  case GL_STENCIL_TEST:
    return caps_.StencilTest;

  case GL_DEPTH_TEST:
    return caps_.DepthTest;

  case GL_POINT_SMOOTH:
    return caps_.PointSmooth;

  case GL_POINT_SPRITE_OES:
    return caps_.PointSprite;

  case GL_LINE_SMOOTH:
    return caps_.LineSmooth;

  case GL_SCISSOR_TEST:
    return caps_.ScissorTest;

  case GL_COLOR_MATERIAL:
    return caps_.ColorMaterial;

  case GL_NORMALIZE:
    return caps_.Normalize;

  case GL_CLIP_PLANE0:
  case GL_CLIP_PLANE1:
  case GL_CLIP_PLANE2:
  case GL_CLIP_PLANE3:
  case GL_CLIP_PLANE4:
  case GL_CLIP_PLANE5:
    return (caps_.ClipPlanes >> (cap - GL_CLIP_PLANE0)) & 0x1;

  case GL_LIGHT0:
  case GL_LIGHT1:
  case GL_LIGHT2:
  case GL_LIGHT3:
  case GL_LIGHT4:
  case GL_LIGHT5:
  case GL_LIGHT6:
  case GL_LIGHT7:
    return (caps_.Lights >> (cap - GL_LIGHT0)) & 0x1;

  case GL_RESCALE_NORMAL:
    return caps_.RescaleNormal;

  case GL_POLYGON_OFFSET_FILL:
    return caps_.PolygonOffsetFill;

  case GL_MULTISAMPLE:
    return caps_.MultiSample;

  case GL_SAMPLE_ALPHA_TO_COVERAGE:
    return caps_.SampleAlphaToCoverage;

  case GL_SAMPLE_ALPHA_TO_ONE:
    return caps_.SampleAlphaToOne;

  case GL_SAMPLE_COVERAGE:
    return caps_.SampleCoverage;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::isEnabled() failed, invalid cap parameter: %d.\r\n",
              cap);
    return false;
  }
}

bool GLContext::isBuffer(GLuint buffer) {
  return (HANDLE_BUFFER == handles_->getType(buffer));
}

bool GLContext::isTexture(GLuint texture) {
  return (HANDLE_TEXTURE == handles_->getType(texture));
}
