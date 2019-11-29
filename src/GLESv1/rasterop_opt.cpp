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
#include "raster.hpp"
#include "rasterop.hpp"
#include "rasterop_opt.hpp"
#include "rasterop_opt.inl"

#define MAKE_SCANLINE(flags, states, texture0, texture1)                       \
  {                                                                            \
    RASTERID(flags, states, texture0, texture1)                                \
    , TOptimizedScanline<flags, states, texture0, texture1>::Execute           \
  }

static const struct ScanlineRasterID {
  RASTERID RasterID;
  PFN_Scanline pfnScanline;
}

l_optimizedScanlines[] = {
    MAKE_SCANLINE(0, 0, 0, 0),
};

COptimizedRasterOp::COptimizedRasterOp(PFN_Scanline pfnScanline) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
  m_pfnScanline = pfnScanline;
}

COptimizedRasterOp::~COptimizedRasterOp() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
}

GLenum COptimizedRasterOp::Create(IRasterOp **ppRasterOp,
                                  const RASTERID &rasterID) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  ASSERT(ppRasterOp);

  // First lookup the optimized scanline table
  for (uint32_t i = 0; i < __countof(l_optimizedScanlines); ++i) {
    if (l_optimizedScanlines[i].RasterID == rasterID) {
      // Create an optimized rasterOp object
      auto pRasterOp =
          new COptimizedRasterOp(l_optimizedScanlines[i].pfnScanline);
      if (nullptr == pRasterOp) {
        __glLogError(
            _T("COptimizedRasterOp allocation failed, out of memory.\r\n"));
        return GL_OUT_OF_MEMORY;
      }

      pRasterOp->AddRef();
      *ppRasterOp = pRasterOp;
      return GL_NO_ERROR;
    }
  }

  return __GL_NO_DATA;
}