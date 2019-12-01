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

void CGLContext::ShadeModel(GLenum mode) {
  switch (mode) {
  case GL_FLAT:
  case GL_SMOOTH:
    caps_.ShadeModel = ShadeModelFromEnum(mode);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::ShadeMode() failed, invalid mode parameter: %d.\r\n",
        mode);
    return;
  }
}

void CGLContext::Scissor(GLint x, GLint y, GLsizei width, GLsizei height) {
  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE,
              "CGLContext::Scissor() failed, invalid "
              "width=%d or height=%d parameters.\r\n",
              width, height);
    return;
  }

  scissor_.left = x;
  scissor_.top = y;
  scissor_.right = x + width;
  scissor_.bottom = y + height;

  dirtyFlags_.ScissorRECT = 1;
}

void CGLContext::SampleCoverage(floatf value, GLboolean invert) {
  sampleCoverage_.fValue = Math::TSat(value);
  sampleCoverage_.bInvert = invert ? true : false;
}

void CGLContext::PolygonOffset(floatf factor, floatf units) {
  polygonOffset_.fFactor = factor;
  polygonOffset_.fUnits = units;
}

void CGLContext::PointSize(floatf size) {
  if (size <= fZERO) {
    __glError(
        GL_INVALID_VALUE,
        "CGLContext::PointSize() failed, invalid size parameter: %d.\r\n",
        size);
    return;
  }

  fPointSize_ = size;
}

void CGLContext::LineWidth(floatf width) {
  if (width <= fZERO) {
    __glError(
        GL_INVALID_VALUE,
        "CGLContext::LineWidth() failed, invalid width parameter: %d.\r\n",
        width);
    return;
  }

  fLineWidth_ = width;
}

void CGLContext::AlphaFunc(GLenum func, floatf ref) {
  switch (func) {
  case GL_NEVER:
  case GL_LESS:
  case GL_EQUAL:
  case GL_LEQUAL:
  case GL_GREATER:
  case GL_NOTEQUAL:
  case GL_GEQUAL:
  case GL_ALWAYS:
    rasterStates_.AlphaFunc = CompareFuncFromEnum(func);
    rasterData_.AlphaRef =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(ref)));
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::AlphaFunc() failed, invalid func parameter: %d.\r\n",
        func);
    return;
  }
}

void CGLContext::StencilFunc(GLenum func, GLint ref, GLuint mask) {
  switch (func) {
  case GL_NEVER:
  case GL_LESS:
  case GL_EQUAL:
  case GL_LEQUAL:
  case GL_GREATER:
  case GL_NOTEQUAL:
  case GL_GEQUAL:
  case GL_ALWAYS:
    rasterStates_.StencilFunc = CompareFuncFromEnum(func);
    rasterData_.StencilRef = static_cast<uint8_t>(ref);
    rasterData_.StencilMask = static_cast<uint8_t>(mask);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::StencilFunc() failed, invalid func parameter: %d.\r\n",
        func);
    return;
  }
}

void CGLContext::StencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
#ifndef NDEBUG
  static const char *op_names[3] = {"fail", "zfail", "zpass"};
#endif
  uint32_t results[3];
  const GLenum operations[3] = {fail, zfail, zpass};

  for (uint32_t i = 0; i < 3; ++i) {
    GLenum op = operations[i];
    switch (op) {
    case GL_ZERO:
    case GL_INVERT:
    case GL_KEEP:
    case GL_REPLACE:
    case GL_INCR:
    case GL_DECR:
      results[i] = StencilOpFromEnum(op);
      break;

    default:
      __glError(
          GL_INVALID_VALUE,
          "CGLContext::StencilOp() failed, invalid %s parameter: %d.\r\n",
          op_names[i], op);
      return;
    }
  }

  rasterStates_.StencilFail = results[0];
  rasterStates_.StencilZFail = results[1];
  rasterStates_.StencilZPass = results[2];
}

void CGLContext::DepthFunc(GLenum func) {
  switch (func) {
  case GL_NEVER:
  case GL_LESS:
  case GL_EQUAL:
  case GL_LEQUAL:
  case GL_GREATER:
  case GL_NOTEQUAL:
  case GL_GEQUAL:
  case GL_ALWAYS:
    rasterStates_.DepthFunc = CompareFuncFromEnum(func);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::DepthFunc() failed, invalid func parameter: %d.\r\n",
        func);
    return;
  }
}

void CGLContext::BlendFunc(GLenum sfactor, GLenum dfactor) {
  switch (sfactor) {
  case GL_ZERO:
  case GL_ONE:
  case GL_SRC_ALPHA:
  case GL_ONE_MINUS_SRC_ALPHA:
  case GL_DST_ALPHA:
  case GL_ONE_MINUS_DST_ALPHA:
  case GL_DST_COLOR:
  case GL_ONE_MINUS_DST_COLOR:
  case GL_SRC_ALPHA_SATURATE:
    rasterStates_.BlendSrc = BlendFuncFromEnum(sfactor);
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "CGLContext::BlendFunc() failed, invalid "
              "sfactor parameter: %d.\r\n",
              sfactor);
    return;
  }

  switch (dfactor) {
  case GL_ZERO:
  case GL_ONE:
  case GL_SRC_COLOR:
  case GL_ONE_MINUS_SRC_COLOR:
  case GL_SRC_ALPHA:
  case GL_ONE_MINUS_SRC_ALPHA:
  case GL_DST_ALPHA:
  case GL_ONE_MINUS_DST_ALPHA:
    rasterStates_.BlendDst = BlendFuncFromEnum(dfactor);
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "CGLContext::BlendFunc() failed, invalid "
              "dfactor parameter: %d.\r\n",
              dfactor);
    return;
  }
}

void CGLContext::LogicOp(GLenum opcode) {
  switch (opcode) {
  case GL_CLEAR:
  case GL_AND:
  case GL_AND_REVERSE:
  case GL_COPY:
  case GL_AND_INVERTED:
  case GL_NOOP:
  case GL_XOR:
  case GL_OR:
  case GL_NOR:
  case GL_EQUIV:
  case GL_INVERT:
  case GL_OR_REVERSE:
  case GL_COPY_INVERTED:
  case GL_OR_INVERTED:
  case GL_NAND:
  case GL_SET:
    rasterStates_.LogicFunc = LogicOpFromEnum(opcode);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::LogicOp() failed, invalid opcode parameter: %d.\r\n",
        opcode);
    return;
  }
}
