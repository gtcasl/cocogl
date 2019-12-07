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
#include "renderer.hpp"

Renderer::Renderer(EGLNativeWindowType window) {
  EGLint egl_config_attr[] = {
      EGL_BUFFER_SIZE,  32, EGL_DEPTH_SIZE,   16,
      EGL_STENCIL_SIZE, 0,  EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE};

  EGLint numConfigs, majorVersion, minorVersion;

  glDisplay_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(glDisplay_, &majorVersion, &minorVersion);
  eglChooseConfig(glDisplay_, egl_config_attr, &glConfig_, 1, &numConfigs);
  glContext_ = eglCreateContext(glDisplay_, glConfig_, EGL_NO_CONTEXT, nullptr);
  glSurface_ = eglCreateWindowSurface(glDisplay_, glConfig_, window, 0);
  eglQuerySurface(glDisplay_, glSurface_, EGL_WIDTH, &width_);
  eglQuerySurface(glDisplay_, glSurface_, EGL_HEIGHT, &height_);
  eglMakeCurrent(glDisplay_, glSurface_, glSurface_, glContext_);
}

Renderer::~Renderer() {
  eglDestroySurface(glDisplay_, glSurface_);
  eglDestroyContext(glDisplay_, glContext_);
  eglTerminate(glDisplay_);
}

bool Renderer::OnInitialize() { return true; }

void Renderer::OnDestroy() {
  //--
}

void Renderer::Present() { eglSwapBuffers(glDisplay_, glSurface_); }

void Renderer::OnKeyNext() {
  //--
}

void Renderer::OnKeyPrev() {
  //--
}