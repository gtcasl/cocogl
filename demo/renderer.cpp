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

TextRenderer::TextRenderer() {
  //--
}

TextRenderer::~TextRenderer() {
  //--
}

bool TextRenderer::initialize(const char* fontimage) {   
  std::vector<uint8_t> pixels;
  int width, height, bpp;
  if (!LoadTGA(fontimage, pixels, &width, &height, &bpp))    
    return false;
  assert(bpp == 32);

  auto bits = reinterpret_cast<uint32_t*>(pixels.data());
  uint32_t sepColor = bits[0];
	int x = 0, c = SPACE + 1;
  while (x < width && c < NUMCHARS) {
    if (bits[x] != sepColor) {
      auto& glyph = glyphs_[c];
      glyph.l = x;
      while (sepColor != bits[x] && x < width) {
				++x;
			}
			glyph.w = (x - glyph.l);      
      glyph.t = 1;
      glyph.h = 11;
      ++c;
    } else {
      ++x;
    }
  }

  // fix space character
	glyphs_[SPACE].l = 0;
	glyphs_[SPACE].w = glyphs_[SPACE + 1].w;

  texture_ = loadTexture(pixels, width, height, bpp);
  inv_width_ = 1.0f / width;
  inv_height_ = 1.0f / height;
  return true;
}

void TextRenderer::drawText(const char *text, float x, float y, float scale) {
  vertices_.resize(6 * strlen(text));

  int n = 0;

  auto coords = vertices_.data();
  for (const char *p = text; *p; ++p) { 
    auto& glyph = glyphs_[*p];
    
    auto bl = glyph.l;
    auto bt = glyph.t;
    auto bw = glyph.w;
    auto bh = glyph.h;

    auto x2 =  x + bl * scale;
    auto y2 = -y - bt * scale;
    auto w = bw * scale;
    auto h = bh * scale;

    x += bw * scale; // next character

    auto tl = float(bl) * inv_width_;
    auto tr = float(bl + bw) * inv_width_;
    auto tt = float(bt) * inv_height_;
    auto tb = float(bt + bh) * inv_height_;    
    
    coords[n++] = {x2,     -y2    , tl, tt};
    coords[n++] = {x2 + w, -y2    , tr, tt};
    coords[n++] = {x2,     -y2 - h, tl, tb};
    coords[n++] = {x2 + w, -y2    , tr, tt};
    coords[n++] = {x2,     -y2 - h, tl, tb};
    coords[n++] = {x2 + w, -y2 - h, tr, tb};
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex_t), vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
  glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(8));

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

  glDrawArrays(GL_TRIANGLES, 0, n);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TextRenderer::destroy() {
  glDeleteTextures(1, &texture_);
}

///////////////////////////////////////////////////////////////////////////////

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

bool Renderer::OnInitialize() {  
  return true;
}

void Renderer::OnDestroy() {
  //--
}

void Renderer::Present() {
  eglSwapBuffers(glDisplay_, glSurface_);
}

void Renderer::OnKeyNext() {
  //--
}

void Renderer::OnKeyPrev() {
  //--
}