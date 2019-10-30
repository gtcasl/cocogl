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
#pragma once

#include <mutex>

class CAllocator : public CObject {
public:
  static HRESULT Create(CAllocator **ppAllocator, bool execute_access = false);

  virtual void *Allocate(uint32_t dwBytes);
  virtual void Free(void *ptr);

private:
  struct Block {
    enum {
      ALIGN_SIZE = 4,  // 4 Bytes
      MIN_SIZE = 0x40, // 64 Bytes
    };

    Block *pNextFreeS;
    Block *pPrevFreeS;

    Block *pNextFreeM;
    Block *pPrevFreeM;

    Block *pNextUsed;
    Block *pPrevUsed;

    uint8_t *pMem;
    unsigned Size;

    Block(uint8_t *pMem, unsigned size)
        : pNextFreeS(NULL), pPrevFreeS(NULL), pNextFreeM(NULL),
          pPrevFreeM(NULL), pNextUsed(NULL), pPrevUsed(NULL), pMem(pMem),
          Size(size) {}
  };

  struct Page {
    enum {
      ALIGN_SIZE = (1 << 16), // 64 KB
    };

    Page *pNext;
    Block *pUsedList;
    Block *pFreeSList;
    Block *pFreeMList;
    uint8_t *pBuffer;
    unsigned BufferSize;

    Page(uint8_t *pBuffer_, unsigned bufferSize)
        : pNext(NULL), pUsedList(NULL), pBuffer(pBuffer_),
          BufferSize(bufferSize) {
      //
      // Allocate the first free block
      //

      const unsigned cbPlacementSize =
          __align(sizeof(Page) + sizeof(Block), Block::ALIGN_SIZE);

      this->pFreeSList = this->pFreeMList = new (pBuffer_ + sizeof(Page))
          Block(pBuffer_ + cbPlacementSize, bufferSize - cbPlacementSize);
    }

    void RemoveUsedBlock(Block *pBlock);
    void InsertUsedBlock(Block *pBlock);

    void RemoveFreeMBlock(Block *pBlock);
    void InsertFreeMBlock(Block *pBlock);

    void RemoveFreeSBlock(Block *pBlock);
    void InsertFreeSBlock(Block *pBlock);
  };

  CAllocator(bool execute_access);
  virtual ~CAllocator();

  Page *NewPage(uint32_t dwCbSize);
  void DeletePage(Page *pPage);

  std::mutex m_CS;
  Page *m_pPages;
  bool m_execute_access;
};

//////////////////////////////////////////////////////////////////////////////

class StdAllocator {
public:
  StdAllocator() {}
  ~StdAllocator() {}
};