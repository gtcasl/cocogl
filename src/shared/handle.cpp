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
#include "handle.hpp"

HandleTable::HandleTable()
  : capacity_(0)
  , entries_(nullptr)
  , activelist_(nullptr)
  , size_(0)
{}

HandleTable::~HandleTable() {
  this->clear();
}

void *HandleTable::getObject(uint32_t handle) const {
  // Retrieve the corresponding table entry
  auto pEntry = this->getEntry(handle);
  if (pEntry) {
    return pEntry->pObject;
  }
  return nullptr;
}

uint8_t HandleTable::getType(uint32_t handle) const {
  // Retrieve the corresponding table entry
  auto pEntry = this->getEntry(handle);
  if (pEntry) {
    return pEntry->Type;
  }

  return 0;
}

HRESULT HandleTable::insert(uint32_t *phandle, void *pObject, uint8_t type) {
  if ((nullptr == pObject) || (0 == type) || (nullptr == phandle))
    return E_FAIL;

  uint32_t index;

  if (size_ < capacity_) {
    // Find next free handle entry int he table
    for (index = 0; index < capacity_; ++index) {
      if (0 == entries_[index].Type) {
        break;
      }
    }
  } else {
    index = capacity_;

    // Calculate the table new size
    uint32_t newSize = capacity_ + GROW_INCREMENT;
    if (newSize > HANDLE_INDEX_MASK)
      return E_FAIL;

    // Reallocate the table's buffer
    auto pEntries =
        reinterpret_cast<Entry *>(realloc(entries_, newSize * sizeof(Entry)));
    if (nullptr == pEntries) {
      return E_OUTOFMEMORY;
    }

    if (activelist_) {
      // Calculate the reallocation offset
      int offset = pEntries - entries_;
      if (offset) {
        // Update active pointers to refect the offset
        activelist_ += offset;

        auto pCurEntry = activelist_;
        while (pCurEntry) {
          if (pCurEntry->pNext) {
            pCurEntry->pNext += offset;
          }

          pCurEntry = pCurEntry->pNext;
        }
      }
    }

    // Reset newly allocated entries
    for (uint32_t i = capacity_; i < newSize; ++i) {
      Entry &entry = pEntries[i];
      entry.Type = 0;
      entry.Reuse = 0;
      entry.pObject = nullptr;
      entry.pNext = nullptr;
    }

    entries_ = pEntries;
    capacity_ = newSize;
  }

  // Update the new handle entry
  auto pEntryNew = entries_ + index;
  pEntryNew->Type = static_cast<uint8_t>(type);
  pEntryNew->pObject = pObject;
  ++pEntryNew->Reuse;
  pEntryNew->pNext = nullptr;

  // Add the entry to the active list
  Entry *pPrevEntry = nullptr;
  auto pCurEntry = activelist_;
  while (pCurEntry && (pCurEntry < pEntryNew)) {
    pPrevEntry = pCurEntry;
    pCurEntry = pCurEntry->pNext;
  }

  pEntryNew->pNext = pCurEntry;
  if (pPrevEntry) {
    pPrevEntry->pNext = pEntryNew;
  } else {
    activelist_ = pEntryNew;
  }

  // Update the handle count
  ++size_;

  // Return the new handle
  *phandle = (pEntryNew->Serial << HANDLE_SERIAL_SHIFT) | index;

  return S_OK;
}

void *HandleTable::deleteHandle(uint32_t handle) {
  // Retrieve the corresponding table entry
  auto pEntry = this->getEntry(handle);
  if (pEntry) {
    // Get the object pointer
    auto pObject = pEntry->pObject;

    // Remove the entry from the table
    this->deleteEntry(pEntry);

    // Return the object
    return pObject;
  }
  return nullptr;
}

void HandleTable::optimize() {
  // Check if we need to optimize the table
  if ((capacity_ - size_) < GROW_INCREMENT) {
    return;
  }

  uint32_t freeSlots = 0;

  // Calculate the size of consecutive free slots starting at the end
  for (uint32_t i = capacity_; i--;) {
    if (entries_[i].Type != 0) {
      break;
    }

    ++freeSlots;
  }

  if (freeSlots >= GROW_INCREMENT) {
    capacity_ -= GROW_INCREMENT * (freeSlots / GROW_INCREMENT);
    if (capacity_) {
      // Shrink the table's buffer
      entries_ =
          reinterpret_cast<Entry *>(realloc(entries_, capacity_ * sizeof(Entry)));
      assert(entries_);
    } else {
      // Destroy table's buffer
      free(entries_);
      entries_ = nullptr;
    }
  }
}

HandleTable::Entry *HandleTable::deleteEntry(HandleTable::Entry *pEntry) {
  assert(pEntry);
  // Save the next entry
  auto pNextEntry = pEntry->pNext;

  // Remove the entry from the active list
  Entry *pPrevEntry = nullptr;
  auto pCurEntry = activelist_;
  while (pCurEntry) {
    if (pCurEntry == pEntry) {
      if (pPrevEntry) {
        pPrevEntry->pNext = pNextEntry;
      } else {
        activelist_ = pNextEntry;
      }
      break;
    }
    pPrevEntry = pCurEntry;
    pCurEntry = pCurEntry->pNext;
  }

  // Reset the entry
  pEntry->Type = 0;
  pEntry->pObject = nullptr;
  pEntry->pNext = nullptr;

  // Update the handle count
  --size_;

  // Return the next entry
  return pNextEntry;
}

uint32_t HandleTable::findHandle(const void *pObject) {
  auto enumerator = this->getEnumerator();
  while (!enumerator.isEnd()) {
    if (pObject == enumerator.getObject()) {
      return this->getHandle(enumerator);
    }
    enumerator.moveNext();
  }
  return 0;
}

HandleTable::Entry *HandleTable::getEntry(uint32_t handle) const {
  // Get the handle entry from the table
  auto index = this->getHandleIndex(handle);
  if (index < capacity_) {
    // Validate the handle serial
    auto pEntry = entries_ + index;
    if (pEntry->Serial == this->getHandleSerial(handle)) {
      return pEntry;
    }
  }
  return nullptr;
}

void HandleTable::clear() {
  if (entries_) {
    free(entries_);
    entries_ = nullptr;
  }
  activelist_ = nullptr;
  capacity_ = 0;    
  size_ = 0;
}