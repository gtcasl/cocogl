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


class CStencilTest : CTestBase {
private:
  GLfloat _boxColors[16 * 6];
  GLfloat _box[12 * 6];
  GLfloat _floorVertices[12];

  GLuint m_texture;
  float m_xrot;
  float m_yrot;
  float m_yval;
  bool m_yup;

public:
  CStencilTest() {
    m_xrot = 0.0f;
    m_yrot = 0.0f;
    m_yval = 1.0f;
    m_yup = true;
  }

  ~CStencilTest() {}

  void SetPerspective(HWND hWnd) {
    Rect r;
    ::GetClientRect(hWnd, &r);

    const float ratio = (float)(r.right - r.left) / (r.bottom - r.top);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, r.right - r.left, r.bottom - r.top);
    Perspective(45.0f, ratio, 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  bool OnInitialize(HWND hWnd, LPTSTR /*lpCmdLine*/) {
    static const GLfloat floorVertices[] = {-3.0f, 0.0f, 3.0f,  3.0f,
                                            0.0f,  3.0f, -3.0f, 0.0f,
                                            -3.0f, 3.0f, 0.0f,  -3.0f};

    static_assert(sizeof(_floorVertices) == sizeof(floorVertices), "invalid size");
    memcpy(_floorVertices, floorVertices, sizeof(floorVertices));

    static const GLfloat box[] = {
        // FRONT
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f, // BACK
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f, // LEFT
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f, // RIGHT
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f, // TOP
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,

        // BOTTOM
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
    };

    static_assert(sizeof(_box) == sizeof(box), "invalid size");
    memcpy(_box, box, sizeof(box));

    static const GLfloat boxColors[] = {
        // FRONT
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // BACK
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // LEFT
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RIGHT
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // TOP
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // BOTTOM
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    static_assert(sizeof(_boxColors) == sizeof(boxColors), "invalid size");
    memcpy(_boxColors, boxColors, sizeof(boxColors));

    SetPerspective(hWnd);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glClearStencil(0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, _boxColors);

    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    return true;
  }

  void drawCube() {
    glPushMatrix();

    glEnableClientState(GL_COLOR_ARRAY);

    glTranslatef(0.0f, m_yval, 0.0f);

    glRotatef(m_xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yrot, 0.0f, 1.0f, 0.0f);

    glVertexPointer(3, GL_FLOAT, 0, _box);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
  }

  void OnRender(CGDIFont * /*pFont*/, float /*fFPS*/) {
    m_xrot += 2.0f;
    m_yrot += 3.0f;

    m_yval += m_yup ? 0.02f : -0.02f;

    if (m_yval > 2.0f)
      m_yup = false;

    if (m_yval < 1.0f)
      m_yup = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    LookAtf(0.0f, 3.0f, 7.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);

    // Floor
    glVertexPointer(3, GL_FLOAT, 0, _floorVertices);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Cube
    glPushMatrix();
    {
      glScalef(1.0f, -1.0f, 1.0f);

      glCullFace(GL_FRONT);

      drawCube();

      glCullFace(GL_BACK);
    }

    glPopMatrix();

    glDisable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Floor
    glVertexPointer(3, GL_FLOAT, 0, _floorVertices);
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);

    drawCube();
  }

  void OnKeyDown(WPARAM /*wp*/) {
    //--
  }

  void OnDestroy() { glDeleteTextures(1, &m_texture); }
};
