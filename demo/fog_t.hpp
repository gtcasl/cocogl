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

class FogTest : public TestBase {
private:
  GLfloat _box[12 * 6];
  GLfloat _texCoords[8 * 6];
  GLuint texture_;
  float xrot_;
  float yrot_;

public:
  FogTest() {
    xrot_ = 0.0f;
    yrot_ = 0.0f;
  }

  ~FogTest() {}

  bool OnInitialize(uint32_t width, uint32_t height) {
    static const float fogColor[] = {0.5f, 0.5f, 0.5f, 1.0f};

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

    static const GLfloat texCoords[] = {
        // FRONT
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // BACK
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // LEFT
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // RIGHT
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // TOP
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // BOTTOM
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    static_assert(sizeof(_texCoords) == sizeof(texCoords));
    memcpy(_texCoords, texCoords, sizeof(texCoords));

    static const float lightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};

    static const float matAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};

    if (!LoadTGA("media/block.tga", &texture_))
      return false;

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float ratio = (float)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    Perspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glFogf(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepthf(1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, _box);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);

    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    return true;
  }

  void OnRender() {
    xrot_ += 2.0f;
    yrot_ += 3.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);

    glRotatef(xrot_, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot_, 0.0f, 1.0f, 0.0f);

    glScalef(2.0f, 2.0f, 2.0f);

    // FRONT AND BACK
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

    // LEFT AND RIGHT
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

    // TOP AND BOTTOM
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
  }

  void OnDestroy() { glDeleteTextures(1, &texture_); }
};
