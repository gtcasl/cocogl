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
#define __f2x(f) ((int)(f * 65536))
#define __mulx(a, b) ((((int64_t)(a)) * (b)) >> 16)
#define __divx(a, b) (((int64_t)(a) << 16) / (b))
#define __PI 3.14159265358979323846f


bool FileExists(const TCHAR *pszFileName);


HRESULT PathAppend(TCHAR *pszPath, unsigned cchPathSize, const TCHAR *pszMore);

#ifdef _WIN32_WCE


HRESULT PathRemoveFileSpec(TCHAR *pszPath);


HRESULT PathRemoveBackslash(TCHAR *pszPath);
#endif


bool DirectoryExists(const TCHAR *pszDirectoryName);


const TCHAR *FindFileName(const TCHAR *pszPath);


void Ortho2D(int width, int height);


void Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);


void normalizef(float v[3]);


void crossf(float v0[3], float v1[3], float r[3]);


void __identx(GLfixed m[]);


void __identf(GLfloat m[]);


void LookAtf(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
             GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
             GLfloat upz);


bool LoadTexture(LPCTSTR lpszFileName, GLuint *id);


FixedMesh *LoadMeshFromFile(LPCTSTR lpszFileName);


HBITMAP CreatePixmap(HDC hDC);


class CTestBase {
protected:
  CTestBase() {}

  ~CTestBase() {}
};
