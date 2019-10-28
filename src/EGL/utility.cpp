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
#include "stdafx.h"

EGLint EGLERROR_FROM_HRESULT(HRESULT hr) {
  if (SUCCEEDED(hr)) {
    return EGL_SUCCESS;
  } else if (E_INVALIDARG == hr) {
    return EGL_BAD_PARAMETER;
  }

  return EGL_BAD_ALLOC;
}

EGLint EGLERROR_FROM_GLERROR(GLenum glErr) {
  switch (glErr) {
  case GL_NO_ERROR:
    return EGL_SUCCESS;

  case GL_INVALID_OPERATION:
    return EGL_BAD_ACCESS;

  case GL_INVALID_ENUM:
  case GL_INVALID_VALUE:
    return EGL_BAD_PARAMETER;
  }

  return EGL_BAD_ALLOC;
}
