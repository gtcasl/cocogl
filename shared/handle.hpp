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

#include <mutex>
#include "object.hpp"

class CHandleTable : public CObject {
private:
  enum {
    GROW_INCREMENT = 64,
    HANDLE_INDEX_MASK = 0x0000ffff,
    HANDLE_SERIAL_MASK = 0xffff0000,
    HANDLE_SERIAL_SHIFT = 16,
  };

  struct Entry {
    DISABLE_WARNING_PUSH
    DISABLE_WARNING_ANONYMOUS_STRUCT
    union {
      struct {
        uint8_t Type;  // Object type
        uint8_t Reuse; // Entry reuse count
      };
      uint16_t Serial; // Serial number
    };
    DISABLE_WARNING_POP
    void *pObject; // Object pointer
    void *pOwner;  // Object owner
    Entry *pNext;  // Next active entry
  };

public:
  class Enumerator {
  public:
    uint8_t GetType() const { return list_->Type; }

    void *GetObject() const { return list_->pObject; }

    void *GetOwner() const { return list_->pOwner; }

    uint32_t GetHandle() const {
      return (list_->Serial << HANDLE_SERIAL_SHIFT) |
             ((list_ - handles_->entries_) & HANDLE_INDEX_MASK);
    }

    bool IsEnd() const { return (nullptr == list_); }

    void MoveNext() {
      if (list_) {
        do {
          list_ = list_->pNext;
        } while (list_ && owner_ && (list_->pOwner != owner_));
      }
    }

    void *RemoveNext() {
      if (list_) {
        auto pObject = list_->pObject;
        list_ = handles_->DeleteEntry(list_);
        while (list_ && owner_ && (list_->pOwner != owner_)) {
          list_ = list_->pNext;
        }
        return pObject;
      }
      return nullptr;
    }

  private:
    Enumerator(CHandleTable *pHandles, const void *pOwner) {
      assert(pHandles);
      std::lock_guard<std::mutex> lock(pHandles->cs_);

      handles_ = pHandles;
      list_ = pHandles->activelist_;
      owner_ = pOwner;

      // Find the next owned entry
      while (list_ && pOwner && (list_->pOwner != pOwner)) {
        list_ = list_->pNext;
      }
    }

    CHandleTable *handles_;
    Entry *list_;
    const void *owner_;

    friend class CHandleTable;
  };

  static HRESULT Create(CHandleTable **ppCHandleTable) {
    assert(ppCHandleTable);

    // Allocate a new handle table
    auto pHandleTable = new CHandleTable();
    if (nullptr == pHandleTable) {
      return E_OUTOFMEMORY;
    }

    pHandleTable->AddRef();

    *ppCHandleTable = pHandleTable;

    return S_OK;
  }

  void *GetObject(uint32_t dwHandle, const void *pOwner = nullptr) {
    std::lock_guard<std::mutex> lock(cs_);

    // Retrieve the corresponding table entry
    auto pEntry = this->GetEntry(dwHandle);
    if (pEntry && ((nullptr == pOwner) || (pEntry->pOwner == pOwner))) {
      return pEntry->pObject;
    }

    return nullptr;
  }

  uint8_t GetType(uint32_t dwHandle, const void *pOwner = nullptr) {
    std::lock_guard<std::mutex> lock(cs_);

    // Retrieve the corresponding table entry
    auto pEntry = this->GetEntry(dwHandle);
    if (pEntry && ((nullptr == pOwner) || (pEntry->pOwner == pOwner))) {
      return pEntry->Type;
    }

    return 0;
  }

  HRESULT Insert(uint32_t *pdwHandle, void *pObject, uint8_t type,
                 void *pOwner = nullptr) {
    std::lock_guard<std::mutex> lock(cs_);

    if ((nullptr == pObject) || (0 == type) || (nullptr == pdwHandle)) {
      return E_FAIL;
    }

    uint32_t index;

    if (count_ < size_) {
      // Find next free handle entry int he table
      for (index = 0; index < size_; ++index) {
        if (0 == entries_[index].Type) {
          break;
        }
      }
    } else {
      index = size_;

      // Calculate the table new size
      uint32_t dwNewSize = size_ + GROW_INCREMENT;
      if (dwNewSize > HANDLE_INDEX_MASK)
        return E_FAIL;

      // Reallocate the table's buffer
      auto pEntries = reinterpret_cast<Entry *>(
          realloc(entries_, dwNewSize * sizeof(Entry)));
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
      for (uint32_t i = size_; i < dwNewSize; ++i) {
        Entry &entry = pEntries[i];
        entry.pOwner = nullptr;
        entry.Type = 0;
        entry.Reuse = 0;
        entry.pObject = nullptr;
        entry.pNext = nullptr;
      }

      entries_ = pEntries;
      size_ = dwNewSize;
    }

    // Update the new handle entry
    auto pEntryNew = entries_ + index;
    pEntryNew->pOwner = pOwner;
    pEntryNew->Type = static_cast<uint8_t>(type);
    pEntryNew->pObject = pObject;
    ++pEntryNew->Reuse;
    pEntryNew->pNext = nullptr;

    // Add the entry to the active list ( sorted by address )
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
    ++count_;

    // Return the new handle
    *pdwHandle = (pEntryNew->Serial << HANDLE_SERIAL_SHIFT) | index;

    return S_OK;
  }

  void *Delete(uint32_t dwHandle, const void *pOwner = nullptr) {
    std::lock_guard<std::mutex> lock(cs_);

    // Retrieve the corresponding table entry
    auto pEntry = this->GetEntry(dwHandle);
    if (pEntry && ((nullptr == pOwner) || (pEntry->pOwner == pOwner))) {
      // Get the object pointer
      auto pObject = pEntry->pObject;

      // Remove the entry from the table
      this->DeleteEntry(pEntry);

      // Return the object
      return pObject;
    }

    return nullptr;
  }

  void Optimize() {
    std::lock_guard<std::mutex> lock(cs_);

    // Check if we need to optimize the table
    if ((size_ - count_) < GROW_INCREMENT) {
      return;
    }

    uint32_t freeSlots = 0;

    // Calculate the size of consecutive free slots starting at the end
    for (uint32_t i = size_; i--;) {
      if (entries_[i].Type != 0) {
        break;
      }

      ++freeSlots;
    }

    if (freeSlots >= GROW_INCREMENT) {
      size_ -= GROW_INCREMENT * (freeSlots / GROW_INCREMENT);
      if (size_) {
        // Shrink the table's buffer
        entries_ =
            reinterpret_cast<Entry *>(realloc(entries_, size_ * sizeof(Entry)));
        assert(entries_);
      } else {
        // Destroy table's buffer
        free(entries_);
        entries_ = nullptr;
      }
    }
  }

  auto GetEnumerator(const void *pOwner = nullptr) {
    return Enumerator(this, pOwner);
  }

  uint32_t GetNumHandles() const { return count_; }

  uint32_t FindHandle(const void *pObject, const void *pOwner = nullptr) {
    auto enumerator = this->GetEnumerator(pOwner);
    while (!enumerator.IsEnd()) {
      if (pObject == enumerator.GetObject()) {
        return enumerator.GetHandle();
      }
      enumerator.MoveNext();
    }
    return 0;
  }

private:
  CHandleTable()
      : size_(0), entries_(nullptr), activelist_(nullptr), count_(0) {}

  ~CHandleTable() {
    if (entries_) {
      free(entries_);
    }
  }

  static uint32_t GetHandleIndex(uint32_t dwHandle) {
    return (dwHandle & HANDLE_INDEX_MASK);
  }

  static uint32_t GetHandleSerial(uint32_t dwHandle) {
    return (dwHandle & HANDLE_SERIAL_MASK) >> HANDLE_SERIAL_SHIFT;
  }

  Entry *GetEntry(uint32_t dwHandle) const {
    // Get the handle entry from the table
    auto index = this->GetHandleIndex(dwHandle);
    if (index < size_) {
      // Validate the handle serial
      auto pEntry = entries_ + index;
      if (pEntry->Serial == this->GetHandleSerial(dwHandle)) {
        return pEntry;
      }
    }
    return nullptr;
  }

  Entry *DeleteEntry(Entry *pEntry) {
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
    pEntry->pOwner = nullptr;
    pEntry->Type = 0;
    pEntry->pObject = nullptr;
    pEntry->pNext = nullptr;

    // Update the handle count
    --count_;

    // Return the next entry
    return pNextEntry;
  }

  std::mutex cs_;
  uint32_t size_;
  Entry *entries_;
  Entry *activelist_;
  uint32_t count_;
};
