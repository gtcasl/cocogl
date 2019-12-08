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

#include "renderer.hpp"

class CubeTest : public Renderer {
private:
  int rotation_;

public:
  CubeTest(EGLNativeWindowType window)
      : Renderer(window)
      , rotation_(0) 
  {}

  bool OnInitialize() {

    /*Remember: because we are programming for a mobile device, we cant
    use any of the OpenGL ES functions that finish in 'f', we must use
    the fixed point version (they finish in 'x'*/
    glClearColor(0, 0, 0, 0);

    // Do not want to see smoothed colors, only a plain color for face
    glShadeModel(GL_FLAT);

    // Enable the depth test in order to see the cube correctly
    glEnable(GL_DEPTH_TEST);

    /*Taking care of specifying correctly the winding order of the
    vertices (counter clock wise order), we can cull up all back faces.
    This is probably one of the better optimizations we could work,
    because we are avoiding a lot of computations that wont be reflected
    in the screen, using  glEnable(GL_CULL_FACE) to do the work*/
    glEnable(GL_CULL_FACE);

    /*In order to set a viewport that fits entirely our window, we need
    to know the window dimensions. They could be obtained through the
    WinCE call GetWindowRect, using our window handle*/
    glViewport(0, 0, width_, height_);

    /*
   Setup of the orthographic matrix.
   We use an ortho cube centered at ( 0, 0, 0 ) with 40 units of edge*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-20, 20, -20, 20, -20, 20);
    glMatrixMode(GL_MODELVIEW);

    return true;
  }

  void OnRender() {
    // We are going to draw a cube centered at origin, and with an edge of 10
    // units
    /*
           7                      6
            +--------------------+
          / |                   /|
        /   |                 /  |
    3 /     |             2 /    |
    +---------------------+      |
    |       |             |      |
    |       |             |      |
    |       |             |      |
    |       |             |      |
    |       |             |      |
    |       |             |      |
    |       |             |      |
    |       |4            |      |5
    |       +-------------|------+
    |     /               |     /
    |   /                 |   /
    | /                   | /
    +---------------------+
    0                      1

    */
    static GLubyte front[] = {2, 1, 3, 0};  // front face
    static GLubyte back[] = {5, 6, 4, 7};   // back face
    static GLubyte top[] = {6, 2, 7, 3};    // top face
    static GLubyte bottom[] = {1, 5, 0, 4}; // bottom face
    static GLubyte left[] = {3, 0, 7, 4};   // left face
    static GLubyte right[] = {6, 5, 2, 1};  // right face
    static GLshort vertices[] = {-5, -5, -5, 5, -5, -5, 5, 5, -5, -5, 5, -5,
                                 -5, -5, 5, 5, -5, 5, 5, 5, 5, -5, 5, 5};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glRotatef(45, 1.f, 0, 0);
    glRotatef(rotation_, 0, 1.0f, 0);

    // Enable the vertices array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, vertices);

    // 3 = XYZ coordinates, GL_SHORT = data type, 0 = 0 stride bytes
    /*We are going to draw the cube, face by face, with different colors for
    each face, using indexed vertex arrays and triangle strips*/
    glColor4f(1, 0, 0, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, front);

    glColor4f(0, 1, 0, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, back);

    glColor4f(0, 0, 1, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, top);

    glColor4f(1, 1, 0, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, bottom);

    glColor4f(0, 1, 1, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, left);

    glColor4f(1, 0, 1, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, right);

    glDisableClientState(GL_VERTEX_ARRAY);

    ++rotation_;
  }
};
