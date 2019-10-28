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


struct FixedMesh {
  GLshort indexCounter;
  GLshort vertexCounter;
  GLshort *Indices;
  GLfixed *Geometry;
  GLfixed *Normals;
  GLfixed *TexCoord;
};


struct GenericObjectData {
  char Name[128];
  char ParentName[128];
  unsigned *Indices;
  float *Geometry;
  float *Normals;
  float *TexCoord;
  ULONG iC;
  ULONG vC;
};


struct GSDHeader {
  char id[32];
  char version[16];
  int numberOfSubObjects;
};


class Mesh {
public:
  Mesh(LPCTSTR lpszFileName);
  ~Mesh();

  void Draw();

  bool GetState() { return m_state; }

private:
  FixedMesh m_mesh;
  bool m_state;
};
