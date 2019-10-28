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

void CGLContext::FrontFace(GLenum mode) {
  switch (mode) {
  case GL_CW:
  case GL_CCW:
    m_cullStates.FrontFace = static_cast<uint8_t>(FrontFaceFromEnum(mode));
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::FrontFace() failed, invalid mode parameter: %d.\r\n"),
        mode);
    return;
  }
}


void CGLContext::CullFace(GLenum mode) {
  switch (mode) {
  case GL_FRONT:
  case GL_BACK:
  case GL_FRONT_AND_BACK:
    m_cullStates.CullFace = static_cast<uint8_t>(CullFaceFromEnum(mode));
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::CullFace() failed, invalid mode parameter: %d.\r\n"),
        mode);
    return;
  }
}
