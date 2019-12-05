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

class TriangleTest : public Renderer {
public:
  TriangleTest() {}

  ~TriangleTest() {}

  bool OnInitialize(EGLNativeWindowType window) {
    Renderer::OnInitialize(window);
    /*
    Remember: because we are programming for a mobile device, we cant
    use any of the OpenGL ES functions that finish in 'f', we must use
    the fixed point version (they finish in 'x'
    */
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glShadeModel(GL_SMOOTH);

    /*
    In order to set a viewport that fits entirely our window, we need
    to know the window dimensions. They could be obtained through the
    WinCE call GetWindowRect, using our window handle
    */
    glViewport(0, 0, width_, height_);

    /*
    Setup of the projection matrix. We will use an ortho cube centered
    at (0,0,0) with 100 units of edge
    */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-50, 50, -50, 50, -50, 50);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
  }

  void OnRender() {
    /* Vertex 1    Vertex 2      Vertex 3*/
    GLshort vertexArray[9] = {-25, -25, 0, 25, -25, 0, 0, 25, 0};

    // GLshort vertexArray[9] = { -80,-20,0,   -80,-80,0,     40,-20,0 };
    GLubyte colorArray[12] = {255, 0, 0, 0, 0, 255, 0, 0, 0, 0, 255, 0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0, 0, -10);

    // Enable the vertices array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, vertexArray);

    // Enable the vertex color array
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);

    // Draw a triangle, 0 = first element, 3 = number of vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Disable selected arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    Renderer::OnRender();
  }
};
