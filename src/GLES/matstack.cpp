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
    : m_curIndex(0), m_size(0), m_pMatrices(NULL), m_identityMask(0) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
}


CMatrixStack::~CMatrixStack() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  __safeDeleteArray(m_pMatrices);
}


GLenum CMatrixStack::Create(CMatrixStack **ppMatrixStack, uint8_t size) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  ASSERT(ppMatrixStack);

  // Create a new object
  CMatrixStack *pMatrixStack = new CMatrixStack();
  if (NULL == pMatrixStack) {
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
  ASSERT(size <= 16);

  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  m_pMatrices = new MATRIX44[size];
  if (NULL == m_pMatrices) {
    __glLogError(_T("MATRIX44() allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  m_size = size;

  return GL_NO_ERROR;
}
