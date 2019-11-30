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

void CGLContext::MatrixMode(GLenum mode) {
  switch (mode) {
  case GL_MODELVIEW:
    pMatrixStack_ = pMsModelView_;
    break;

  case GL_PROJECTION:
    pMatrixStack_ = pMsProjection_;
    break;

  case GL_TEXTURE:
    pMatrixStack_ = pMsTexCoords_[activeTexture_];
    break;

  case GL_MATRIX_PALETTE_OES:
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::MatrixMode() failed, invalid mode parameter: %d.\r\n"),
        mode);
    return;
  }

  matrixMode_ = mode;
}

void CGLContext::PushMatrix() {
  if (!pMatrixStack_->Push()) {
    __glError(GL_STACK_OVERFLOW, _T("CGLContext::PushMatrix() failed, the ")
                                 _T("current matrix stack is full.\r\n"));
    return;
  }
}

void CGLContext::PopMatrix() {
  if (!pMatrixStack_->Pop()) {
    __glError(GL_STACK_UNDERFLOW, _T("CGLContext::PopMatrix() failed, the ")
                                  _T("current matrix stack contains only a ")
                                  _T("single matrix.\r\n"));
    return;
  }

  this->UpdateMatrixDirtyFlags();
}

void CGLContext::LoadIdentity() {
  if (!pMatrixStack_->IsIdentity()) {
    pMatrixStack_->ToIdentity();
    this->UpdateMatrixDirtyFlags();
  }
}

void CGLContext::LoadMatrix(const MATRIX44 &matrix) {
  pMatrixStack_->SetMatrix(matrix);
  this->UpdateMatrixDirtyFlags();
}

void CGLContext::Frustum(floatf left, floatf right, floatf bottom, floatf top,
                         floatf zNear, floatf zFar) {
  if (zNear < fZERO) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::Frustum() failed, zNear should not be negative.\r\n"));
    return;
  }

  if (zFar < fZERO) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::Frustum() failed, zFar should not be negative.\r\n"));
    return;
  }

  if (left == right) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::Frustum() failed, left and ")
                                _T("right parameters should not be ")
                                _T("equal.\r\n"));
    return;
  }

  if (bottom == top) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::Frustum() failed, bottom and ")
                                _T("top parameters should not be equal.\r\n"));
    return;
  }

  MATRIX44 matTmp;
  Math::Frustum(&matTmp, left, right, bottom, top, zNear, zFar);
  this->Multiply(matTmp);
}

void CGLContext::Ortho(floatf left, floatf right, floatf bottom, floatf top,
                       floatf zNear, floatf zFar) {
  MATRIX44 matTmp;
  Math::Ortho(&matTmp, left, right, bottom, top, zNear, zFar);
  this->Multiply(matTmp);
}

void CGLContext::Scale(floatf x, floatf y, floatf z) {
  MATRIX44 matTmp;
  Math::Scale(&matTmp, x, y, z);
  this->Multiply(matTmp);
}

void CGLContext::Translate(floatf x, floatf y, floatf z) {
  MATRIX44 matrix, matTmp;
  Math::Translate(&matrix, x, y, z);
  Math::Mul(&matTmp, pMatrixStack_->GetMatrix(), matrix);
  pMatrixStack_->SetMatrix(matTmp);
  this->UpdateMatrixDirtyFlags();
}

void CGLContext::Rotate(floatf angle, floatf x, floatf y, floatf z) {
  MATRIX44 matTmp;
  Math::Rotate(&matTmp, Math::DegToRad(angle), x, y, z);
  this->Multiply(matTmp);
}

void CGLContext::Multiply(const MATRIX44 &matrix) {
  MATRIX44 matTmp;
  Math::Mul(&matTmp, pMatrixStack_->GetMatrix(), matrix);
  pMatrixStack_->SetMatrix(matTmp);
  this->UpdateMatrixDirtyFlags();
}
