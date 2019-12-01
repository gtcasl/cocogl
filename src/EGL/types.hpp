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
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#pragma once

#define SZ_EGL_VENDOR "Blaise Tine"
#define EGL_VERSION_MAJOR 1
#define EGL_VERSION_MINOR 1
#define SZ_EGL_VERSION "1.1"
#define SZ_EGL_EXTENSIONS ""

#define MAX_PBUFFER_MIPLEVELS 11
#define MAX_PBUFFER_WIDTH (1 << MAX_PBUFFER_MIPLEVELS)
#define MAX_PBUFFER_HEIGHT (1 << MAX_PBUFFER_MIPLEVELS)
#define MAX_PBUFFER_PIXELS (MAX_PBUFFER_MIPLEVELS * MAX_PBUFFER_WIDTH * MAX_PBUFFER_HEIGHT)

enum eHANDLETYPE {
  HANDLE_NONE = 0,
  HANDLE_DISPLAY = 1,
  HANDLE_CONFIG = 2,
  HANDLE_CONTEXT = 3,
  HANDLE_SURFACE = 4,
};
