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
#include "stdafx.h"
#include <sys/mman.h>

CAllocator::CAllocator(bool execute_access) {
  m_pPages = nullptr;
  m_execute_access = execute_access;
}

CAllocator::~CAllocator() {
  // All pages should be released by now
  ASSERT(nullptr == m_pPages);

  // Free allocated pages
  Page *pCurPage = m_pPages;
  while (pCurPage) {
    Page *const pNextPage = pCurPage->pNext;
    this->DeletePage(pCurPage);
    pCurPage = pNextPage;
  }
}

HRESULT
CAllocator::Create(CAllocator **ppAllocator, bool execute_access) {
  if (nullptr == ppAllocator) {
    __debugMsg(1, _T("CAllocator::Create() failed, invalid arguments.\r\n"));
    return E_INVALIDARG;
  }

  CAllocator *pAllocator = new CAllocator(execute_access);
  if (nullptr == pAllocator) {
    __debugMsg(1, _T("New CAllocator() failed, out of memory.\r\n"));
    return E_OUTOFMEMORY;
  }

  pAllocator->AddRef();
  *ppAllocator = pAllocator;

  return S_OK;
}

void *CAllocator::Allocate(uint32_t dwBytes) {
  std::lock_guard<std::mutex> lock(m_CS);

  if (0 == dwBytes) {
    __debugMsg(1, _T("Empty size allocation.\r\n"));
    return nullptr;
  }

  // Pad allocation size to nearest uint32_t to ensure alignment
  dwBytes = __align(dwBytes, Block::ALIGN_SIZE);

  // Walk thru all pages to find a free block
  Block *pFreeBlock = nullptr;
  Page *pCurPage = m_pPages;
  while (pCurPage) {
    Block *pCurBlock = pCurPage->pFreeSList;
    if (pCurBlock) {
      // The free list is already sorted with biggest block on top,
      // just check if the last block has enough space.
      if (pCurBlock->Size >= dwBytes) {
        // Find the smallest matching block
        while (pCurBlock->pNextFreeS &&
               (pCurBlock->pNextFreeS->Size >= dwBytes)) {
          pCurBlock = pCurBlock->pNextFreeS;
        }

        // Return the free block
        pFreeBlock = pCurBlock;
        break;
      }
    }
    pCurPage = pCurPage->pNext;
  }

  if (nullptr == pFreeBlock) {
    // Allocate a new page for this request
    pCurPage = this->NewPage(dwBytes);
    if (nullptr == pCurPage) {
      __debugMsg(1, _T("CAllocator::NewPage() failed.\r\n"));
      return nullptr;
    }

    pFreeBlock = pCurPage->pFreeSList;
  }

  // Remove the block from the free list
  ASSERT(pFreeBlock->Size >= dwBytes);
  pCurPage->RemoveFreeMBlock(pFreeBlock);
  pCurPage->RemoveFreeSBlock(pFreeBlock);

  // If the free block we have found is larger than what we are looking for,
  // we may be able to split our free block in two.
  const unsigned cbExtraBytes = pFreeBlock->Size - dwBytes;
  const unsigned cbBlockSize = __align(sizeof(Block), Block::ALIGN_SIZE);
  const unsigned cbMinAllocSize = cbBlockSize + Block::MIN_SIZE;
  if (cbExtraBytes >= cbMinAllocSize) {
    // Reduce the free block size to the requested value
    pFreeBlock->Size = dwBytes;

    // Allocate a new block to contain the extra buffer
    uint8_t *const pExtraMem = pFreeBlock->pMem + dwBytes;
    Block *const pNewBlock = new (pExtraMem)
        Block(pExtraMem + cbBlockSize, cbExtraBytes - cbBlockSize);

    // Add the new block to the free list
    pCurPage->InsertFreeMBlock(pNewBlock);
    pCurPage->InsertFreeSBlock(pNewBlock);
  }

  // Insert the free block into the used list
  pCurPage->InsertUsedBlock(pFreeBlock);

  // Return the free block memory
  return pFreeBlock->pMem;
}

void CAllocator::Free(void *ptr) {
  if (nullptr == ptr) {
    return;
  }

  std::lock_guard<std::mutex> lock(m_CS);

  // Walk all pages to find the pointer
  Block *pUsedBlock = nullptr;
  Page *pCurPage = m_pPages;
  while (pCurPage) {
    if ((pCurPage->pBuffer < ptr) &&
        ((pCurPage->pBuffer + pCurPage->BufferSize) > ptr)) {
      Block *pCurBlock = pCurPage->pUsedList;
      while (pCurBlock) {
        if (pCurBlock->pMem == ptr) {
          pUsedBlock = pCurBlock;
          break;
        }
        pCurBlock = pCurBlock->pNextUsed;
      }

      if (pUsedBlock) {
        break;
      }
    }
    pCurPage = pCurPage->pNext;
  }

  if (nullptr == pUsedBlock) {
    ASSERT(false);
    return;
  }

  // Remove the block from the allocation list
  pCurPage->RemoveUsedBlock(pUsedBlock);

  // Insert the block into the free memory list.
  pCurPage->InsertFreeMBlock(pUsedBlock);

  // Calculate the aligned block size
  const unsigned cbBlockSize = __align(sizeof(Block), Block::ALIGN_SIZE);

  // Check if we can merge adjacent free blocks from the left.
  if (pUsedBlock->pPrevFreeM) {
    // Calculate the previous memory end address
    uint8_t *const pPrevMem = pUsedBlock->pPrevFreeM->pMem +
                              pUsedBlock->pPrevFreeM->Size + cbBlockSize;

    if (pUsedBlock->pMem == pPrevMem) {
      Block *const pMergedBlock = pUsedBlock->pPrevFreeM;

      // Detach left block from the free size list
      pCurPage->RemoveFreeSBlock(pMergedBlock);

      // Merge the blocks to the left
      pMergedBlock->Size += (pUsedBlock->Size + cbBlockSize);
      pMergedBlock->pNextFreeM = pUsedBlock->pNextFreeM;
      if (pMergedBlock->pNextFreeM) {
        pMergedBlock->pNextFreeM->pPrevFreeM = pMergedBlock;
      }
      pUsedBlock = pMergedBlock;
    }
  }

  // Check if we can merge adjacent free blocks from the right.
  if (pUsedBlock->pNextFreeM) {
    // Calculate the next memory start address
    uint8_t *const pNextMem = pUsedBlock->pMem + pUsedBlock->Size + cbBlockSize;

    if (pUsedBlock->pNextFreeM->pMem == pNextMem) {
      Block *const pNextBlock = pUsedBlock->pNextFreeM;

      // Detach right block from the free size list
      pCurPage->RemoveFreeSBlock(pNextBlock);

      // Merge the blocks to the right
      pUsedBlock->Size += (pNextBlock->Size + cbBlockSize);
      pUsedBlock->pNextFreeM = pNextBlock->pNextFreeM;
      if (pUsedBlock->pNextFreeM) {
        pUsedBlock->pNextFreeM->pPrevFreeM = pUsedBlock;
      }
    }
  }

  // Insert the block into the free size list.
  pCurPage->InsertFreeSBlock(pUsedBlock);

  // Check if we can free the page
  if (nullptr == pCurPage->pUsedList) {
    // Delete the current page
    this->DeletePage(pCurPage);
  }
}

CAllocator::Page *CAllocator::NewPage(uint32_t dwCbSize) {
  // Increase buffer size to include the page and first block size
  // also add padding to ensure page aligment
  const unsigned cbPlacementSize =
      __align(sizeof(Page) + sizeof(Block), Block::ALIGN_SIZE);
  dwCbSize = __align(dwCbSize + cbPlacementSize, Page::ALIGN_SIZE);

// Allocate virtual memory
#ifdef _WIN32
  auto protection = m_execute_access ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
  uint8_t *pBuffer = reinterpret_cast<uint8_t *>(
      ::VirtualAlloc(nullptr, dwCbSize, MEM_COMMIT | MEM_RESERVE, protection));
  if (nullptr == pBuffer) {
    __debugMsg(1, _T("VirtualAlloc() failed, error = %d.\r\n"),
               ::GetLastError());
    return nullptr;
  }
#else
  auto protection = m_execute_access ? (PROT_READ | PROT_WRITE | PROT_EXEC)
                                     : (PROT_READ | PROT_WRITE);
  uint8_t *pBuffer = reinterpret_cast<uint8_t *>(
      mmap(nullptr, dwCbSize, protection, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0));
  if (pBuffer == MAP_FAILED) {
    __debugMsg(1, _T("mmap() failed. error = %d\r\n"), errno);
    return nullptr;
  }
#endif

  // Allocate the page
  Page *const pNewPage = new (pBuffer) Page(pBuffer, dwCbSize);

  // Insert the new page into the list
  pNewPage->pNext = m_pPages;
  m_pPages = pNewPage;

  return pNewPage;
}

void CAllocator::DeletePage(Page *pPage) {
  ASSERT(pPage);

  // Remove the page from the list
  Page *pPrevPage = nullptr;
  Page *pCurPage = m_pPages;
  while (pCurPage) {
    if (pCurPage == pPage) {
      if (pPrevPage) {
        pPrevPage->pNext = pCurPage->pNext;
      } else {
        m_pPages = pCurPage->pNext;
      }
      break;
    }

    pPrevPage = pCurPage;
    pCurPage = pCurPage->pNext;
  }

  // The allocated list should be empty by now
  ASSERT(nullptr == pPage->pUsedList);

  // The free list should be one
  ASSERT(pPage->pFreeMList && (nullptr == pPage->pFreeMList->pNextFreeM));

// Release allocated virtual memory
#ifdef _WIN32
  if (!::VirtualFree(pPage->pBuffer, 0, MEM_RELEASE)) {
    __debugMsg(1, _T("VirtualFree() failed, error = %d.\r\n"),
               ::GetLastError());
  }
#else
  if (0 != munmap(pPage->pBuffer, pPage->BufferSize)) {
    __debugMsg(1, _T("mumap() failed, error = %d.\r\n"), errno);
  }
#endif
}

//////////////////////////////////////////////////////////////////////////////

void CAllocator::Page::RemoveUsedBlock(Block *pBlock) {
  ASSERT(pBlock);

  if (pBlock->pPrevUsed) {
    pBlock->pPrevUsed->pNextUsed = pBlock->pNextUsed;
  } else {
    this->pUsedList = pBlock->pNextUsed;
  }

  if (pBlock->pNextUsed) {
    pBlock->pNextUsed->pPrevUsed = pBlock->pPrevUsed;
  }

  pBlock->pNextUsed = nullptr;
  pBlock->pPrevUsed = nullptr;
}

void CAllocator::Page::InsertUsedBlock(Block *pBlock) {
  ASSERT(pBlock);

  pBlock->pNextUsed = this->pUsedList;
  if (this->pUsedList) {
    this->pUsedList->pPrevUsed = pBlock;
  }
  this->pUsedList = pBlock;
}

void CAllocator::Page::RemoveFreeMBlock(Block *pBlock) {
  ASSERT(pBlock);

  if (pBlock->pPrevFreeM) {
    pBlock->pPrevFreeM->pNextFreeM = pBlock->pNextFreeM;
  } else {
    this->pFreeMList = pBlock->pNextFreeM;
  }

  if (pBlock->pNextFreeM) {
    pBlock->pNextFreeM->pPrevFreeM = pBlock->pPrevFreeM;
  }

  pBlock->pNextFreeM = nullptr;
  pBlock->pPrevFreeM = nullptr;
}

void CAllocator::Page::InsertFreeMBlock(Block *pBlock) {
  ASSERT(pBlock);

  Block *pCurBlock = this->pFreeMList;
  Block *pPrevBlock = nullptr;

  // Sort the block list by increasing memory addresses
  // to simplify blocks merging when freeing memory.
  while (pCurBlock && (pCurBlock->pMem < pBlock->pMem)) {
    pPrevBlock = pCurBlock;
    pCurBlock = pCurBlock->pNextFreeM;
  }

  pBlock->pNextFreeM = pCurBlock;
  pBlock->pPrevFreeM = pPrevBlock;

  if (pPrevBlock) {
    pPrevBlock->pNextFreeM = pBlock;
  } else {
    this->pFreeMList = pBlock;
  }

  if (pCurBlock) {
    pCurBlock->pPrevFreeM = pBlock;
  }
}

void CAllocator::Page::RemoveFreeSBlock(Block *pBlock) {
  ASSERT(pBlock);

  if (pBlock->pPrevFreeS) {
    pBlock->pPrevFreeS->pNextFreeS = pBlock->pNextFreeS;
  } else {
    this->pFreeSList = pBlock->pNextFreeS;
  }

  if (pBlock->pNextFreeS) {
    pBlock->pNextFreeS->pPrevFreeS = pBlock->pPrevFreeS;
  }

  pBlock->pNextFreeS = nullptr;
  pBlock->pPrevFreeS = nullptr;
}

void CAllocator::Page::InsertFreeSBlock(Block *pBlock) {
  ASSERT(pBlock);

  Block *pCurBlock = this->pFreeSList;
  Block *pPrevBlock = nullptr;

  // Sort the block list by descreasing sizes
  // to simplify the look up when allocating new memory.
  while (pCurBlock && (pCurBlock->Size > pBlock->Size)) {
    pPrevBlock = pCurBlock;
    pCurBlock = pCurBlock->pNextFreeS;
  }

  pBlock->pNextFreeS = pCurBlock;
  pBlock->pPrevFreeS = pPrevBlock;

  if (pPrevBlock) {
    pPrevBlock->pNextFreeS = pBlock;
  } else {
    this->pFreeSList = pBlock;
  }

  if (pCurBlock) {
    pCurBlock->pPrevFreeS = pBlock;
  }
}
