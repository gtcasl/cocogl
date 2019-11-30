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

typedef void (*PFN_Scanline)(const RasterData &rasterData, int y, int lx,
                             int rx);

class IRasterOp : public CObject {
public:
  virtual PFN_Scanline GetScanline() const = 0;

  virtual uint32_t GetCbSize() const = 0;

#ifdef COCOGL_RASTER_PROFILE
public:
  void StartProfile(uint32_t numPixels);
  void EndProfile(float elapsedTime);
  void LogProfile(const RASTERID &rasterID);
  const ProfileCounter &GetProfile() const { return profile_; }

private:
  ProfileCounter profile_;
#endif
};

class CRasterCache : public CObject {
public:
  ~CRasterCache() {
#ifndef NDEBUG
    if (slowRasterIDs_.size() != 0) {
      __glLog(_T("BEGIN MAKE_SCANLINE().\r\n"));
      for (auto &rid : slowRasterIDs_) {
        __glLog(_T("MAKE_SCANLINE(%d,%d,%d,%d),\r\n"), rid.Flags.Value,
                rid.States.Value, rid.Textures[0].Value, rid.Textures[1].Value);
      }
      __glLog(_T("END MAKE_SCANLINE().\r\n"));
    }
#endif
#ifdef COCOGL_RASTER_PROFILE
    {
      // Copy the cache list
      Cache::List sortedList;
      if (SUCCEEDED(map_.ToList(&sortedList))) {
        // Sort the list
        sortedList.Sort(ProfileCompare);

        // Dump the profile result
        for (Cache::Iter iter = sortedList.GetBegin(),
                         iterEnd = sortedList.GetEnd();
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

  bool Lookup(const RASTERID &rasterID, IRasterOp **ppRasterOp) {
    assert(ppRasterOp);
    auto it = map_.find(rasterID);
    if (it == map_.end())
      return false;
    *ppRasterOp = it->second;
    return true;
  }

  void Insert(const RASTERID &rasterID, IRasterOp *pRasterOp) {
    uint32_t cbTotalSize = cbTotalSize_;
    if (cbTotalSize > MAX_CACHE_SIZE) {
      uint32_t compactSize = cbTotalSize / COMPACT_RATIO;
      if (cbTotalSize > compactSize) {
        for (auto iter = map_.rbegin(); cbTotalSize > compactSize;) {
          auto iterCur = iter--;
          cbTotalSize -= iterCur->second->GetCbSize();
          __safeRelease(iterCur->second);
          map_.erase(iterCur->first);
        }
        cbTotalSize_ = cbTotalSize;
      }
    }

    uint32_t cbSize = pRasterOp->GetCbSize();
    cbTotalSize_ += cbSize;
    map_.insert(std::make_pair(rasterID, pRasterOp));
  }

  static GLenum Create(CRasterCache **ppRasterCache) {
    __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

    assert(ppRasterCache);

    // Create a new raster cache
    CRasterCache *pRasterCache = new CRasterCache();
    if (nullptr == pRasterCache) {
      __glLogError(_T("CRasterCache allocation failed, out of memory.\r\n"));
      return GL_OUT_OF_MEMORY;
    }

    pRasterCache->AddRef();
    *ppRasterCache = pRasterCache;

    return GL_NO_ERROR;
  }

  void TrackSlowRasterID(const RASTERID &rasterID) {
    for (auto &rid : slowRasterIDs_) {
      if (rid == rasterID) {
        return;
      }
    }
    slowRasterIDs_.push_back(rasterID);
  }

private:
#ifdef COCOGL_RASTER_PROFILE

  static bool ProfileCompare(const Cache::List::Type &lhs,
                             const Cache::List::Type &rhs) {
    return (lhs.Second->GetProfile() < rhs.Second->GetProfile());
  }

#endif

  enum {
    MAX_CACHE_SIZE = 0x20000,
    COMPACT_RATIO = 4,
  };

  CRasterCache() { cbTotalSize_ = 0; }

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

  RasterData() : pRasterOp(nullptr) {}
};