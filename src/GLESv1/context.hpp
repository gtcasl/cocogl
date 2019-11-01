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

#include "raster.hpp"

class CGLContext : public CRasterizer {
public:
  //--
  static GLenum Create(CGLContext **ppContext, CHandleTable *pHandles,
                       CRasterCache *pRasterCache, CGLContext *pCtxShared);

  //--
  void SetDrawSurface(CGLSurface *pSurface);
  void SetReadSurface(CGLSurface *pSurface);

  //--
  void SetError(GLenum error);
  GLenum GetError() const;

  //--
  void ClearColor(floatf red, floatf green, floatf blue, floatf alpha);
  void ClearDepth(floatf depth);
  void ClearStencil(GLint stencil);

  void ColorMask(GLboolean red, GLboolean green, GLboolean blue,
                 GLboolean alpha);

  void DepthMask(GLboolean flag);
  void StencilMask(GLuint mask);

  void ClientState(GLenum array, bool bValue);
  void ClientActiveTexture(GLenum texture);
  void ActiveTexture(GLenum texture);

  void VertexPointer(GLint size, GLenum type, GLsizei stride,
                     const GLvoid *pPointer);

  void NormalPointer(GLenum type, GLsizei stride, const GLvoid *pPointer);

  void ColorPointer(GLint size, GLenum type, GLsizei stride,
                    const GLvoid *pPointer);

  void TexCoordPointer(GLint size, GLenum type, GLsizei stride,
                       const GLvoid *pPointer);

  void PointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pPointer);

  void Color(floatf red, floatf green, floatf blue, floatf alpha);
  void Normal(floatf nx, floatf ny, floatf nz);
  void MultiTexCoord(GLenum target, floatf s, floatf t, floatf r, floatf q);

  void GenBuffers(GLsizei n, GLuint *phBuffers);
  void BindBuffer(GLenum target, GLuint buffer);

  void BufferData(GLenum target, GLsizeiptr size, const GLvoid *pData,
                  GLenum usage);

  void BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                     const GLvoid *pData);

  void GetBufferParameter(GLenum target, GLenum pname, GLint *pParams);
  void DeleteBuffers(GLsizei n, const GLuint *phBuffers);

  void GenTextures(GLsizei n, GLuint *phTextures);
  void BindTexture(GLenum target, GLuint texture);

  void TexImage2D(GLenum target, GLint level, GLint internalformat,
                  GLsizei width, GLsizei height, GLint border, GLenum format,
                  GLenum type, const GLvoid *pPixels);

  void TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                     GLsizei width, GLsizei height, GLenum format, GLenum type,
                     const GLvoid *pPixels);

  void CopyTexImage2D(GLenum target, GLint level, GLenum internalformat,
                      GLint x, GLint y, GLsizei width, GLsizei height,
                      GLint border);

  void CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                         GLint yoffset, GLint x, GLint y, GLsizei width,
                         GLsizei height);

  void CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
                            GLsizei width, GLsizei height, GLint border,
                            GLsizei imageSize, const GLvoid *pData);

  void CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                               GLint yoffset, GLsizei width, GLsizei height,
                               GLenum format, GLsizei imageSize,
                               const GLvoid *pData);

  void DeleteTextures(GLsizei n, const GLuint *phTextures);
  void PixelStorei(GLenum pname, GLint param);

  void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                  GLenum format, GLenum type, GLvoid *pPixels);

  GLenum BindTexImage(CGLSurface *pSurface, bool bGenMipMaps);
  GLenum ReleaseTexImage(CGLSurface *pSurface);

  void MatrixMode(GLenum mode);
  void PushMatrix();
  void PopMatrix();
  void LoadIdentity();
  void LoadMatrix(const MATRIX44 &matrix);

  void Ortho(floatf left, floatf right, floatf bottom, floatf top, floatf zNear,
             floatf zFar);

  void Scale(floatf x, floatf y, floatf z);
  void Translate(floatf x, floatf y, floatf z);
  void Rotate(floatf angle, floatf x, floatf y, floatf z);
  void Multiply(const MATRIX44 &matrix);
  void Frustum(floatf left, floatf right, floatf bottom, floatf top,
               floatf zNear, floatf zFar);

  template <class T> GLbitfield TQueryMatrix(T *pMantissa, GLint exponent[16]);

  void DepthRange(floatf zNear, floatf zFar);
  void ClipPlane(GLenum plane, const VECTOR4 &equation);
  void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

  template <class T> void TLight(GLenum light, GLenum pname, const T *pParams);

  template <class T> void TLightModel(GLenum pname, const T *pParams);

  template <class T>
  void TMaterial(GLenum face, GLenum pname, const T *pParams);

  template <class T> void TFog(GLenum pname, const T *pParams);

  template <class T>
  void TTexParameter(GLenum target, GLenum pname, const T *pParams);

  template <class T>
  void TTexEnv(GLenum target, GLenum pname, const T *pParams);

  void AlphaFunc(GLenum func, floatf ref);
  void StencilFunc(GLenum func, GLint ref, GLuint mask);
  void StencilOp(GLenum fail, GLenum zfail, GLenum zpass);
  void DepthFunc(GLenum func);
  void BlendFunc(GLenum sfactor, GLenum dfactor);
  void LogicOp(GLenum opcode);

  void ShadeModel(GLenum mode);

  void FrontFace(GLenum mode);

  void CullFace(GLenum mode);

  void Scissor(GLint x, GLint y, GLsizei width, GLsizei height);

  void Activate(GLenum cap, bool bValue);
  void ActivateLight(GLenum cap, bool bValue);

  void SampleCoverage(floatf value, GLboolean invert);

  void Hint(GLenum target, GLenum mode);

  void PolygonOffset(floatf factor, floatf units);

  void PointSize(floatf size);
  void LineWidth(floatf width);

  //--
  const GLubyte *GetString(GLenum name);

  //--
  template <class T> void TGetClipPlane(GLenum plane, T eqn[4]);

  template <class T> void TGet(GLenum pname, T *pParams);

  template <class T> void TGetLight(GLenum light, GLenum pname, T *pParams);

  template <class T> void TGetMaterial(GLenum face, GLenum pname, T *pParams);

  template <class T> void TGetTexEnv(GLenum env, GLenum pname, T *pParams);

  template <class T>
  void TGetTexParameter(GLenum target, GLenum pname, T *pParams);

  void GetPointer(void **ppParams, GLenum pname);
  bool IsEnabled(GLenum cap);
  bool IsBuffer(GLuint buffer);
  bool IsTexture(GLuint texture);

  template <class T> void TPointParameter(GLenum pname, const T *pParams);

  void Clear(GLbitfield mask);

  void DrawArrays(GLenum mode, GLint first, GLsizei count);

  void DrawElements(GLenum mode, GLsizei count, GLenum type,
                    const GLvoid *pIndices);

  void Flush();
  void Finish();

private:
  CGLContext(CHandleTable *pHandles, CRasterCache *pRasterCache,
             CGLContext *pSharedCtx);

  ~CGLContext();

  GLenum Initialize();
};
