//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#include "stdafx.h"
#include "mesh.hpp"


Mesh::Mesh(LPCTSTR lpszFileName) {
  m_state = true;

  GSDHeader header;

  TCHAR szFullPath[MAX_PATH];
  if (!::GetModuleFileName(NULL, szFullPath, MAX_PATH)) {
    m_state = false;
    return;
  }

  PathRemoveFileSpec(szFullPath);
  PathAppend(szFullPath, MAX_PATH, lpszFileName);

  FILE *meshFile = _tfopen(szFullPath, _T("rb"));
  if (NULL == meshFile) {
    m_state = false;
    return;
  }

  /*The header holds a brief description of the file, the version number, and
  the number of meshes
  that are stored in the file. This type of files are thought for static meshes
  only*/
  fread(&header, sizeof(GSDHeader), 1, meshFile);

  // Check if there is at least one object
  if (header.numberOfSubObjects < 1) {
    m_state = false;
    fclose(meshFile);
    return;
  }

  GenericObjectData o;

  // we only will use the first object, so we won't iterate over the others, if
  // they exist
  fread(o.Name, sizeof(char) * 128, 1, meshFile); // read the object name
  fread(
      o.ParentName, sizeof(char) * 128, 1,
      meshFile); // Read the name of the parent object (useful for hierarchies)
  fread(&o.iC, sizeof(unsigned long), 1,
        meshFile); // read the number of vertex indices
  fread(&o.vC, sizeof(unsigned long), 1,
        meshFile); // read the number of vertices

  // allocate enough space for the indices and the GLshort version of them
  o.Indices = new unsigned int[o.iC];
  m_mesh.Indices = new GLshort[o.iC];
  fread(o.Indices, sizeof(unsigned int) * o.iC, 1,
        meshFile); // read all indices

  // allocate enough space for the vertices and the GLfixed version of them
  o.Geometry = new float[o.vC * 3];
  m_mesh.Geometry = new GLfixed[o.vC * 3];
  fread(o.Geometry, o.vC * 3 * sizeof(float), 1,
        meshFile); // read all vertices (1 vertex = 3 float4)

  // allocate enough space for the texture coordinates and the GLfixed version
  // of them
  o.TexCoord = new float[o.vC * 2];
  m_mesh.TexCoord = new GLfixed[o.vC * 2];
  fread(o.TexCoord, o.vC * 2 * sizeof(float), 1,
        meshFile); // read all texcoords (1 tex coord = 2 float4)

  // allocate enough space for the normals and the GLfixed version of them
  o.Normals = new float[o.vC * 3];
  m_mesh.Normals = new GLfixed[o.vC * 3];
  fread(o.Normals, o.vC * 3 * sizeof(float), 1,
        meshFile);  // read all normals (1 normal = 3 float4)
  fclose(meshFile); // Do not need the file opened anymore

  // Convert data to optimized data types for OpenGL ES (GLfixed and GLshort)
  for (unsigned int i = 0; i < o.vC * 3; i++) {
    m_mesh.Geometry[i] = FixedFromFloat(o.Geometry[i]);
    m_mesh.Normals[i] = FixedFromFloat(o.Normals[i]);
  }

  for (unsigned i = 0; i < o.vC * 2; i++)
    m_mesh.TexCoord[i] = FixedFromFloat(o.TexCoord[i]);

  for (unsigned i = 0; i < o.iC; i++)
    m_mesh.Indices[i] = (GLshort)o.Indices[i];

  m_mesh.indexCounter = (GLshort)o.iC;
  m_mesh.vertexCounter = (GLshort)o.vC;

  // delete original values, we will use only the optimized ones
  delete[] o.Indices;
  delete[] o.Geometry;
  delete[] o.Normals;
  delete[] o.TexCoord;
}


Mesh::~Mesh() {
  if (m_mesh.Geometry)
    delete[] m_mesh.Geometry;
  if (m_mesh.Indices)
    delete[] m_mesh.Indices;
  if (m_mesh.Normals)
    delete[] m_mesh.Normals;
  if (m_mesh.TexCoord)
    delete[] m_mesh.TexCoord;
}


void Mesh::Draw() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FIXED, 0, m_mesh.Geometry);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FIXED, 0, m_mesh.Normals);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FIXED, 0, m_mesh.TexCoord);

  glDrawElements(GL_TRIANGLES, m_mesh.indexCounter, GL_UNSIGNED_SHORT,
                 m_mesh.Indices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
