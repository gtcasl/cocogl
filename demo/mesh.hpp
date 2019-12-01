//
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

class CMesh {
public:
  CMesh();
  ~CMesh();

  bool OnInitialize(const char *fileName); 

  void OnRender();
private:
  char name_[128];
  char parentName_[128];
  GLshort num__indices;
  GLshort num__vertices;
  GLshort *pIndices_;
  GLfloat *pGeometry_;
  GLfloat *pNormals_;
  GLfloat *pTexCoords_;
};
