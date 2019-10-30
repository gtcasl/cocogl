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

CHandleTable::CHandleTable() {
  m_dwSize = 0;
  m_pEntries = nullptr;
  m_pActiveList = nullptr;
  m_nHandleCount = 0;
}

CHandleTable::~CHandleTable() {
  // Free allocated memory
  if (m_pEntries) {
    free(m_pEntries);
  }
}

HRESULT
CHandleTable::Create(CHandleTable **ppCHandleTable) {
  ASSERT(ppCHandleTable);

  // Allocate a new handle table
  CHandleTable *pHandleTable = new CHandleTable();
  if (nullptr == pHandleTable) {
    return E_OUTOFMEMORY;
  }

  pHandleTable->AddRef();

  *ppCHandleTable = pHandleTable;

  return S_OK;
}

void *CHandleTable::GetObject(uint32_t dwHandle, const void *pOwner) {
  std::lock_guard<std::mutex> lock(m_CS);

  // Retrieve the corresponding table entry
  Entry *pEntry = this->GetEntry(dwHandle);
  if (pEntry && ((nullptr == pOwner) || (pEntry->pOwner == pOwner))) {
    return pEntry->pObject;
  }

  return nullptr;
}

uint8_t CHandleTable::GetType(uint32_t dwHandle, const void *pOwner) {
  std::lock_guard<std::mutex> lock(m_CS);

  // Retrieve the corresponding table entry
  Entry *pEntry = this->GetEntry(dwHandle);
  if (pEntry && ((nullptr == pOwner) || (pEntry->pOwner == pOwner))) {
    return pEntry->Type;
  }

  return 0;
}

HRESULT
CHandleTable::Insert(uint32_t *pdwHandle, void *pObject, uint8_t type,
                     void *pOwner) {
  std::lock_guard<std::mutex> lock(m_CS);

  if ((nullptr == pObject) || (0 == type) || (nullptr == pdwHandle)) {
    return E_FAIL;
  }

  unsigned index;

  if (m_nHandleCount < m_dwSize) {
    // Find next free handle entry int he table
    for (index = 0; index < m_dwSize; ++index) {
      if (0 == m_pEntries[index].Type) {
        break;
      }
    }
  } else {
    index = m_dwSize;

    // Calculate the table new size
    const uint32_t dwNewSize = m_dwSize + GROW_INCREMENT;
    if (dwNewSize > HANDLE_INDEX_MASK)
      return E_FAIL;

    // Reallocate the table's buffer
    Entry *pEntries = reinterpret_cast<Entry *>(
        realloc(m_pEntries, dwNewSize * sizeof(Entry)));
    if (nullptr == pEntries) {
      return E_OUTOFMEMORY;
    }

    if (m_pActiveList) {
      // Calculate the reallocation offset
      const int offset = pEntries - m_pEntries;
      if (offset) {
        // Update active pointers to refect the offset
        m_pActiveList += offset;

        Entry *pCurEntry = m_pActiveList;
        while (pCurEntry) {
          if (pCurEntry->pNext) {
            pCurEntry->pNext += offset;
          }

          pCurEntry = pCurEntry->pNext;
        }
      }
    }

    // Reset newly allocated entries
    for (unsigned i = m_dwSize; i < dwNewSize; ++i) {
      Entry &entry = pEntries[i];
      entry.pOwner = nullptr;
      entry.Type = 0;
      entry.Reuse = 0;
      entry.pObject = nullptr;
      entry.pNext = nullptr;
    }

    m_pEntries = pEntries;
    m_dwSize = dwNewSize;
  }

  // Update the new handle entry
  Entry *pEntryNew = m_pEntries + index;
  pEntryNew->pOwner = pOwner;
  pEntryNew->Type = static_cast<uint8_t>(type);
  pEntryNew->pObject = pObject;
  ++pEntryNew->Reuse;
  pEntryNew->pNext = nullptr;

  // Add the entry to the active list ( sorted by address )
  Entry *pPrevEntry = nullptr;
  Entry *pCurEntry = m_pActiveList;
  while (pCurEntry && (pCurEntry < pEntryNew)) {
    pPrevEntry = pCurEntry;
    pCurEntry = pCurEntry->pNext;
  }

  pEntryNew->pNext = pCurEntry;
  if (pPrevEntry) {
    pPrevEntry->pNext = pEntryNew;
  } else {
    m_pActiveList = pEntryNew;
  }

  // Update the handle count
  ++m_nHandleCount;

  // Return the new handle
  *pdwHandle = (pEntryNew->Serial << HANDLE_SERIAL_SHIFT) | index;

  return S_OK;
}

CHandleTable::Entry *CHandleTable::DeleteEntry(Entry *pEntry) {
  ASSERT(pEntry);

  // Save the next entry
  Entry *const pNextEntry = pEntry->pNext;

  // Remove the entry from the active list
  Entry *pPrevEntry = nullptr;
  Entry *pCurEntry = m_pActiveList;
  while (pCurEntry) {
    if (pCurEntry == pEntry) {
      if (pPrevEntry) {
        pPrevEntry->pNext = pNextEntry;
      } else {
        m_pActiveList = pNextEntry;
      }

      break;
    }

    pPrevEntry = pCurEntry;
    pCurEntry = pCurEntry->pNext;
  }

  // Reset the entry
  pEntry->pOwner = nullptr;
  pEntry->Type = 0;
  pEntry->pObject = nullptr;
  pEntry->pNext = nullptr;

  // Update the handle count
  --m_nHandleCount;

  // Return the next entry
  return pNextEntry;
}

void *CHandleTable::Delete(uint32_t dwHandle, const void *pOwner) {
  std::lock_guard<std::mutex> lock(m_CS);

  // Retrieve the corresponding table entry
  Entry *pEntry = this->GetEntry(dwHandle);
  if (pEntry && ((nullptr == pOwner) || (pEntry->pOwner == pOwner))) {
    // Get the object pointer
    void *const pObject = pEntry->pObject;

    // Remove the entry from the table
    this->DeleteEntry(pEntry);

    // Return the object
    return pObject;
  }

  return nullptr;
}

void CHandleTable::Optimize() {
  std::lock_guard<std::mutex> lock(m_CS);

  // Check if we need to optimize the table
  if ((m_dwSize - m_nHandleCount) < GROW_INCREMENT) {
    return;
  }

  unsigned freeSlots = 0;

  // Calculate the size of consecutive free slots starting at the end
  for (uint32_t dw = m_dwSize; dw--;) {
    if (m_pEntries[dw].Type != 0) {
      break;
    }

    ++freeSlots;
  }

  if (freeSlots >= GROW_INCREMENT) {
    m_dwSize -= GROW_INCREMENT * (freeSlots / GROW_INCREMENT);
    if (m_dwSize) {
      // Shrink the table's buffer
      m_pEntries = reinterpret_cast<Entry *>(
          realloc(m_pEntries, m_dwSize * sizeof(Entry)));
      ASSERT(m_pEntries);
    } else {
      // Destroy table's buffer
      free(m_pEntries);
      m_pEntries = nullptr;
    }
  }
}
