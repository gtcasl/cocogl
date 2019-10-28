//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#define SURFACE_TYPE EGL_WINDOW_BIT

//#define SURFACE_TYPE EGL_PIXMAP_BIT
//#define SURFACE_TYPE EGL_PBUFFER_BIT

#include "stdafx.h"
#include "clear_t.hpp"
#include "cube_t.hpp"
#include "debug_t.hpp"
#include "fog_t.hpp"
#include "lighting_t.hpp"
#include "line_t.hpp"
#include "quad_t.hpp"
#include "shared.hpp"
#include "stencil_t.hpp"
#include "texture_t.hpp"
#include "triangle_t.hpp"

/******************************************************************************
 Defines
******************************************************************************/
// The application name and the window caption
#define SZ_APP_NAME _T("GLTest")
#define WINDOW_SIZE (1 << 10)

// OpenGL variables
EGLDisplay g_glesDisplay = NULL; // EGL display
EGLSurface g_glesSurface = NULL; // EGL rendering surface
EGLContext g_glesContext = NULL; // EGL rendering context
HBITMAP g_hPixmapBmp = NULL;
HDC g_hPixmapDC = NULL;
CGDIFont *g_pFont = NULL;
CTimer g_timer;

// CClearTest application;
// CTriangleTest application;
// CCubeTest application;
// CTextureTest application;
// CLightingTest application;
// CFogTest application;
// CStencilTest application;
CQuadTest application;
// CDebugTest application;
// CLine_Test application;


bool OGLES_Initialize(HWND hWnd, unsigned /*width*/, unsigned /*height*/) {
  EGLConfig configs[10];
  EGLint matchingConfigs;

  /*
  configAttribs is a integers list that holds the desired format of
  our framebuffer. We will ask for a framebuffer with 24 bits of
  color and 16 bits of z-buffer. We also ask for a window buffer, not
  a pbuffer or pixmap buffer
  */
  const EGLint configAttribs[] = {EGL_RED_SIZE,     5,
                                  EGL_GREEN_SIZE,   6,
                                  EGL_BLUE_SIZE,    5,
                                  EGL_ALPHA_SIZE,   EGL_DONT_CARE,
                                  EGL_DEPTH_SIZE,   16,
                                  EGL_STENCIL_SIZE, 0,
                                  EGL_SURFACE_TYPE, SURFACE_TYPE,
                                  EGL_NONE,         EGL_NONE};

#if (SURFACE_TYPE != EGL_WINDOW_BIT)
  // Retrieve the window device context
  HDC hDC = ::GetWindowDC(hWnd);

  // Create the window pixmap
  g_hPixmapBmp = CreatePixmap(hDC);
  if (NULL == g_hPixmapBmp) {
    ::ReleaseDC(hWnd, hDC);
    return false;
  }

  // Create a compatible device to hold the pixmap
  g_hPixmapDC = ::CreateCompatibleDC(hDC);
  if (NULL == g_hPixmapDC) {
    ::ReleaseDC(hWnd, hDC);
    return false;
  }

  ::ReleaseDC(hWnd, hDC);

  // Select the pixmap into its device context
  ::SelectObject(g_hPixmapDC, g_hPixmapBmp);
#endif

// Get the default display
#ifdef _WIN32_WCE
  g_glesDisplay = eglGetDisplay(0);
#else
  g_glesDisplay = eglGetDisplay(::GetDC(hWnd));
#endif
  if (EGL_NO_DISPLAY == g_glesDisplay) {
    return false;
  }

  // Display initialization (we don't care about the OGLES version numbers)
  if (!eglInitialize(g_glesDisplay, NULL, NULL)) {
    return false;
  }

  /*
  Ask for the framebuffer confiburation that best fits our
  parameters. At most, we want 10 configurations
  */
  if (!eglChooseConfig(g_glesDisplay, configAttribs, &configs[0], 10,
                       &matchingConfigs)) {
    return false;
  }

  // Check if we found a matching configuration
  if (0 == matchingConfigs) {
    return false;
  }

// Create the render surface
#if (SURFACE_TYPE == EGL_WINDOW_BIT)
  g_glesSurface = eglCreateWindowSurface(g_glesDisplay, configs[0], hWnd, NULL);
#elif (SURFACE_TYPE == EGL_PIXMAP_BIT)
  g_glesSurface =
      eglCreatePixmapSurface(g_glesDisplay, configs[0], g_hPixmapBmp, NULL);
#else
  static const EGLint pbufferAttribs[] = {EGL_WIDTH, width, EGL_HEIGHT, height,
                                          EGL_NONE};
  g_glesSurface =
      eglCreatePbufferSurface(g_glesDisplay, configs[0], pbufferAttribs);
#endif
  if (!g_glesSurface) {
    return false;
  }

  // Let's create our rendering context
  g_glesContext = eglCreateContext(g_glesDisplay, configs[0], 0, NULL);
  if (!g_glesContext) {
    return false;
  }

  // Now we will activate the context for rendering
  if (!eglMakeCurrent(g_glesDisplay, g_glesSurface, g_glesSurface,
                      g_glesContext)) {
    return false;
  }

  return true;
}


void OGLES_Cleanup() {
  if (g_pFont) {
    g_pFont->Release();
  }

  if (g_hPixmapDC) {
    ::DeleteDC(g_hPixmapDC);
  }

  if (g_hPixmapBmp) {
    ::DeleteObject(g_hPixmapBmp);
  }

  if (g_glesDisplay) {
    eglMakeCurrent(g_glesDisplay, NULL, NULL, NULL);

    if (g_glesContext) {
      eglDestroyContext(g_glesDisplay, g_glesContext);
    }

    if (g_glesSurface) {
      eglDestroySurface(g_glesDisplay, g_glesSurface);
    }

    eglTerminate(g_glesDisplay);
  }
}


LRESULT CALLBACK WndProc(HWND hWnd, unsigned uiMessage, WPARAM wParam,
                         LPARAM lParam) {
  switch (uiMessage) {
  case WM_KEYDOWN:
    switch (wParam) {
    case VK_ESCAPE:
    case 0xC1:
      ::PostQuitMessage(0);
      return 0;

    default:
      application.OnKeyDown(wParam);
      break;
    }

    break;

#ifdef _WIN32_WCE

  case WM_KILLFOCUS:
#endif

  case WM_CLOSE:
    ::DestroyWindow(hWnd);
    return 0;

  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  };

  return ::DefWindowProc(hWnd, uiMessage, wParam, lParam);
}



// Application entry point
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
                       LPTSTR lpCmdLine, int nCmdShow) {
  HWND hWnd;

#ifdef _WIN32_WCE
  /*
  This block of code is to ensure that the user only can run one
  instance of the application. First we search for a window with the
  same class name, if found, we will focus it and return
  */
  hWnd = ::FindWindow(SZ_APP_NAME, SZ_APP_NAME);
  if (hWnd) {
    /*
    Set focus to foremost child window. The "| 0x01" is used to
    bring any owned windows to the foreground and activate them.
    */
    ::SetForegroundWindow((HWND)((ULONG)hWnd | 0x00000001));
    return 0;
  }

#endif

  // This structure will hold some init values for our window
  WNDCLASS wc = {0};
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = (WNDPROC)WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = ::LoadIcon(hInstance, NULL);
  wc.hCursor = NULL;
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = SZ_APP_NAME;

  // Before creating the window, we must register this new window class
  if (!::RegisterClass(&wc)) {
    return -1;
  }

#ifdef _WIN32_WCE
  const int width = ::GetSystemMetrics(SM_CXSCREEN);
  const int height = ::GetSystemMetrics(SM_CYSCREEN);

#else
  const int width = WINDOW_SIZE;
  const int height = WINDOW_SIZE;
#endif

  // Create the application window
  hWnd = ::CreateWindow(SZ_APP_NAME,   // Class Name
                        SZ_APP_NAME,   // Caption string
                        WS_VISIBLE,    // Window style
                        CW_USEDEFAULT, // Starting x position
                        CW_USEDEFAULT, // Starting y position
                        width,         // Width
                        height,        // Height
                        NULL,          // Parent window
                        NULL,          // Menu handle
                        hInstance,     // Instance handle
                        NULL);         // Custom value to pass for WM_CREATE
  if (!hWnd) {
    // Unregister the window class
    ::UnregisterClass(SZ_APP_NAME, hInstance);
    return -1;
  }

  // Bring the window to front, focus it and refresh it
  ::ShowWindow(hWnd, nCmdShow);
  ::UpdateWindow(hWnd);

#ifdef _WIN32_WCE
  // Enable full screen mode
  ::SHFullScreen(hWnd,
                 SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
  ::MoveWindow(hWnd, 0, 0, width, height, TRUE);
#endif

  const HDC hDC = ::GetWindowDC(hWnd);

  // Initialization
  if (SUCCEEDED(CGDIFont::CreateFromResource(
          &g_pFont, hDC, ::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_FONT))) &&
      OGLES_Initialize(hWnd, width, height) &&
      application.OnInitialize(hWnd, lpCmdLine)) {
    // Message Loop
    for (;;) {
      MSG msg;
      if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (WM_QUIT == msg.message) {
          break;
        } else {
          ::TranslateMessage(&msg);
          ::DispatchMessage(&msg);
        }
      } else {
        application.OnRender(g_pFont, g_timer.GetFPS());

#if (SURFACE_TYPE == EGL_WINDOW_BIT)
        // Copy gl surface to the screen
        eglSwapBuffers(g_glesDisplay, g_glesSurface);

        // Copy font surface to the screen
        g_pFont->Present(hDC, 0, 0);

#elif (SURFACE_TYPE == EGL_PIXMAP_BIT)
        // Flush all gl rendering
        glFlush();

        // Copy font data into the pixmap
        g_pFont->Present(g_hPixmapDC, 0, 0);

        // Copy pixmap to the screen
        ::BitBlt(hDC, 0, 0, width, height, g_hPixmapDC, 0, 0, SRCCOPY);

#else
        // Flush all gl rendering
        glFlush();

        // Copy the gl surface into the pixmap
        eglCopyBuffers(g_glesDisplay, g_glesSurface, g_hPixmapBmp);

        // Copy font surface into the pixmap
        g_pFont->Present(g_hPixmapDC, 0, 0);

        // Copy pixmap to the screen
        ::BitBlt(hDC, 0, 0, width, height, g_hPixmapDC, 0, 0, SRCCOPY);
#endif
      }
    }
  }

  ::ReleaseDC(hWnd, hDC);

  // Destroy the application
  application.OnDestroy();

  // OpenGL ES clean up
  OGLES_Cleanup();

  // Unregister the window class
  ::UnregisterClass(SZ_APP_NAME, hInstance);

  return 0;
}
