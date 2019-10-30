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
class CDebugTest : public CTestBase {
public:
  bool OnInitialize(uint32_t /*width*/, uint32_t /*height*/) {
    glViewport(0, 0, 48, 48);
    glScissor(0, 0, 48, 48);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_FLAT);

    const float vProjection[] = {0.041667f, 0.0f,  0.0f, 0.0f, 0.0f,  0.041667f,
                                 0.0f,      0.0f,  0.0f, 0.0f, -1.0f, 0.0f,
                                 -1.0f,     -1.0f, 0.0f, 1.0f};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(vProjection);

    const float vLightPosition[] = {0.0f, 0.0f, 1.0f, 0.0f};
    const float vLightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const float vLightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const float vLightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, vLightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, vLightDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, vLightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, vLightSpecular);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);

    const float vMaterialAmbient[] = {1.0f, 0.0f, 0.0f, 1.0f};
    const float vMaterialDiffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
    const float vMaterialEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    const float vMaterialSpecular[] = {0.0f, 1.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vMaterialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vMaterialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, vMaterialEmission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vMaterialSpecular);

    const float vLightModelAmbient[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vLightModelAmbient);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_RESCALE_NORMAL);

    return true;
  }

  void OnRender() {
    const GLfloat vertexArray[4] = {4.4f, 28.5f, 19.6f, 28.5f};

    // const GLfloat vertexArray[12] = { 4.4f,19.6f,0.0f,  4.4f,4.4f,0.0f,
    // 19.6f,19.6f,0.0f, 19.6f,4.4f,0.0f };
    // const GLfloat normalArray[12] = { 1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
    // 0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f };
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexArray);

    // glVertexPointer( 3, GL_FLOAT, 0, vertexArray );
    // glNormalPointer( GL_FLOAT, 0, normalArray );
    glDrawArrays(GL_LINES, 0, 2);

    // glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // UINT pixels;
    // glReadPixels( 4, 38, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixels );
    // glReadPixels( 19, 38, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixels );
  }
};
