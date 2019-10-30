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

///---------------------------------------------------------------------------
/// <summary>
/// </summary>
///---------------------------------------------------------------------------
class CTextureTest : public CTestBase {
private:
  // Texture handles
  GLuint m_texture1;
  GLuint m_texture2;

public:
  CTextureTest() {}

  ~CTextureTest() {}

  bool OnInitialize(uint32_t width, uint32_t height) {
    /*Remember: because we are programming for a mobile device, we cant
    use any of the OpenGL ES functions that finish in 'f', we must use
    the fixed point version (they finish in 'x'*/
    glClearColorx(FixedFromFloat(0.5f), FixedFromFloat(0.5f),
                  FixedFromFloat(0.5f), ONE);

    // Do not want to see smoothed colors, only a plain color for face
    glShadeModel(GL_FLAT);

    // Enable the depth test in order to see the cube correctly
    glEnable(GL_DEPTH_TEST);

    /*Taking care of specifying correctly the winding order of the
    vertices (counter clock wise order), we can cull all back faces.
    This is probably one of the best optimizations we could do,
    because, by this way, we avoid a lot of computations that wont be
    reflected in the screen. Use  glEnable(GL_CULL_FACE) to do the work*/
    glEnable(GL_CULL_FACE);

    /*In order to set a viewport that fits entirely our window, we need
    to know the window dimensions. They could be obtained through the
    WinCE call GetWindowRect, using our window handle*/
    glViewport(0, 0, width, height);

    // Set perspective
    float ratio = (float)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Perspective(45.0f, ratio, 1.0f, 40.0f);
    glMatrixMode(GL_MODELVIEW);

    bool result = LoadTGA(_T("media/door128.tga"), &m_texture1);
    result &= LoadTGA(_T("media/fire128.tga"), &m_texture2);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return result;
  }

  void OnRender() {
    static int rotation = 0;

    // We are going to draw a cube centered at origin, and with an edge of 10
    // units
    /*
           7                      6
            +--------------------+
          / |                   /|
        /   |                 /  |
    3  /     |             2 /    |
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
                                 -5, -5, 5,  5, -5, 5,  5, 5, 5,  -5, 5, 5};

    static GLfixed texCoords[] = {0,   0,   ONE, 0,   ONE, ONE, 0,   ONE,
                                  ONE, ONE, 0,   ONE, 0,   0,   ONE, 0};

    /*GLfixed TWO = FixedFromInt(2);
    static GLfixed texCoords[] = {0,0,     TWO,0,  TWO,TWO,  0,TWO,
                                TWO,TWO ,0,TWO,  0,0,      TWO,0};*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatex(0, 0, FixedFromInt(-20));
    glScalef(0.7f, 0.7f, 0.7f);
    glRotatex(FixedFromInt(45), ONE, 0, 0);
    glRotatex(FixedFromInt(rotation++), 0, ONE, 0);

    // Enable the vertices array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, vertices);

    // 3 = XYZ coordinates, GL_SHORT = data type, 0 = 0 stride bytes
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FIXED, 0, texCoords);

    glActiveTexture(GL_TEXTURE1);
    glClientActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FIXED, 0, texCoords);

    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

    static float offset = 0.0f;
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatex(FixedFromFloat(offset += 0.01f), 0, 0);
    glMatrixMode(GL_MODELVIEW);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, front);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, back);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, top);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, bottom);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, left);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, right);

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  void OnDestroy() {
    glDeleteTextures(1, &m_texture1);
    glDeleteTextures(1, &m_texture2);
  }
};
