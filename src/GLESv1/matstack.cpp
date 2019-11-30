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
#include "matstack.hpp"

CMatrixStack::CMatrixStack()
    : pMatrices_(nullptr), identityMask_(0), curIndex_(0), size_(0) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
}

CMatrixStack::~CMatrixStack() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  __safeDeleteArray(pMatrices_);
}

GLenum CMatrixStack::Create(CMatrixStack **ppMatrixStack, uint8_t size) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  assert(ppMatrixStack);

  // Create a new object
  auto pMatrixStack = new CMatrixStack();
  if (nullptr == pMatrixStack) {
    __glLogError(_T("CMatrixStack() allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  pMatrixStack->AddRef();

  // Initialize the object
  err = pMatrixStack->Initialize(size);
  if (__glFailed(err)) {
    __safeRelease(pMatrixStack);
    __glLogError(_T("CMatrixStack::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  *ppMatrixStack = pMatrixStack;

  return GL_NO_ERROR;
}

GLenum CMatrixStack::Initialize(uint8_t size) {
  assert(size <= 16);

  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  pMatrices_ = new MATRIX44[size];
  if (nullptr == pMatrices_) {
    __glLogError(_T("MATRIX44() allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  size_ = size;

  return GL_NO_ERROR;
}
