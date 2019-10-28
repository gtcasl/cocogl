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


class CQuadTest : CTestBase {
private:
  GLuint m_texture;
  unsigned m_testcase;

public:
  CQuadTest() {
    m_texture = 0;
    m_testcase = 1;
  }

  ~CQuadTest() {
    //--
  }

  bool OnInitialize(HWND hWnd, LPTSTR /*lpCmdLine*/) {
    /*
    Remember: because we are programming for a mobile device, we cant
    use any of the OpenGL ES functions that finish in 'f', we must use
    the fixed point version (they finish in 'x'
    */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    if (!LoadTexture(_T("lady.tga"), &m_texture)) {
      return false;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

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
    at (0,0,0) with 1.0f units of edge
    */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*float equation0[] = {1.0f, 0.0f, 0.0f, 0.2f};
    float equation1[] = {-1.0f, 0.0f, 0.0f,0.2f};
    float equation2[] = {0.0f, 1.0f, 0.0f, 0.2f};
    float equation3[] = {0.0f,-1.0f, 0.0f, 0.2f};
    glClipPlanef( GL_CLIP_PLANE0, equation0 );
    glClipPlanef( GL_CLIP_PLANE1, equation1 );
    glClipPlanef( GL_CLIP_PLANE2, equation2 );
    glClipPlanef( GL_CLIP_PLANE3, equation3 );
    glEnable( GL_CLIP_PLANE0 );
    glEnable( GL_CLIP_PLANE1 );
    glEnable( GL_CLIP_PLANE2 );
    glEnable( GL_CLIP_PLANE3 );*/

    return true;
  }

  void OnRender(CGDIFont *pFont, float fFPS) {
    const GLfloat s = 1.0f;

    GLfloat vertexArray[12] = {-1.0f / s, -1.0f / s, 0.0f,      1.0f / s,
                               -1.0f / s, 0.5f,      -1.0f / s, 1.0f / s,
                               0.0f,      1.0f / s,  1.0f / s,  0.5f};

    GLubyte colorArray[16] = {0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
                              0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00};

    static GLfloat texCoords[] = {0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 1.0f, 1.0f, 1.0f};

    const float matrixAffine[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f};

    const float matrixPersp[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                   0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f, 1.0f};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable the vertices array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);

    // Enable the vertex color array
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);

    // Enable the texture coordinates array
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    switch (m_testcase) {
    case 0:
      glDisable(GL_TEXTURE_2D);
      glShadeModel(GL_FLAT);
      break;

    case 1:
      glDisable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      break;

    case 2:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 3:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 4:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 5:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 6:
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 7:
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 8:
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 9:
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 10:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 11:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 12:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 13:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 14:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 15:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 16:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 17:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 18:
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 19:
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 20:
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 21:
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixAffine);
      break;

    case 22:
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 23:
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_ALPHA_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);
      glEnable(GL_TEXTURE_2D);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;

    case 24:
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_ALPHA_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);
      glEnable(GL_TEXTURE_2D);
      glShadeModel(GL_SMOOTH);
      glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glLoadMatrixf(matrixPersp);
      break;
    }

    // Draw a quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Disable selected arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Draw the frame rate
    TCHAR szFPS[64];
    _sntprintf(szFPS, 64, _T("%06.2f fps [%d]"), fFPS, m_testcase);
    pFont->Draw(szFPS);
  }

  void OnKeyDown(WPARAM wp) {
    switch (wp) {
    case 37: // Left
      if (m_testcase > 0) {
        --m_testcase;
      }

      break;
    case 39: // Right
      if (m_testcase < 24) {
        ++m_testcase;
      }

      break;
    }
  }

  void OnDestroy() {
    //--
  }
};
