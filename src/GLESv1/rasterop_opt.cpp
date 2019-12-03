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
  PfnScanline pfnScanline;
}

l_optimizedScanlines[] = {
    MAKE_SCANLINE(0, 0, 0, 0),
};

OptimizedRasterOp::OptimizedRasterOp(PfnScanline pfnScanline) {
  __profileAPI(" - %s()\n", __FUNCTION__);
  pfnScanline_ = pfnScanline;
}

OptimizedRasterOp::~OptimizedRasterOp() {
  __profileAPI(" - %s()\n", __FUNCTION__);
}

GLenum OptimizedRasterOp::Create(IRasterOp **ppRasterOp,
                                  const RASTERID &rasterID) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(ppRasterOp);

  // First lookup the optimized scanline table
  for (uint32_t i = 0; i < __countof(l_optimizedScanlines); ++i) {
    if (l_optimizedScanlines[i].RasterID == rasterID) {
      // Create an optimized rasterOp object
      auto pRasterOp =
          new OptimizedRasterOp(l_optimizedScanlines[i].pfnScanline);
      if (nullptr == pRasterOp) {
        __glLogError(
            "OptimizedRasterOp allocation failed, out of memory.\r\n");
        return GL_OUT_OF_MEMORY;
      }

      pRasterOp->addRef();
      *ppRasterOp = pRasterOp;
      return GL_NO_ERROR;
    }
  }

  return __GL_NO_DATA;
}