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


class CLine_Test : CTestBase {
public:
  CLine_Test() {}

  ~CLine_Test() {}

  bool OnInitialize(HWND hWnd, LPTSTR /*lpCmdLine*/) {
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
    Rect r;
    ::GetClientRect(hWnd, &r);
    glViewport(0, 0, r.right - r.left, r.bottom - r.top);

    /*
    Setup of the projection matrix. We will use an ortho cube centered
    at (0,0,0) with 100 units of edge
    */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthox(FixedFromInt(-50), FixedFromInt(50), FixedFromInt(-50),
             FixedFromInt(50), FixedFromInt(-50), FixedFromInt(50));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
  }

  void OnRender(CGDIFont * /*pFont*/, float /*fFPS*/) {
    static int s_rotation = 0;

    GLshort vertexArray[6] = {50, 0, 0, -50, 0, 0};
    GLubyte colorArray[8] = {0xff, 0xff, 0, 0, 0xff, 0, 0xff, 0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatex(0, 0, FixedFromInt(-10));

    // Enable the vertices array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, vertexArray);

    // Enable the vertex color array
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);

    // Draw a line
    glLineWidth(16.0f);
    glDrawArrays(GL_LINES, 0, 2);

    // Disable selected arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }

  void OnKeyDown(WPARAM /*wp*/) {
    //--
  }

  void OnDestroy() {
    //--
  }
};
