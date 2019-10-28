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
#include "shared.hpp"


bool FileExists(const TCHAR *pszFileName) {
  // Check input parameters
  if ((NULL == pszFileName) || (0 == *pszFileName)) {
    return false;
  }

  uint32_t dwFileAttr = GetFileAttributes(pszFileName);
  if ((0xFFFFFFFF != dwFileAttr) && !(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
    return true;
  }

  return false;
}


HRESULT
PathAppend(TCHAR *pszPath, unsigned cchPathSize, const TCHAR *pszMore) {
  // Check input parameters
  if (NULL == pszPath) {
    return E_INVALIDARG;
  }

  // Special case
  if ((NULL == pszMore) || (NULL == *pszMore)) {
    return S_OK;
  }

  size_t cLen1 = _tcslen(pszPath);
  const size_t cLen2 = _tcslen(pszMore);

  if (cLen1 + cLen2 + 1 > cchPathSize) {
    return E_INVALIDARG;
  }

  if (_T('\\') != pszPath[cLen1 - 1]) {
    _tcscpy(pszPath + cLen1++, _T("\\"));
  }

  _tcscpy(&pszPath[cLen1], pszMore);

  return S_OK;
}

#ifdef _WIN32_WCE


HRESULT
PathRemoveFileSpec(TCHAR *pszPath) {
  // Check input parameters
  if ((NULL == pszPath) || (0 == *pszPath)) {
    return E_INVALIDARG;
  }

  TCHAR *const pszLastBkSlash = _tcsrchr(pszPath, _T('\\'));
  if (pszLastBkSlash && (0 != pszLastBkSlash[1])) {
    pszLastBkSlash[1] = 0;
  }

  return S_OK;
}


HRESULT
PathRemoveBackslash(TCHAR *pszPath) {
  // Check input parameters
  if ((NULL == pszPath) || (0 == *pszPath)) {
    return E_INVALIDARG;
  }

  TCHAR *const pszLastBkSlash = _tcsrchr(pszPath, _T('\\'));
  if (pszLastBkSlash && (0 == pszLastBkSlash[1])) {
    pszLastBkSlash[0] = 0;
  }

  return S_OK;
}

#endif


bool DirectoryExists(const TCHAR *pszDirectoryName) {
  // Check input parameters
  if ((NULL == pszDirectoryName) || (0 == *pszDirectoryName)) {
    return false;
  }

  uint32_t dwFileAttr = GetFileAttributes(pszDirectoryName);
  if ((0xFFFFFFFF != dwFileAttr) && (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
    return true;
  } else {
    return false;
  }
}


const TCHAR *FindFileName(const TCHAR *pszPath) {
  // Check input parameters
  if ((NULL == pszPath) || (0 == *pszPath)) {
    return NULL;
  }

  const TCHAR *pszLastBkSlash = _tcsrchr(pszPath, _T('\\'));
  if (pszLastBkSlash) {
    return ++pszLastBkSlash;
  }

  return pszPath;
}

//////////////////////////////////////////////////////////////////////////////


void Ortho2D(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrthox(FixedFromInt(0), FixedFromInt(width), FixedFromInt(0),
           FixedFromInt(height), FixedFromInt(-1), FixedFromInt(1));
  glMatrixMode(GL_MODELVIEW);
}


void Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
  GLfixed xmin, xmax, ymin, ymax, aspectFixed, znearFixed;

  aspectFixed = FixedFromFloat(aspect);
  znearFixed = FixedFromFloat(zNear);

  ymax = MultiplyFixed(
      znearFixed, FixedFromFloat((GLfloat)tan(fovy * 3.1415962f / 360.0f)));
  ymin = -ymax;

  xmin = MultiplyFixed(ymin, aspectFixed);
  xmax = MultiplyFixed(ymax, aspectFixed);
  glFrustumx(xmin, xmax, ymin, ymax, znearFixed, FixedFromFloat(zFar));
}


void normalizef(float v[3]) {
  float r;
  r = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (r == 0.0)
    return;
  r = 1.f / r;
  v[0] *= r;
  v[1] *= r;
  v[2] *= r;
}


void crossf(float v0[3], float v1[3], float r[3]) {
  r[0] = v0[1] * v1[2] - v0[2] * v1[1];
  r[1] = v0[2] * v1[0] - v0[0] * v1[2];
  r[2] = v0[0] * v1[1] - v0[1] * v1[0];
}


void __identf(GLfloat m[]) {
  memset(m, 0, sizeof m[0] * 16);
  m[0] = m[5] = m[10] = m[15] = 1.0f;
}


void __identx(GLfixed m[]) {
  memset(m, 0, sizeof m[0] * 16);
  m[0] = m[5] = m[10] = m[15] = __f2x(1.0f);
}


void LookAtf(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
             GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
             GLfloat upz) {
  GLfloat forward[3], side[3], up[3];
  GLfloat m[4][4];

  forward[0] = centerx - eyex;
  forward[1] = centery - eyey;
  forward[2] = centerz - eyez;

  normalizef(forward);

  up[0] = upx;
  up[1] = upy;
  up[2] = upz;

  crossf(forward, up, side);

  normalizef(side);
  crossf(side, forward, up);

  __identf(&m[0][0]);
  m[0][0] = side[0];
  m[1][0] = side[1];
  m[2][0] = side[2];

  m[0][1] = up[0];
  m[1][1] = up[1];
  m[2][1] = up[2];

  m[0][2] = -forward[0];
  m[1][2] = -forward[1];
  m[2][2] = -forward[2];

  glMultMatrixf(&m[0][0]);
  glTranslatef(-eyex, -eyey, -eyez);
}


bool LoadTexture(LPCTSTR lpszFileName, GLuint *id) {
  TCHAR szFullPath[MAX_PATH];

#ifdef _WIN32_WCE
  if (!::GetModuleFileName(NULL, szFullPath, MAX_PATH)) {
    return false;
  }

  PathRemoveFileSpec(szFullPath);
#else
  _sntprintf(szFullPath, MAX_PATH, _T("..\\media\\"));
#endif
  PathAppend(szFullPath, MAX_PATH, lpszFileName);

  FILE *f = _tfopen(szFullPath, _T("rb"));
  if (NULL == f) {
    return false;
  }

  GLubyte *pixels = NULL;

  uint16_t width = 0, height = 0;
  byte headerLength = 0;
  byte imageType = 0;
  byte bits = 0;
  int format = 0;
  int lineWidth = 0;

  fread(&headerLength, sizeof(byte), 1, f);

  // skip next byte
  fseek(f, 1, SEEK_CUR);

  // read in the imageType (RLE, RGB, etc...)
  fread(&imageType, sizeof(byte), 1, f);

  // skip information we don't care about
  fseek(f, 9, SEEK_CUR);

  // read the width, height and bits per pixel (16, 24 or 32). We only will take
  // care of 24 bits uncompressed TGAs
  fread(&width, sizeof(uint16_t), 1, f);
  fread(&height, sizeof(uint16_t), 1, f);
  fread(&bits, sizeof(byte), 1, f);

  // move the file pointer to the pixel data
  fseek(f, headerLength + 1, SEEK_CUR);

  // check if the image is not compressed.
  if (imageType != 10) {
    switch (bits) {
    case 24:
    case 32: {
      format = bits >> 3; // Another way to divide between 8. We want to know
                          // the pixel size in bytes.
      lineWidth = format * width;
      pixels = new GLubyte[lineWidth * height];

      // we are going to load the pixel data line by line
      for (int y = 0; y < height; ++y) {
        // Read current line of pixels
        GLubyte *line = &pixels[lineWidth * y];
        fread(line, lineWidth, 1, f);

        /*Because the TGA is BGR instead of RGB, we must swap the R and G
      components (OGL ES does not have the
      GL_BGR_EXT extension*/
        for (int i = 0; i < lineWidth; i += format) {
          GLubyte temp = line[i];
          line[i] = line[i + 2];
          line[i + 2] = temp;
        }
      }

      break;
    }

    default:
      fclose(f);
      *id = 0;
      return false;
    }
  }

  fclose(f);

  glGenTextures(1, id);
  glBindTexture(GL_TEXTURE_2D, *id);
  glTexParameterx(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

  switch (bits) {
  case 24:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pixels);
    break;

  case 32:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
    break;
  }

  delete[] pixels;

  return (GL_NO_ERROR == glGetError());
}


FixedMesh *LoadMeshFromFile(LPCTSTR lpszFileName) {
  TCHAR szFullPath[MAX_PATH];

#ifdef _WIN32_WCE
  if (!::GetModuleFileName(NULL, szFullPath, MAX_PATH)) {
    return false;
  }

  PathRemoveFileSpec(szFullPath);
#else
  _sntprintf(szFullPath, MAX_PATH, _T("..\\media\\"));
#endif
  PathAppend(szFullPath, MAX_PATH, lpszFileName);

  FILE *pFile = _tfopen(szFullPath, _T("rb"));
  if (NULL == pFile) {
    return NULL;
  }

  GSDHeader header;

  /*
  The header holds a brief description of the file, the version number, and the
  number of meshes
  that are stored in the file. This type of files are thought for static meshes
  only
  */
  fread(&header, sizeof(GSDHeader), 1, pFile);

  // Check if there is at least one object
  if (header.numberOfSubObjects < 1) {
    return NULL;
  }

  FixedMesh *pMesh = new FixedMesh;
  if (NULL == pMesh) {
    return NULL;
  }

  GenericObjectData o;

  // we only will use the first object, so we won't iterate over the others, if
  // they exist
  fread(o.Name, sizeof(char) * 128, 1, pFile); // read the object name
  fread(o.ParentName, sizeof(char) * 128, 1,
        pFile); // Read the name of the parent object (useful for hierarchies)
  fread(&o.iC, sizeof(unsigned long), 1,
        pFile); // read the number of vertex indices
  fread(&o.vC, sizeof(unsigned long), 1, pFile); // read the number of vertices

  // allocate enough space for the indices and the GLshort version of them
  o.Indices = new unsigned int[o.iC];
  pMesh->Indices = new GLshort[o.iC];
  fread(o.Indices, sizeof(unsigned int) * o.iC, 1, pFile); // read all indices

  // allocate enough space for the vertices and the GLfixed version of them
  o.Geometry = new float[o.vC * 3];
  pMesh->Geometry = new GLfixed[o.vC * 3];
  fread(o.Geometry, o.vC * 3 * sizeof(float), 1,
        pFile); // read all vertices (1 vertex = 3 float4)

  // allocate enough space for the texture coordinates and the GLfixed version
  // of them
  o.TexCoord = new float[o.vC * 2];
  pMesh->TexCoord = new GLfixed[o.vC * 2];
  fread(o.TexCoord, o.vC * 2 * sizeof(float), 1,
        pFile); // read all texcoords (1 tex coord = 2 float4)

  // allocate enough space for the normals and the GLfixed version of them
  o.Normals = new float[o.vC * 3];
  pMesh->Normals = new GLfixed[o.vC * 3];
  fread(o.Normals, o.vC * 3 * sizeof(float), 1,
        pFile);  // read all normals (1 normal = 3 float4)
  fclose(pFile); // Do not need the file opened anymore

  // Convert data to optimized data types for OpenGL ES (GLfixed and GLshort)
  for (unsigned i = 0; i < o.vC * 3; ++i) {
    pMesh->Geometry[i] = FixedFromFloat(o.Geometry[i]);
    pMesh->Normals[i] = FixedFromFloat(o.Normals[i]);
  }

  for (unsigned i = 0; i < o.vC * 2; ++i) {
    pMesh->TexCoord[i] = FixedFromFloat(o.TexCoord[i]);
  }

  for (unsigned i = 0; i < o.iC; ++i) {
    pMesh->Indices[i] = (GLshort)o.Indices[i];
  }

  pMesh->indexCounter = (GLshort)o.iC;
  pMesh->vertexCounter = (GLshort)o.vC;

  // delete original values, we will use only the optimized ones
  delete[] o.Indices;
  delete[] o.Geometry;
  delete[] o.Normals;
  delete[] o.TexCoord;

  return pMesh;
}


HBITMAP
CreatePixmap(HDC hDC) {
  struct BitmapInfo {
    BITMAPINFOHEADER bmiHeader;
    uint32_t bmiColors[3];
  } bmpInfo;

  const unsigned nBPP = ::GetDeviceCaps(hDC, BITSPIXEL);
  const unsigned width = ::GetDeviceCaps(hDC, HORZRES);
  const unsigned height = ::GetDeviceCaps(hDC, VERTRES);

  // Build the bitmap info header
  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = width;
  bmpInfo.bmiHeader.biHeight = height;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biBitCount = static_cast<uint16_t>(nBPP);
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage = width * height * (nBPP >> 3);
  bmpInfo.bmiHeader.biXPelsPerMeter = 0;
  bmpInfo.bmiHeader.biYPelsPerMeter = 0;
  bmpInfo.bmiHeader.biClrUsed = 0;
  bmpInfo.bmiHeader.biClrImportant = 0;

  if (16 == nBPP) {
    bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;
    bmpInfo.bmiColors[0] = 0xF800;
    bmpInfo.bmiColors[1] = 0x07E0;
    bmpInfo.bmiColors[2] = 0x001F;
  }

  // Create the DBI section
  void *pBits;
  return ::CreateDIBSection(hDC, reinterpret_cast<BITMAPINFO *>(&bmpInfo),
                            DIB_RGB_COLORS, &pBits, NULL, 0);
}
