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

struct PrimitiveData;
struct RasterData;
struct Register;

typedef void (*PfnScanline)(const RasterData &rasterData, int y, int lx,
                            int rx);

class IRasterOp : public Object {
public:
  virtual PfnScanline getScanline() const = 0;

  virtual uint32_t getCbSize() const = 0;

#ifdef COCOGL_RASTER_PROFILE
public:
  void startProfile(uint32_t numPixels);
  void endProfile(float elapsedTime);
  void logProfile(const RASTERID &rasterID);
  const ProfileCounter &getProfile() const {
    return profile_;
  }

private:
  ProfileCounter profile_;
#endif
};

class RasterCache : public Object {
public:
  ~RasterCache() {
#ifndef NDEBUG
    if (slowRasterIDs_.size() != 0) {
      __glLog("BEGIN MAKE_SCANLINE().\r\n");
      for (auto &rid : slowRasterIDs_) {
        __glLog("MAKE_SCANLINE(%d,%d,%d,%d),\r\n", rid.Flags.Value,
                rid.States.Value, rid.Textures[0].Value, rid.Textures[1].Value);
      }
      __glLog("END MAKE_SCANLINE().\r\n");
    }
#endif
#ifdef COCOGL_RASTER_PROFILE
    {
      // Copy the cache list
      Cache::List sortedList;
      if (SUCCEEDED(map_.ToList(&sortedList))) {
        // Sort the list
        sortedList.Sort(Profilecompare);

        // Dump the profile result
        for (Cache::Iter iter = sortedList.getBegin(),
                         iterEnd = sortedList.getEnd();
             iter != iterEnd; ++iter) {
          iter->Second->LogProfile(iter->First);
        }
      }
    }
#endif

    // Delete cache entries
    for (auto &it : map_) {
      __safeRelease(it.second);
    }
  }

  bool lookup(const RASTERID &rasterID, IRasterOp **ppRasterOp) {
    assert(ppRasterOp);
    auto it = map_.find(rasterID);
    if (it == map_.end())
      return false;
    *ppRasterOp = it->second;
    return true;
  }

  void insert(const RASTERID &rasterID, IRasterOp *pRasterOp) {
    uint32_t cbTotalSize = cbTotalSize_;
    if (cbTotalSize > MAX_CACHE_SIZE) {
      uint32_t compactSize = cbTotalSize / COMPACT_RATIO;
      if (cbTotalSize > compactSize) {
        for (auto iter = map_.rbegin(); cbTotalSize > compactSize;) {
          auto iterCur = iter--;
          cbTotalSize -= iterCur->second->getCbSize();
          __safeRelease(iterCur->second);
          map_.erase(iterCur->first);
        }
        cbTotalSize_ = cbTotalSize;
      }
    }

    uint32_t cbSize = pRasterOp->getCbSize();
    cbTotalSize_ += cbSize;
    map_.insert(std::make_pair(rasterID, pRasterOp));
  }

  static GLenum Create(RasterCache **ppRasterCache) {
    __profileAPI(" - %s()\n", __FUNCTION__);

    assert(ppRasterCache);

    // Create a new raster cache
    RasterCache *pRasterCache = new RasterCache();
    if (nullptr == pRasterCache) {
      __glLogError("RasterCache allocation failed, out of memory.\r\n");
      return GL_OUT_OF_MEMORY;
    }

    pRasterCache->addRef();
    *ppRasterCache = pRasterCache;

    return GL_NO_ERROR;
  }

  void trackSlowRasterID(const RASTERID &rasterID) {
    for (auto &rid : slowRasterIDs_) {
      if (rid == rasterID) {
        return;
      }
    }
    slowRasterIDs_.push_back(rasterID);
  }

private:
#ifdef COCOGL_RASTER_PROFILE

  static bool profilecompare(const Cache::List::Type &lhs,
                             const Cache::List::Type &rhs) {
    return (lhs.Second->getProfile() < rhs.Second->getProfile());
  }

#endif

  enum {
    MAX_CACHE_SIZE = 0x20000,
    COMPACT_RATIO = 4,
  };

  RasterCache() {
    cbTotalSize_ = 0;
  }

  std::map<RASTERID, IRasterOp *> map_;

  std::list<RASTERID> slowRasterIDs_;

  uint32_t cbTotalSize_;
};

struct Register {
  enum { DIM = 3 };
  fixedRX m[DIM];
};

struct RasterData {
  fixed4 fRefX;
  fixed4 fRefY;

  Register Registers[MAX_REGISTERS];
  Sampler Samplers[MAX_TEXTURES];

  uint8_t *pColorBits;
  int32_t ColorPitch;

  uint8_t *pDepthStencilBits;
  int32_t DepthStencilPitch;

  ColorARGB cFogColor;

  uint8_t AlphaRef;
  uint8_t StencilRef;
  uint8_t StencilMask;
  uint8_t StencilWriteMask;

  uint32_t ColorWriteMask;

  uint8_t ColorFormat;
  uint8_t DepthStencilFormat;
  uint16_t __padding;

  IRasterOp *pRasterOp;

  RasterData()
      : pRasterOp(nullptr) {}
};