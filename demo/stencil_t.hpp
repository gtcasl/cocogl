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

class StencilTest : public TestBase {
private:
  GLfloat _boxColors[16 * 6];
  GLfloat _box[12 * 6];
  GLfloat _floorVertices[12];

  GLuint texture_;
  float xrot_;
  float yrot_;
  float yval_;
  bool yup_;

public:
  StencilTest() {
    xrot_ = 0.0f;
    yrot_ = 0.0f;
    yval_ = 1.0f;
    yup_ = true;
  }

  ~StencilTest() {}

  bool OnInitialize(uint32_t width, uint32_t height) {
    static const GLfloat floorVertices[] = {-3.0f, 0.0f, 3.0f,  3.0f,
                                            0.0f,  3.0f, -3.0f, 0.0f,
                                            -3.0f, 3.0f, 0.0f,  -3.0f};

    static_assert(sizeof(_floorVertices) == sizeof(floorVertices));
    memcpy(_floorVertices, floorVertices, sizeof(floorVertices));

    static const GLfloat box[] = {
        // FRONT
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
        0.5f, // BACK
        -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f, // LEFT
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
        -0.5f, // RIGHT
        0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
        0.5f, // TOP
        -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f,

        // BOTTOM
        -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
        -0.5f,
    };

    static_assert(sizeof(_box) == sizeof(box));
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

    static_assert(sizeof(_boxColors) == sizeof(boxColors));
    memcpy(_boxColors, boxColors, sizeof(boxColors));

    // set perspective
    float ratio = (float)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    Perspective(45.0f, ratio, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

    glTranslatef(0.0f, yval_, 0.0f);

    glRotatef(xrot_, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot_, 0.0f, 1.0f, 0.0f);

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

  void OnRender() {
    xrot_ += 2.0f;
    yrot_ += 3.0f;

    yval_ += yup_ ? 0.02f : -0.02f;

    if (yval_ > 2.0f)
      yup_ = false;

    if (yval_ < 1.0f)
      yup_ = true;

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

  void OnDestroy() { glDeleteTextures(1, &texture_); }
};
