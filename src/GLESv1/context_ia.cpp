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
    m_vertexStates.Position = bValue;
    break;

  case GL_NORMAL_ARRAY:
    m_vertexStates.Normal = bValue;
    break;

  case GL_COLOR_ARRAY:
    m_vertexStates.Color = bValue;
    break;

  case GL_TEXTURE_COORD_ARRAY:
    if (bValue) {
      m_vertexStates.TexCoords |= (1 << m_clientActiveTexture);
    } else {
      m_vertexStates.TexCoords &= ~(1 << m_clientActiveTexture);
    }

    break;

  case GL_POINT_SIZE_ARRAY_OES:
    m_vertexStates.PointSize = bValue;
    break;

  // GL Extensions
  case GL_WEIGHT_ARRAY_OES:
  case GL_MATRIX_INDEX_ARRAY_OES:
    break;

  default:
    __glError(GL_INVALID_ENUM, _T("CGLContext::ClientState() failed, invalid ")
                               _T("array parameter: %d.\r\n"),
              array);
    return;
  }
}

void CGLContext::ClientActiveTexture(GLenum texture) {
  uint32_t index = (texture - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::ClientActiveTexture() failed, ")
                               _T("invalid texture parameter: %d.\r\n"),
              texture);
    return;
  }

  m_clientActiveTexture = index;
}

void CGLContext::ActiveTexture(GLenum texture) {
  uint32_t index = (texture - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::ActiveTexture() failed, ")
                               _T("invalid texture parameter: %d.\r\n"),
              texture);
    return;
  }

  m_activeTexture = index;
}

void CGLContext::VertexPointer(GLint size, GLenum type, GLsizei stride,
                               const GLvoid *pPointer) {
  if ((size < 2) || (size > 4)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::VertexPointer() failed, ")
                                _T("invalid size parameter: %d.\r\n"),
              size);
    return;
  }

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::VertexPointer() failed, ")
                                _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::VertexPointer() failed, ")
                                _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  m_positionArray.Format = ToVertexFormat(type, size);
  m_positionArray.Stride = stride;
  m_positionArray.pPointer = pPointer;
  m_positionArray.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::NormalPointer(GLenum type, GLsizei stride,
                               const GLvoid *pPointer) {
  GLint size = 3;

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::NormalPointer() failed, ")
                                _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::NormalPointer() failed, ")
                                _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  m_normalArray.Format = ToVertexFormat(type, size);
  m_normalArray.Stride = stride;
  m_normalArray.pPointer = pPointer;
  m_normalArray.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::ColorPointer(GLint size, GLenum type, GLsizei stride,
                              const GLvoid *pPointer) {
  if (size != 4) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::ColorPointer() failed, ")
                                _T("invalid size parameter: %d.\r\n"),
              size);
    return;
  }

  if ((type != GL_UNSIGNED_BYTE) && (type != GL_FIXED) && (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::ColorPointer() failed, ")
                                _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::ColorPointer() failed, ")
                                _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  m_colorArray.Format = ToVertexFormat(type, size);
  m_colorArray.Stride = stride;
  m_colorArray.pPointer = pPointer;
  m_colorArray.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::TexCoordPointer(GLint size, GLenum type, GLsizei stride,
                                 const GLvoid *pPointer) {
  if ((size < 2) || (size > 4)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::TexCoordPointer() failed, ")
                                _T("invalid size parameter: %d.\r\n"),
              size);
    return;
  }

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::TexCoordPointer() failed, ")
                                _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::TexCoordPointer() failed, ")
                                _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  VertexArray &texCoordArray = m_texCoordArrays[m_clientActiveTexture];

  texCoordArray.Format = ToVertexFormat(type, size);
  texCoordArray.Stride = stride;
  texCoordArray.pPointer = pPointer;
  texCoordArray.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::PointSizePointerOES(GLenum type, GLsizei stride,
                                     const GLvoid *pPointer) {
  GLint size = 1;

  if ((type != GL_FIXED) && (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::PointSizePointerOES() failed, ")
                                _T("invalid type parameter: %d.\r\n"),
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::PointSizePointerOES() failed, ")
                                _T("invalid stride parameter: %d.\r\n"),
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  m_pointSizeArray.Format = ToVertexFormat(type, size);
  m_pointSizeArray.Stride = stride;
  m_pointSizeArray.pPointer = pPointer;
  m_pointSizeArray.pBuffer = this->GetBufferObject(GL_ARRAY_BUFFER);
}

void CGLContext::Color(floatf red, floatf green, floatf blue, floatf alpha) {
  m_vColor.x = red;
  m_vColor.y = green;
  m_vColor.z = blue;
  m_vColor.w = alpha;

  m_dirtyFlags.DefaultColor =
      Math::TIsZero(alpha - fONE) && Math::TIsZero(red - fONE) &&
      Math::TIsZero(green - fONE) && Math::TIsZero(blue - fONE);
}

void CGLContext::Normal(floatf nx, floatf ny, floatf nz) {
  m_vNormal.x = nx;
  m_vNormal.y = ny;
  m_vNormal.z = nz;

  m_dirtyFlags.NormalizeNormal = 1;
}

void CGLContext::MultiTexCoord(GLenum target, floatf s, floatf t, floatf /*r*/,
                               floatf q) {
  uint32_t index = (target - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::MultiTexCoord() failed, ")
                               _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  VECTOR2 &vTexCoords = m_vTexCoords[index];
  vTexCoords.x = s;
  vTexCoords.y = t;

  if (!Math::TIsZero(q - fONE)) {
    auto fInvQ = Math::TInv<floatf>(q);
    vTexCoords.x *= fInvQ;
    vTexCoords.y *= fInvQ;
  }
}
