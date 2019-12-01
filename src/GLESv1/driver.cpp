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
#include "driver.hpp"

thread_local GLContext *tls_glctx = nullptr;

GLDriver::GLDriver() : handles_(nullptr), pRasterCache_(nullptr) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  // Create the handle table
  handles_ = new HandleTable();
  handles_->addRef();

  // Create the raster cache
  err = CRasterCache::Create(&pRasterCache_);
  if (__glFailed(err)) {
    __glLogError("CRasterCache::Create() failed, err = %x.\r\n", err);
    return;
  }
}

GLDriver::~GLDriver() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeRelease(tls_glctx);
  __safeRelease(pRasterCache_);

  if (handles_) {
    {
      // Release all generated driver handles
      auto enumerator = handles_->getEnumerator(this);
      while (!enumerator.isEnd()) {
        reinterpret_cast<IObject *>(enumerator.removeNext())->release();
      }
    }

    {
      // Release all other generated handles
      auto enumerator = handles_->getEnumerator(nullptr);
      while (!enumerator.isEnd()) {
        reinterpret_cast<IObject *>(enumerator.removeNext())->release();
      }
    }

    // The handle table should now be empty
    assert(0 == handles_->getNumHandles());

    // Release the handle table
    handles_->release();
  }
}

GLContext *GLDriver::getCurrentContext() const { return tls_glctx; }

void GLDriver::makeCurrent(GLContext *pContext, GLSurface *pSurfDraw,
                            GLSurface *pSurfRead) {
  GLContext *pCtxCurr = this->getCurrentContext();
  if (pCtxCurr != pContext) {
    if (pContext) {
      pContext->addRef();
    }

    if (pCtxCurr) {
      pCtxCurr->setDrawSurface(nullptr);
      pCtxCurr->setReadSurface(nullptr);
      pCtxCurr->release();
    }

    tls_glctx = pContext;
  }

  if (pContext) {
    pContext->setDrawSurface(pSurfDraw);
    pContext->setReadSurface(pSurfRead);
  }
}
