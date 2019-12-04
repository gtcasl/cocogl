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

void GLContext::setClientState(GLenum array, bool bValue) {
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
              "GLContext::setClientState() failed, invalid "
              "array parameter: %d.\r\n",
              array);
    return;
  }
}

void GLContext::setClientActiveTexture(GLenum texture) {
  uint32_t index = (texture - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setClientActiveTexture() failed, "
              "invalid texture parameter: %d.\r\n",
              texture);
    return;
  }

  clientActiveTexture_ = index;
}

void GLContext::setActiveTexture(GLenum texture) {
  uint32_t index = (texture - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setActiveTexture() failed, "
              "invalid texture parameter: %d.\r\n",
              texture);
    return;
  }

  activeTexture_ = index;
}

void GLContext::setVertexPointer(GLint size, GLenum type, GLsizei stride,
                                 const GLvoid *pPointer) {
  if ((size < 2) || (size > 4)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setVertexPointer() failed, "
              "invalid size parameter: %d.\r\n",
              size);
    return;
  }

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setVertexPointer() failed, "
              "invalid type parameter: %d.\r\n",
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setVertexPointer() failed, "
              "invalid stride parameter: %d.\r\n",
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  positionArray_.Format = ToVertexFormat(type, size);
  positionArray_.Stride = stride;
  positionArray_.pPointer = pPointer;
  positionArray_.pBuffer = this->getBufferObject(GL_ARRAY_BUFFER);
}

void GLContext::setNormalPointer(GLenum type, GLsizei stride,
                                 const GLvoid *pPointer) {
  GLint size = 3;

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setNormalPointer() failed, "
              "invalid type parameter: %d.\r\n",
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setNormalPointer() failed, "
              "invalid stride parameter: %d.\r\n",
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  normalArray_.Format = ToVertexFormat(type, size);
  normalArray_.Stride = stride;
  normalArray_.pPointer = pPointer;
  normalArray_.pBuffer = this->getBufferObject(GL_ARRAY_BUFFER);
}

void GLContext::setColorPointer(GLint size, GLenum type, GLsizei stride,
                                const GLvoid *pPointer) {
  if (size != 4) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setColorPointer() failed, "
              "invalid size parameter: %d.\r\n",
              size);
    return;
  }

  if ((type != GL_UNSIGNED_BYTE) && (type != GL_FIXED) && (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setColorPointer() failed, "
              "invalid type parameter: %d.\r\n",
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setColorPointer() failed, "
              "invalid stride parameter: %d.\r\n",
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  colorArray_.Format = ToVertexFormat(type, size);
  colorArray_.Stride = stride;
  colorArray_.pPointer = pPointer;
  colorArray_.pBuffer = this->getBufferObject(GL_ARRAY_BUFFER);
}

void GLContext::setTexCoordPointer(GLint size, GLenum type, GLsizei stride,
                                   const GLvoid *pPointer) {
  if ((size < 2) || (size > 4)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexCoordPointer() failed, "
              "invalid size parameter: %d.\r\n",
              size);
    return;
  }

  if ((type != GL_BYTE) && (type != GL_SHORT) && (type != GL_FIXED) &&
      (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexCoordPointer() failed, "
              "invalid type parameter: %d.\r\n",
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setTexCoordPointer() failed, "
              "invalid stride parameter: %d.\r\n",
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
  texCoordArray.pBuffer = this->getBufferObject(GL_ARRAY_BUFFER);
}

void GLContext::setPointSizePointerOES(GLenum type, GLsizei stride,
                                       const GLvoid *pPointer) {
  GLint size = 1;

  if ((type != GL_FIXED) && (type != GL_FLOAT)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setPointSizePointerOES() failed, "
              "invalid type parameter: %d.\r\n",
              type);
    return;
  }

  if (stride < 0) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setPointSizePointerOES() failed, "
              "invalid stride parameter: %d.\r\n",
              stride);
    return;
  }

  if (0 == stride) {
    stride = GLSizeOf(type) * size;
  }

  pointSizeArray_.Format = ToVertexFormat(type, size);
  pointSizeArray_.Stride = stride;
  pointSizeArray_.pPointer = pPointer;
  pointSizeArray_.pBuffer = this->getBufferObject(GL_ARRAY_BUFFER);
}

void GLContext::setColor(floatf red, floatf green, floatf blue, floatf alpha) {
  vColor_.x = red;
  vColor_.y = green;
  vColor_.z = blue;
  vColor_.w = alpha;

  dirtyFlags_.DefaultColor =
      Math::IsAlmostZero(alpha - Math::One<floatf>()) && Math::IsAlmostZero(red - Math::One<floatf>()) &&
      Math::IsAlmostZero(green - Math::One<floatf>()) && Math::IsAlmostZero(blue - Math::One<floatf>());
}

void GLContext::setNormal(floatf nx, floatf ny, floatf nz) {
  vNormal_.x = nx;
  vNormal_.y = ny;
  vNormal_.z = nz;

  dirtyFlags_.NormalizeNormal = 1;
}

void GLContext::setMultiTexCoord(GLenum target, floatf s, floatf t,
                                 floatf /*r*/, floatf q) {
  uint32_t index = (target - GL_TEXTURE0);
  if (index >= MAX_TEXTURES) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setMultiTexCoord() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  VECTOR2 &vTexCoords = vTexCoords_[index];
  vTexCoords.x = s;
  vTexCoords.y = t;

  if (!Math::IsAlmostZero(q - Math::One<floatf>())) {
    auto fInvQ = Math::Inverse<floatf>(q);
    vTexCoords.x *= fInvQ;
    vTexCoords.y *= fInvQ;
  }
}
