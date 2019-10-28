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


class CLightingTest : CTestBase {
private:
  FixedMesh *m_pMesh;

public:
  CLightingTest() : m_pMesh(NULL) {}

  ~CLightingTest() {}

  void SetPerspective(HWND hWnd) {
    Rect r;
    GetWindowRect(hWnd, &r);

    float ratio = (float)(r.right - r.left) / (r.bottom - r.top);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Perspective(45.0f, ratio, 1.0f, 40.0f);
    glMatrixMode(GL_MODELVIEW);
  }

  bool OnInitialize(HWND hWnd, LPTSTR /*lpCmdLine*/) {
    /*Remember: because we are programming for a mobile device, we cant
    use any of the OpenGL ES functions that finish in 'f', we must use
    the fixed point version (they finish in 'x'*/
    glClearColorx(FixedFromFloat(0.5f), FixedFromFloat(0.5f),
                  FixedFromFloat(0.5f), ONE);

    // Do not want to see smoothed colors, only a plain color for face
    glShadeModel(GL_SMOOTH);

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
    Rect r;
    ::GetClientRect(hWnd, &r);
    glViewport(0, 0, r.right - r.left, r.bottom - r.top);
    SetPerspective(hWnd);

    // Light Settings
    // Set a 50% grey diffuse component
    GLfixed diffuse[] = {FixedFromFloat(0.5f), FixedFromFloat(0.5f),
                         FixedFromFloat(0.5f), ONE};
    glLightxv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    // Enable lighting computations
    glEnable(GL_LIGHTING);

    // Enable Light0 (switch on it)
    glEnable(GL_LIGHT0);

    // Fog settings
    // 50% green fog
    GLfixed fogColor[] = {0, FixedFromFloat(0.5f), 0, 0};
    glFogxv(GL_FOG_COLOR, fogColor);

    /*We chosen a linear mode for fog. Fog will begin to show its effects at 20
    units
    of distance from the camera, all the highest fog density will be reached at
    50 units*/
    glFogx(GL_FOG_MODE, GL_LINEAR);
    glFogx(GL_FOG_START, FixedFromInt(20));
    glFogx(GL_FOG_END, FixedFromInt(50));
    glEnable(GL_FOG);

    // Load our scene file
    m_pMesh = LoadMeshFromFile(_T("scene.gsd"));

    return m_pMesh ? true : false;
  }

  void OnRender(CGDIFont * /*pFont*/, float /*fFPS*/) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    static int rotation = 0;

    /*"camera" setup to have a good point of view of the scene, also we will
     perform a rotation
     on the mesh*/
    glTranslatex(0, FixedFromInt(-5), FixedFromInt(-40));
    glRotatex(FixedFromInt(20), ONE, 0, 0);
    glRotatex(FixedFromInt(rotation++), 0, ONE, 0);

    // With x and z we compute the points of a circle, with a radius = 30, and
    // the velocity = LightRotation
    static float LightRotation = 0;
    float x = 0, z = 0;
    LightRotation += 0.2f;
    x = 30 *
        (float)sin(LightRotation); // we have to use float4, because we do not
    z = 30 *
        (float)cos(LightRotation); // have a fixed point version of sin and cos
    GLfixed lightPosition[] = {0, FixedFromInt(10), 0, FixedFromInt(1)};
    lightPosition[0] = FixedFromFloat(x);
    lightPosition[2] = FixedFromFloat(z);

    // Setup of Light0 position
    glLightxv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FIXED, 0, m_pMesh->Geometry);

    // Normals are needed for the lighting computations
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FIXED, 0, m_pMesh->Normals);

    glDrawElements(GL_TRIANGLES, m_pMesh->indexCounter, GL_UNSIGNED_SHORT,
                   m_pMesh->Indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }

  void OnKeyDown(WPARAM /*wp*/) {
    //--
  }

  void OnDestroy() {
    if (m_pMesh) {
      // Delete mesh data
      delete[] m_pMesh->Geometry;
      delete[] m_pMesh->Indices;
      delete[] m_pMesh->Normals;
      delete[] m_pMesh->TexCoord;
      delete m_pMesh;
    }
  }
};
