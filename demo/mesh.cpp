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

struct GSDHeader {
  char id[32];
  char version[16];
  int num_sub_objs;
};

CMesh::CMesh() 
: m_num_indices(0)
, m_num_vertices(0)
, m_pIndices(nullptr)
, m_pGeometry(nullptr)
, m_pNormals(nullptr)
, m_pTexCoords(nullptr)
{}

CMesh::~CMesh() {
  delete[] m_pIndices;
  delete[] m_pGeometry;
  delete[] m_pNormals;
  delete[] m_pTexCoords;
}

bool CMesh::OnInitialize(LPCTSTR lpszFileName) {
  FILE* file = nullptr;
  uint32_t num_indices = 0;
  uint32_t num_vertices = 0;
  uint32_t* indices = nullptr;
  size_t size = 0;
  bool status = false;

  for (;;) {
    GSDHeader header;

    auto file = _tfopen(lpszFileName, _T("rb"));
    if (nullptr == file)
      break;

    // load file header
    size = fread(&header, 1, sizeof(GSDHeader), file);

    // Check if there is at least one object
    if (header.num_sub_objs < 1)
      break;

    // read the object name  
    size = fread(m_name, 1, sizeof(char) * 128, file); 
    if (size != sizeof(char) * 128)
      break;

    // Read the name of the parent object
    size = fread(m_parentName, 1, sizeof(char) * 128, file); 
    if (size != sizeof(char) * 128)
      break;

    // read the number of vertex indices
    size = fread(&num_indices, 1, sizeof(uint32_t),  file); 
    if (size != sizeof(uint32_t))
      break;

    // read the number of vertices
    size = fread(&num_vertices, 1, sizeof(uint32_t), file); 
    if (size != sizeof(uint32_t))
      break;

    // read indices
    auto indices = new uint32_t[num_indices];
    m_pIndices = new GLshort[num_indices];
    size = fread(indices, 1, sizeof(uint32_t) * num_indices, file);
    if (size != sizeof(uint32_t) * num_indices)
      break;

    // read vertex data (1 vertex = 3 float4)
    m_pGeometry = new float[num_vertices * 3];
    size = fread(m_pGeometry, 1, num_vertices * 3 * sizeof(float), file);
    if (size != num_vertices * 3 * sizeof(float))
      break;

    // read texture coordinates  (1 texcoord = 2 float4)
    m_pTexCoords = new float[num_vertices * 2];
    size = fread(m_pTexCoords, 1, num_vertices * 2 * sizeof(float), file);
    if (size != num_vertices * 2 * sizeof(float))
      break;

    // read normals data (1 normal = 3 float4)
    m_pNormals = new float[num_vertices * 3];
    size = fread(m_pNormals, 1, num_vertices * 3 * sizeof(float), file);
    if (size != num_vertices * 3 * sizeof(float))
      break;

    // Convert data to optimized data types for OpenGL ES (GLfixed and GLshort)
    for (uint32_t i = 0; i < num_indices; i++) {
      m_pIndices[i] = (GLshort)indices[i];
    }

    m_num_indices = num_indices;
    m_num_vertices = num_vertices;
    status = true;
    break;
  }

  if (file) {
    fclose(file); 
  }

  // release temp allocations
  delete[] indices;

  return status;
}

void CMesh::OnRender() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m_pGeometry);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, m_pNormals);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, m_pTexCoords);

  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_SHORT, m_pIndices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}