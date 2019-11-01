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

  bool OnInitialize(LPCTSTR lpszFileName); 

  void OnRender();
private:
  char m_name[128];
  char m_parentName[128];
  GLshort m_num_indices;
  GLshort m_num_vertices;
  GLshort *m_pIndices;
  GLfloat *m_pGeometry;
  GLfloat *m_pNormals;
  GLfloat *m_pTexCoords;
};
