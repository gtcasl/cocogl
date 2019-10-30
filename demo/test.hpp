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

#include "fixed.hpp"
#include "mesh.hpp"

void Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

void LookAtf(
        GLfloat eyex,
        GLfloat eyey,
        GLfloat eyez,
        GLfloat centerx,
        GLfloat centery,
        GLfloat centerz,
        GLfloat upx,
        GLfloat upy,
        GLfloat upz);

bool LoadTGA(LPCTSTR lpszFileName, GLuint* id);

///////////////////////////////////////////////////////////////////////////////

class CTestBase {
public:
  CTestBase() {}
  virtual ~CTestBase() {}

  virtual bool OnInitialize(uint32_t /*width*/, uint32_t /*height*/) { return true; };

  virtual void OnRender(){};

  virtual void OnKeyNext(){};

  virtual void OnKeyPrev(){};

  virtual void OnDestroy(){};
};
