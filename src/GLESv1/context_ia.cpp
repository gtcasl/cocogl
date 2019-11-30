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

void CGLContext::ClientState(GLenum array, bool bValue) {
  switch (array) {
  case GL_VERTEX_ARRAY:
    vertexStates_.Position = bValue;
    break;

  case GL_NORMAL_ARRAY:
    vertexStates_.Normal = bValue;
    break;

  case GL_COLOR_ARRAY:
    vertexStates_.Color = bValue;
    break;

  case GL_TEXTURE_COORD_ARRAY:
    if (bValue) {
      vertexStates_.TexCoords |= (1 << clientActiveTexture_);
    } else {
      vertexStates_.TexCoords &= ~(1 << clientActiveTexture_);
    }

    break;

  case GL_POINT_SIZE_ARRAY_OES:
    vertexStates_.PointSize = bValue;
    break;

  // GL Extensions
  case GL_WEIGHT_ARRAY_OES:
  case GL_MATRIX_INDEX_ARRAY_OES:
    break;

  default:
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::ClientState() failed, invalid ")
              _T("array parameter: %d.\r\n"),
              array);
    return;
  }
}

void CGLContext::ClientActiveTexture(GLenum texture) {
  uint32_t index = (texture - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::ClientActiveTexture() failed, ")
              _T("invalid texture parameter: %d.\r\n"),
              texture);
    return;
  }

  clientActiveTexture_ = index;
}

void CGLContext::ActiveTexture(GLenum texture) {
  uint32_t index = (texture - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::ActiveTexture() failed, ")
              _T("invalid texture parameter: %d.\r\n"),
              texture);
    return;
  }

  activeTexture_ = index;
}

void CGLContext::VertexPointer(GLint size, GLenum type, GLsizei stride,
                               const GLvoid *pPointer) {
  if ((size < 2) || (size > 4)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::VertexPointer() failed, ")
              _T("invalid size parameter: %d.\r\n"),
              size);
    return;
  }

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::VertexPointer() failed, ")
              _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::VertexPointer() failed, ")
              _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  positionArray_.Format = ToVertexFormat(type, size);
  positionArray_.Stride = stride;
  positionArray_.pPointer = pPointer;
  positionArray_.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::NormalPointer(GLenum type, GLsizei stride,
                               const GLvoid *pPointer) {
  GLint size = 3;

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::NormalPointer() failed, ")
              _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::NormalPointer() failed, ")
              _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  normalArray_.Format = ToVertexFormat(type, size);
  normalArray_.Stride = stride;
  normalArray_.pPointer = pPointer;
  normalArray_.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::ColorPointer(GLint size, GLenum type, GLsizei stride,
                              const GLvoid *pPointer) {
  if (size != 4) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::ColorPointer() failed, ")
              _T("invalid size parameter: %d.\r\n"),
              size);
    return;
  }

  if ((type != GL_UNSIGNED_BYTE) && (type != GL_FIXED) && (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::ColorPointer() failed, ")
              _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::ColorPointer() failed, ")
              _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  colorArray_.Format = ToVertexFormat(type, size);
  colorArray_.Stride = stride;
  colorArray_.pPointer = pPointer;
  colorArray_.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::TexCoordPointer(GLint size, GLenum type, GLsizei stride,
                                 const GLvoid *pPointer) {
  if ((size < 2) || (size > 4)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexCoordPointer() failed, ")
              _T("invalid size parameter: %d.\r\n"),
              size);
    return;
  }

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexCoordPointer() failed, ")
              _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::TexCoordPointer() failed, ")
              _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  VertexArray &texCoordArray = texCoordArrays_[clientActiveTexture_];

  texCoordArray.Format = ToVertexFormat(type, size);
  texCoordArray.Stride = stride;
  texCoordArray.pPointer = pPointer;
  texCoordArray.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::PointSizePointerOES(GLenum type, GLsizei stride,
                                     const GLvoid *pPointer) {
  GLint size = 1;

  if ((type != GL_FIXED) && (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::PointSizePointerOES() failed, ")
              _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::PointSizePointerOES() failed, ")
              _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  pointSizeArray_.Format = ToVertexFormat(type, size);
  pointSizeArray_.Stride = stride;
  pointSizeArray_.pPointer = pPointer;
  pointSizeArray_.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::Color(floatf red, floatf green, floatf blue, floatf alpha) {
  vColor_.x = red;
  vColor_.y = green;
  vColor_.z = blue;
  vColor_.w = alpha;

  dirtyFlags_.DefaultColor =
      Math::TIsZero(alpha - fONE) && Math::TIsZero(red - fONE) &&
      Math::TIsZero(green - fONE) && Math::TIsZero(blue - fONE);
}

void CGLContext::Normal(floatf nx, floatf ny, floatf nz) {
  vNormal_.x = nx;
  vNormal_.y = ny;
  vNormal_.z = nz;

  dirtyFlags_.NormalizeNormal = 1;
}

void CGLContext::MultiTexCoord(GLenum target, floatf s, floatf t, floatf /*r*/,
                               floatf q) {
  uint32_t index = (target - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::MultiTexCoord() failed, ")
              _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  VECTOR2 &vTexCoords = vTexCoords_[index];
  vTexCoords.x = s;
  vTexCoords.y = t;

  if (!Math::TIsZero(q - fONE)) {
    auto fInvQ = Math::TInv<floatf>(q);
    vTexCoords.x *= fInvQ;
    vTexCoords.y *= fInvQ;
  }
}
