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

template <typename T>
GLenum ToGLenum(T param);

template <typename T>
class AddressOf {
public:
  AddressOf(const T &value)
      : value_(value) {}

  operator const T *() const {
    return &value_;
  }

  operator T *() {
    return &value_;
  }

private:
  T value_;
};

#ifndef COCOGL_API_TRACE
#define __profileAPI(func, ...)
#else
#define __profileAPI(func, ...) AutoLog profiler(g_logger, func, __VA_ARGS__);
#endif

inline bool __glFailed(uint32_t err) {
  return (err != GL_NO_ERROR);
}

inline bool __glSucceeded(uint32_t err) {
  return (err == GL_NO_ERROR);
}

#ifndef NDEBUG
#define __glLog(...) g_logger.write(__VA_ARGS__);

#define __glLogError(...)                                                     \
  g_logger.write("*** Error in file %s at line %d.\r\n", __FILE__, __LINE__); \
  g_logger.write(__VA_ARGS__);                                                \
  assert(false);
#else
#define __glLog(...)
#define __glLogError(...)
#endif
#define __glError(error, ...) \
  __glLogError(__VA_ARGS__);  \
  this->setError(error);

void memset16(void *dst, int fill, int cwCount);
void memset32(void *dst, int fill, int cwCount);

uint32_t GLSizeOf(GLenum type);

eVertexFormat ToVertexFormat(GLenum type, GLint size);

uint32_t VertexDataSize(eVertexFormat format);

GLenum DecodeDataType(eVertexFormat format);

GLint DecodeDataSize(eVertexFormat format);

GLenum GLERROR_FROM_HRESULT(HRESULT hr);

GLenum ToPixelFormat(ePixelFormat *pOut, uint32_t *pBPP, GLenum format,
                     GLenum type);

GLenum ToPixelFormat(ePixelFormat *pOut, GLint format);

ecompare Reverse(ecompare compare);

uint32_t compareFuncFromEnum(GLenum func);
GLenum EnumFromcompareFunc(uint32_t func);

uint32_t StencilOpFromEnum(GLenum op);
GLenum EnumFromStencilOp(uint32_t op);

uint32_t BlendFuncFromEnum(GLenum func);
GLenum EnumFromBlendFunc(uint32_t func);

uint32_t LogicOpFromEnum(GLenum op);
GLenum EnumFromLogicOp(uint32_t op);

uint32_t FogModeFromEnum(GLenum mode);
GLenum EnumFromFogMode(uint32_t mode);

uint32_t ShadeModelFromEnum(GLenum mode);
GLenum EnumFromShadeModel(uint32_t mode);

uint32_t CullFaceFromEnum(GLenum mode);
GLenum EnumFromCullFace(uint32_t mode);

uint32_t FrontFaceFromEnum(GLenum mode);
GLenum EnumFromFrontFace(uint32_t mode);

uint32_t HintFromEnum(GLenum mode);
GLenum EnumFromHint(uint32_t mode);

uint32_t TexFilterFromEnum(GLenum param);
GLenum EnumFromTexFilter(uint32_t param);

uint32_t TexAddressFromEnum(GLenum param);
GLenum EnumFromTexAddress(uint32_t param);

eEnvMode TexEnvFromEnum(GLenum param);
GLenum EnumFromTexEnv(uint32_t param);

GLenum CopyBuffers(const GLSurfaceDesc &dstDesc, int32_t dstOffsetX,
                   int32_t dstOffsetY, int32_t copyWidth, int32_t copyHeight,
                   const GLSurfaceDesc &srcDesc, int32_t srcOffsetX,
                   int32_t srcOffsetY);

template <>
inline GLenum ToGLenum<float>(float param) {
  return static_cast<GLenum>(param);
}

template <>
inline GLenum ToGLenum<fixed16>(fixed16 param) {
  return *reinterpret_cast<const GLenum *>(&param);
}

template <>
inline GLenum ToGLenum<int>(int param) {
  return static_cast<GLenum>(param);
}

template <typename T, uint32_t BITS>
class BitPtr {
public:
  enum {
    MASK = (1 << BITS) - 1,
  };

  BitPtr(const void *ptr) {
    ptr_ = reinterpret_cast<const T *>(ptr);
    shift_ = (sizeof(T) << 3) - BITS;
  }

  T operator*() const {
    return (*ptr_ >> shift_) & MASK;
  }

  const BitPtr &operator++() {
    if (shift_ >= BITS) {
      shift_ -= BITS;
    } else {
      shift_ = (sizeof(T) << 3) - BITS;
      ++ptr_;
    }

    return *this;
  }

private:
  const T *ptr_;
  uint32_t shift_;
};
