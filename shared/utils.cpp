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
#include <stdarg.h>

#if defined(_WIN32)

HDC GetDefaultDisplay() {
  return ::GetDC(NULL);
}

void ReleaseDisplay(HDC display) {
  ::ReleaseDC(display);
}

void DestroyPixmap(HBITMAP pixmap) {
  ::DeleteObject(pixmap);
}

#elif defined(__linux__)

Display* GetDefaultDisplay() {
  return XOpenDisplay(NULL);
}

void ReleaseDisplay(Display* display) {
  XCloseDisplay(display);
}

void DestroyPixmap(Pixmap pixmap) {
  //TODO:
}

#endif

void DbgPrintf(int level, LPCTSTR format, ...) {
  __unreferenced(level);
  va_list args;
  va_start(args, format);
  vftprintf(stderr, format, args);
  va_end(args);
}
