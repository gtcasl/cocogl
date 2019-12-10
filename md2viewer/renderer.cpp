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
      EGL_BUFFER_SIZE, 32,
      EGL_DEPTH_SIZE, 16,
      EGL_STENCIL_SIZE, 0,
      EGL_SURFACE_TYPE,
      EGL_WINDOW_BIT,
      EGL_NONE
  };

  EGLint numConfigs, majorVersion, minorVersion;

  glDisplay_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(glDisplay_, &majorVersion, &minorVersion);
  eglChooseConfig(glDisplay_, egl_config_attr, &glConfig_, 1, &numConfigs);
  glContext_ = eglCreateContext(glDisplay_, glConfig_, EGL_NO_CONTEXT, nullptr);
  glSurface_ = eglCreateWindowSurface(glDisplay_, glConfig_, window, 0);
  eglQuerySurface(glDisplay_, glSurface_, EGL_WIDTH, &width_);
  eglQuerySurface(glDisplay_, glSurface_, EGL_HEIGHT, &height_);
  eglMakeCurrent(glDisplay_, glSurface_, glSurface_, glContext_);

  translation_ = {0, 0, -100};
  rotation_ = {-90, 0, -90};
  scaling_ = 1.0;
  animKey_ = 0;
}

Renderer::~Renderer() {
  if (background_) {
    glDeleteTextures(1, &background_);
  }

  eglDestroySurface(glDisplay_, glSurface_);
  eglDestroyContext(glDisplay_, glContext_);
  eglTerminate(glDisplay_);
}

bool Renderer::OnInitialize(const char* model, const char* background) {
  if (background) {
		std::vector<uint8_t> pixels;
		int width, height, bpp;
		if (!LoadTGA(background, pixels, &width, &height, &bpp))
			return false;
		background_ = loadTexture(pixels, width, height, bpp);
	} else {
    background_ = 0;
  }

  if (!md2_.load(model))
    return false;

  this->resize(width_, height_);

  GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0};
	GLfloat	diffuse[] = { 1.0, 1.0, 1.0, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position[] = { 0.5, 0.5, 0, 0.0 };
	GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0 };
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_DEPTH_TEST);

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);  

  return true;
}

void Renderer::resize(int width, int height) {	
  glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	Perspective(45.0f, float(width) / height, 0.1f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	width_ = width;
	height_ = height;
}

void Renderer::OnDestroy() {
  md2_.destroy();
}

void Renderer::OnRender() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (background_) {
    GLfloat vertices[8] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat texCoords[8] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glActiveTexture(GL_TEXTURE0);
	  glClientActiveTexture(GL_TEXTURE0);	
	  glBindTexture(GL_TEXTURE_2D, background_);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
	  glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }

  glLoadIdentity();	
	glTranslatef(translation_.x, translation_.y, translation_.z);
	glScalef(scaling_, scaling_, scaling_);
	glRotatef(rotation_.x, 1.0, 0.0, 0.0);
	glRotatef(rotation_.y, 0.0, 1.0, 0.0);
	glRotatef(rotation_.z, 0.0, 0.0, 1.0);
	
  md2_.animate(animKey_);

  md2_.render();
}

void Renderer::Present() {
  eglSwapBuffers(glDisplay_, glSurface_);
}

void Renderer::OnKeyPrev() {
  if (animKey_ > 0)
    --animKey_;  
}

void Renderer::OnKeyNext() {
  if (animKey_+1 < md2_.getNumAnimKeys())
    ++animKey_;
}