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

class GLSurface;
class Surface2D;

class SurfaceDesc {
public:
  const uint8_t *getBits() const { return pbBits_; }

  uint8_t *getBits() { return pbBits_; }

  uint8_t getLogWidth() const { return logWidth_; }

  uint8_t getLogHeight() const { return logHeight_; }

  uint16_t getWidth() const { return 1 << logWidth_; }

  uint16_t getHeight() const { return 1 << logHeight_; }

protected:
  uint8_t *pbBits_;

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint16_t logWidth__logHeight;
    struct {
      uint8_t logWidth_;
      uint8_t logHeight_;
    };
  };
  DISABLE_WARNING_POP
};

struct Sampler {
  const Surface2D *pMipLevels;

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      uint32_t cEnvColor_MaxMipLevel;
    };
    struct {
      uint8_t cEnvColor[3];
      uint8_t MaxMipLevel; // cEnvColor alpha channel
    };
  };
  DISABLE_WARNING_POP
};

class Surface2D : public SurfaceDesc {
public:
  Surface2D() { this->clear(); }

  ~Surface2D() { this->destroy(); }

  GLenum initialize(uint32_t width, uint32_t height, ePixelFormat format);

  GLenum initialize(uint32_t width, uint32_t height, int32_t pitch,
                    ePixelFormat format, GLvoid *pPixels);

  GLenum initialize(uint32_t width, uint32_t height, ePixelFormat format,
                    const GLvoid *pPixels);

  uint32_t getPitch() const { return pitch_; }

  ePixelFormat getFormat() const { return static_cast<ePixelFormat>(format_); }

  void getDesc(GLSurfaceDesc *pSurfaceDesc) const {
    assert(pSurfaceDesc);
    pSurfaceDesc->pBits = pbBits_;
    pSurfaceDesc->Width = this->getWidth();
    pSurfaceDesc->Height = this->getHeight();
    pSurfaceDesc->Pitch = pitch_;
    pSurfaceDesc->Format = format_;
  }

  void destroy();

private:
  void clear();

  int32_t pitch_;
  uint8_t format_;
  bool bOwnedBuffer_;
};

class Texture : public Object {
public:
  TexParams Params;
  bool bGenMipMaps;

  static GLenum Create(Texture **ppTexture);

  const Surface2D &getSurface(uint32_t level) const {
    assert(level < MAX_TEXTURE_LEVELS);
    return surfaces_[level];
  }

  const Surface2D *getSurfaces() const { return surfaces_; }

  ePixelFormat getFormat() const { return surfaces_[0].getFormat(); }

  uint32_t getHandle() const { return handle_; }

  void setHandle(uint32_t handle) { handle_ = handle; }

  GLenum InitializeSurface(uint32_t level, uint32_t width, uint32_t height,
                           ePixelFormat format) {
    if (pBoundSurface_) {
      this->releaseSurface(pBoundSurface_);
    }

    return surfaces_[level].initialize(width, height, format);
  }

  GLenum bindSurface(GLSurface *pSurface, bool bGenMipMaps);

  GLenum releaseSurface(GLSurface *pSurface);

  GLenum generateMipmaps();

  bool validate();

  bool isDirty() const { return bIsDirty_; }

  void Invalidate() { bIsDirty_ = true; }

  uint8_t getLogWidth() const { return surfaces_[0].getLogWidth(); }

  uint8_t getLogHeight() const { return surfaces_[0].getLogHeight(); }

  uint8_t getMaxMipLevel() const { return maxMipLevel_; }

  void freeSurfaces();

private:
  Texture();

  ~Texture();

  Surface2D surfaces_[MAX_TEXTURE_LEVELS];
  uint8_t *pbMipBuffer_;
  GLSurface *pBoundSurface_;
  uint32_t handle_;
  uint8_t maxMipLevel_;
  bool bIsDirty_;
};

struct TexUnit {
public:
  ColorARGB cEnvColor;
  uint8_t EnvMode;
  bool bCoordReplace;

  TexUnit() : pTexture_(nullptr) {}

  ~TexUnit() { __safeRelease(pTexture_); }

  Texture *getTexture() const { return pTexture_; }

  void setTexture(Texture *pTexture) {
    if (pTexture) {
      pTexture->addRef();
    }

    if (pTexture_) {
      pTexture_->release();
    }

    pTexture_ = pTexture;
  }

  ePixelFormat getFormat() const {
    assert(pTexture_);
    return pTexture_->getFormat();
  }

  const TexParams &getParams() const {
    assert(pTexture_);
    return pTexture_->Params;
  }

  bool prepare(Sampler *pSampler, TEXTURESTATES *pStates);

private:
  Texture *pTexture_;
};
