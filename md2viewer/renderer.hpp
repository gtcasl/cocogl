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

#include "md2.hpp"

enum class ShadeMode {
  flat,
  gourand,
  texture
};

class Renderer {
public:
  
  Renderer(EGLNativeWindowType window);

  virtual ~Renderer();

  virtual bool OnInitialize(const char* model, const char* background);

  virtual void OnRender();

  virtual void OnKeyNext();

  virtual void OnKeyPrev();

  virtual void OnDestroy();

  void Present();

  void resize(int width, int height);

  const char* getAnimation() const {
    return md2_.getAnimation(animKey_);
  }

protected:

  EGLDisplay glDisplay_;
  EGLConfig glConfig_;
  EGLContext glContext_;
  EGLSurface glSurface_;
  EGLint width_;
  EGLint height_;

   GLuint background_;

  md2::MD2Model md2_;

  vec3_t translation_;
  vec3_t rotation_;
  float scaling_;

  int animKey_;
};
