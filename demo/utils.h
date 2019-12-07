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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

void LookAtf(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
             GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
             GLfloat upz);

GLuint loadTexture(const std::vector<uint8_t> &pixels, int width, int height,
                   int bpp);

bool LoadTGA(const char *filename, std::vector<uint8_t> &pixels, int *width,
             int *height, int *bpp);
