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
#include "context.hpp"

thread_local GLContext *tls_glctx = nullptr;

GLDriver::GLDriver() : pRasterCache_(nullptr) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  // Create the raster cache
  err = RasterCache::Create(&pRasterCache_);
  if (__glFailed(err)) {
    __glLogError("RasterCache::Create() failed, err = %x.\r\n", err);
    std::abort();
  }
}

GLDriver::~GLDriver() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeRelease(tls_glctx);
  __safeRelease(pRasterCache_);
    
  // Release all handles
  auto enumerator = handles_.getEnumerator();
  while (!enumerator.isEnd()) {
    reinterpret_cast<IObject *>(enumerator.getObject())->release();
    enumerator.moveNext();
  }
}

GLContext *GLDriver::getCurrentContext() const {
  return tls_glctx;
}

void GLDriver::makeCurrent(GLContext *pContext, 
                           GLSurface *pSurfDraw,
                           GLSurface *pSurfRead) {
  auto pCtxCurr = this->getCurrentContext();
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
