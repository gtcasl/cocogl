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

class TextRenderer {
public:

  TextRenderer();

  ~TextRenderer();

  bool initialize(const char* fontimage);

  void drawText(const char *text, float x, float y, float scale);

  void destroy();

private:

  enum{ NUMCHARS = 256, SPACE = 32 };

  struct glyph_t {
    int l;
    int t;
    int w;
    int h;
  };

  struct vertex_t {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
  };

  std::vector<vertex_t> vertices_;
  std::array<glyph_t, NUMCHARS> glyphs_;
  float inv_width_;
  float inv_height_;

  GLuint texture_;
};

///////////////////////////////////////////////////////////////////////////////

class Renderer {
public:

  Renderer(EGLNativeWindowType window);

  virtual ~Renderer();

  virtual bool OnInitialize();

  virtual void OnRender() = 0;

  virtual void OnKeyNext();

  virtual void OnKeyPrev();

  virtual void OnDestroy();

  void Present();
  
protected:

  EGLDisplay glDisplay_;
  EGLConfig  glConfig_;
  EGLContext glContext_;
  EGLSurface glSurface_;
  EGLint width_;
  EGLint height_;
};
