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
    m_caps.ShadeModel = ShadeModelFromEnum(mode);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::ShadeMode() failed, invalid mode parameter: %d.\r\n"),
        mode);
    return;
  }
}

void CGLContext::Scissor(GLint x, GLint y, GLsizei width, GLsizei height) {
  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::Scissor() failed, invalid ")
                                _T("width=%d or height=%d parameters.\r\n"),
              width, height);
    return;
  }

  m_scissor.left = x;
  m_scissor.top = y;
  m_scissor.right = x + width;
  m_scissor.bottom = y + height;

  m_dirtyFlags.ScissorRECT = 1;
}

void CGLContext::SampleCoverage(floatf value, GLboolean invert) {
  m_sampleCoverage.fValue = Math::TSat(value);
  m_sampleCoverage.bInvert = invert ? true : false;
}

void CGLContext::PolygonOffset(floatf factor, floatf units) {
  m_polygonOffset.fFactor = factor;
  m_polygonOffset.fUnits = units;
}

void CGLContext::PointSize(floatf size) {
  if (size <= fZERO) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::PointSize() failed, invalid size parameter: %d.\r\n"),
        size);
    return;
  }

  m_fPointSize = size;
}

void CGLContext::LineWidth(floatf width) {
  if (width <= fZERO) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::LineWidth() failed, invalid width parameter: %d.\r\n"),
        width);
    return;
  }

  m_fLineWidth = width;
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
    m_rasterStates.AlphaFunc = CompareFuncFromEnum(func);
    m_rasterData.AlphaRef =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(ref)));
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::AlphaFunc() failed, invalid func parameter: %d.\r\n"),
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
    m_rasterStates.StencilFunc = CompareFuncFromEnum(func);
    m_rasterData.StencilRef = static_cast<uint8_t>(ref);
    m_rasterData.StencilMask = static_cast<uint8_t>(mask);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::StencilFunc() failed, invalid func parameter: %d.\r\n"),
        func);
    return;
  }
}

void CGLContext::StencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
#ifndef NDEBUG
  static const LPCTSTR szOperations[3] = {_T("fail"), _T("zfail"), _T("zpass")};
#endif
  unsigned results[3];
  const GLenum operations[3] = {fail, zfail, zpass};

  for (unsigned i = 0; i < 3; ++i) {
    const GLenum op = operations[i];
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
          _T("CGLContext::StencilOp() failed, invalid %s parameter: %d.\r\n"),
          szOperations[i], op);
      return;
    }
  }

  m_rasterStates.StencilFail = results[0];
  m_rasterStates.StencilZFail = results[1];
  m_rasterStates.StencilZPass = results[2];
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
    m_rasterStates.DepthFunc = CompareFuncFromEnum(func);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::DepthFunc() failed, invalid func parameter: %d.\r\n"),
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
    m_rasterStates.BlendSrc = BlendFuncFromEnum(sfactor);
    break;

  default:
    __glError(GL_INVALID_ENUM, _T("CGLContext::BlendFunc() failed, invalid ")
                               _T("sfactor parameter: %d.\r\n"),
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
    m_rasterStates.BlendDst = BlendFuncFromEnum(dfactor);
    break;

  default:
    __glError(GL_INVALID_ENUM, _T("CGLContext::BlendFunc() failed, invalid ")
                               _T("dfactor parameter: %d.\r\n"),
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
    m_rasterStates.LogicFunc = LogicOpFromEnum(opcode);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::LogicOp() failed, invalid opcode parameter: %d.\r\n"),
        opcode);
    return;
  }
}
