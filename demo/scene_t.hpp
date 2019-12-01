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

class CSceneTest : public CTestBase {
private:
  CMesh mesh_;
  int rotation_;
  float lightRotation_;

public:
  CSceneTest() : rotation_(0), lightRotation_(0) {}

  bool OnInitialize(uint32_t width, uint32_t height) {
    /*Remember: because we are programming for a mobile device, we cant
    use any of the OpenGL ES functions that finish in 'f', we must use
    the fixed point version (they finish in 'x'*/
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

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
    glViewport(0, 0, width, height);

    // Set perspective
    float ratio = (float)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Perspective(45.0f, ratio, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    // Light Settings
    // Set a 50% grey diffuse component
    float diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    // Enable lighting computations
    glEnable(GL_LIGHTING);

    // Enable Light0 (switch on it)
    glEnable(GL_LIGHT0);

    // Fog settings
    // 50% green fog
    float fogColor[] = {0, 0.5f, 0, 0};
    glFogfv(GL_FOG_COLOR, fogColor);

    /*We chosen a linear mode for fog. Fog will begin to show its effects at 20
    units
    of distance from the camera, all the highest fog density will be reached at
    50 units*/
    glFogx(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 20);
    glFogf(GL_FOG_END, 50);
    glEnable(GL_FOG);

    // Load our scene file
    return mesh_.OnInitialize("media/scene.gsd");
  }

  void OnRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    /*"camera" setup to have a good point of view of the scene, also we will
     perform a rotation
     on the mesh*/
    glTranslatef(0, -5, -40);
    glRotatef(20, 1, 0, 0);
    glRotatef(rotation_, 0, 1, 0);

    // With x and z we compute the points of a circle, with a radius = 30, and
    // the velocity = LightRotation
    float x = 0, z = 0;
    x = 30 * sin(lightRotation_); // we have to use float4, because we do not
    z = 30 * cos(lightRotation_); // have a fixed point version of sin and cos
    float lightPosition[] = {0, 10, 0, 1};
    lightPosition[0] = x;
    lightPosition[2] = z;

    // Setup of Light0 position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // render mesh
    mesh_.OnRender();

    ++rotation_;
    lightRotation_ += 0.2f;
  }
};
