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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void *__GLContext;
typedef void *__GLSurface;

struct GLSurfaceDesc {
  uint8_t  Format;
  uint8_t *pBits;
  uint16_t Width;
  uint16_t Height;
  uint16_t Pitch;  
};

extern GLenum __glCreateSurface(const GLSurfaceDesc *pColorDesc,
                                const GLSurfaceDesc *pDepthStencilDesc,
                                __GLSurface *pSurface);
extern GLenum __glUpdateSurface(__GLSurface surface,
                                const GLSurfaceDesc *pColorDesc,
                                const GLSurfaceDesc *pDepthStencilDesc);
extern GLenum __glDestroySurface(__GLSurface surface);
extern GLenum __glCreateContext(__GLContext shared_context,
                                __GLContext *pContext);
extern GLenum __glDestroyContext(__GLContext context);
extern GLenum __glMakeCurrent(__GLContext context, __GLSurface draw,
                              __GLSurface read);
extern GLenum __glCopyBuffers(const GLSurfaceDesc *pDstDesc,
                              const GLSurfaceDesc *pSrcDesc);
extern GLenum __glBindTexImage(__GLSurface surface, bool bGenMipMaps);
extern GLenum __glReleaseTexImage(__GLSurface surface);
extern GLenum __glSaveBitmap(__GLSurface surface, LPCTSTR lpszFilename);

#ifdef __cplusplus
}

#endif
