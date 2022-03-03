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

void GLContext::setMatrixMode(GLenum mode) {
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
        "GLContext::setMatrixMode() failed, invalid mode parameter: %d.\r\n",
        mode);
    return;
  }

  matrixMode_ = mode;
}

void GLContext::pushMatrix() {
  if (!pMatrixStack_->push()) {
    __glError(GL_STACK_OVERFLOW, "GLContext::pushMatrix() failed, the "
                                 "current matrix stack is full.\r\n");
    return;
  }
}

void GLContext::popMatrix() {
  if (!pMatrixStack_->pop()) {
    __glError(GL_STACK_UNDERFLOW, "GLContext::popMatrix() failed, the "
                                  "current matrix stack contains only a "
                                  "single matrix.\r\n");
    return;
  }

  this->updateMatrixDirtyFlags();
}

void GLContext::loadIdentity() {
  if (!pMatrixStack_->isIdentity()) {
    pMatrixStack_->toIdentity();
    this->updateMatrixDirtyFlags();
  }
}

void GLContext::loadMatrix(const MATRIX44 &matrix) {
  pMatrixStack_->setMatrix(matrix);
  this->updateMatrixDirtyFlags();
}

void GLContext::frustum(floatf left, floatf right, floatf bottom, floatf top,
                        floatf zNear, floatf zFar) {
  if (zNear < Zero<floatf>()) {
    __glError(GL_INVALID_VALUE,
              "GLContext::frustum() failed, zNear should not be negative.\r\n");
    return;
  }

  if (zFar < Zero<floatf>()) {
    __glError(GL_INVALID_VALUE,
              "GLContext::frustum() failed, zFar should not be negative.\r\n");
    return;
  }

  if (left == right) {
    __glError(GL_INVALID_VALUE, "GLContext::frustum() failed, left and "
                                "right parameters should not be "
                                "equal.\r\n");
    return;
  }

  if (bottom == top) {
    __glError(GL_INVALID_VALUE, "GLContext::frustum() failed, bottom and "
                                "top parameters should not be equal.\r\n");
    return;
  }

  MATRIX44 matTmp;
  Frustum(&matTmp, left, right, bottom, top, zNear, zFar);
  this->multiply(matTmp);
}

void GLContext::ortho(floatf left, floatf right, floatf bottom, floatf top,
                      floatf zNear, floatf zFar) {
  MATRIX44 matTmp;
  Ortho(&matTmp, left, right, bottom, top, zNear, zFar);
  this->multiply(matTmp);
}

void GLContext::scale(floatf x, floatf y, floatf z) {
  MATRIX44 matTmp;
  Scale(&matTmp, x, y, z);
  this->multiply(matTmp);
}

void GLContext::translate(floatf x, floatf y, floatf z) {
  MATRIX44 matrix, matTmp;
  Translate(&matrix, x, y, z);
  Mul(&matTmp, pMatrixStack_->getMatrix(), matrix);
  pMatrixStack_->setMatrix(matTmp);
  this->updateMatrixDirtyFlags();
}

void GLContext::rotate(floatf angle, floatf x, floatf y, floatf z) {
  MATRIX44 matTmp;
  Rotate(&matTmp, DegToRad(angle), x, y, z);
  this->multiply(matTmp);
}

void GLContext::multiply(const MATRIX44 &matrix) {
  MATRIX44 matTmp;
  Mul(&matTmp, pMatrixStack_->getMatrix(), matrix);
  pMatrixStack_->setMatrix(matTmp);
  this->updateMatrixDirtyFlags();
}
