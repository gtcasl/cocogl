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

void CGLContext::ClipPlane(GLenum plane, const VECTOR4 &equation) {
  if ((plane - GL_CLIP_PLANE0) >= MAX_CLIPPLANES) {
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::ClipPlane() failed, invalid plane parameter: %d.\r\n"),
        plane);
    return;
  }

  if (dirtyFlags_.ModelViewInvT44) {
    this->UpdateModelViewInvT44();
  }

  uint32_t index = (plane - GL_CLIP_PLANE0);
  Math::Mul(&vClipPlanesES_[index], equation, mModelViewInvT_);

  dirtyFlags_.ClipPlanesCS |= (1 << index);
}

void CGLContext::DepthRange(floatf zNear, floatf zFar) {
  depthRange_.fNear = Math::TSat(zNear);
  depthRange_.fFar = Math::TSat(zFar);

  dirtyFlags_.ScreenXform = 1;
}

void CGLContext::Viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  if ((width < 0) || (height < 0)) {
    __glError(GL_INVALID_VALUE,
              _T("CGLContext::Viewport() failed, invalid ")
              _T("width=%d or height=%d parameters.\r\n"),
              width, height);
    return;
  }

  viewport_.left = x;
  viewport_.top = y;
  viewport_.right = x + width;
  viewport_.bottom = y + height;

  dirtyFlags_.ScreenXform = 1;
}
