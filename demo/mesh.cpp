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

Mesh::Mesh()
    : num__indices(0), num__vertices(0), pIndices_(nullptr),
      pGeometry_(nullptr), pNormals_(nullptr), pTexCoords_(nullptr) {}

Mesh::~Mesh() {
  delete[] pIndices_;
  delete[] pGeometry_;
  delete[] pNormals_;
  delete[] pTexCoords_;
}

bool Mesh::OnInitialize(const char *fileName) {
  FILE *file = nullptr;
  uint32_t num_indices = 0;
  uint32_t num_vertices = 0;
  uint32_t *indices = nullptr;
  size_t size = 0;
  bool status = false;

  for (;;) {
    GSDHeader header;

    auto file = fopen(fileName, "rb");
    if (nullptr == file)
      break;

    // load file header
    size = fread(&header, 1, sizeof(GSDHeader), file);

    // Check if there is at least one object
    if (header.num_sub_objs < 1)
      break;

    // read the object name
    size = fread(name_, 1, sizeof(char) * 128, file);
    if (size != sizeof(char) * 128)
      break;

    // Read the name of the parent object
    size = fread(parentName_, 1, sizeof(char) * 128, file);
    if (size != sizeof(char) * 128)
      break;

    // read the number of vertex indices
    size = fread(&num_indices, 1, sizeof(uint32_t), file);
    if (size != sizeof(uint32_t))
      break;

    // read the number of vertices
    size = fread(&num_vertices, 1, sizeof(uint32_t), file);
    if (size != sizeof(uint32_t))
      break;

    // read indices
    auto indices = new uint32_t[num_indices];
    pIndices_ = new GLshort[num_indices];
    size = fread(indices, 1, sizeof(uint32_t) * num_indices, file);
    if (size != sizeof(uint32_t) * num_indices)
      break;

    // read vertex data (1 vertex = 3 float4)
    pGeometry_ = new float[num_vertices * 3];
    size = fread(pGeometry_, 1, num_vertices * 3 * sizeof(float), file);
    if (size != num_vertices * 3 * sizeof(float))
      break;

    // read texture coordinates  (1 texcoord = 2 float4)
    pTexCoords_ = new float[num_vertices * 2];
    size = fread(pTexCoords_, 1, num_vertices * 2 * sizeof(float), file);
    if (size != num_vertices * 2 * sizeof(float))
      break;

    // read normals data (1 normal = 3 float4)
    pNormals_ = new float[num_vertices * 3];
    size = fread(pNormals_, 1, num_vertices * 3 * sizeof(float), file);
    if (size != num_vertices * 3 * sizeof(float))
      break;

    // Convert data to optimized data types for OpenGL ES (GLfixed and GLshort)
    for (uint32_t i = 0; i < num_indices; i++) {
      pIndices_[i] = static_cast<GLshort>(indices[i]);
    }

    num__indices = num_indices;
    num__vertices = num_vertices;
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

void Mesh::OnRender() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, pGeometry_);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, pNormals_);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, pTexCoords_);

  glDrawElements(GL_TRIANGLES, num__indices, GL_UNSIGNED_SHORT, pIndices_);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}