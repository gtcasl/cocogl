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

#include "object.hpp"
#include <mutex>

class HandleTable : public Object {
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
    uint8_t getType() const { return list_->Type; }

    void *getObject() const { return list_->pObject; }

    void *getOwner() const { return list_->pOwner; }

    uint32_t getHandle() const {
      return (list_->Serial << HANDLE_SERIAL_SHIFT) |
             ((list_ - handles_->entries_) & HANDLE_INDEX_MASK);
    }

    bool isEnd() const { return (nullptr == list_); }

    void moveNext() {
      if (list_) {
        do {
          list_ = list_->pNext;
        } while (list_ && owner_ && (list_->pOwner != owner_));
      }
    }

    void *removeNext() {
      if (list_) {
        auto pObject = list_->pObject;
        list_ = handles_->deleteEntry(list_);
        while (list_ && owner_ && (list_->pOwner != owner_)) {
          list_ = list_->pNext;
        }
        return pObject;
      }
      return nullptr;
    }

  private:
    Enumerator(HandleTable *pHandles, const void *pOwner);

    HandleTable *handles_;
    Entry *list_;
    const void *owner_;

    friend class HandleTable;
  };

  HandleTable()
      : size_(0), entries_(nullptr), activelist_(nullptr), count_(0) {}

  void *getObject(uint32_t handle, const void *pOwner = nullptr);

  uint8_t getType(uint32_t handle, const void *pOwner = nullptr);

  HRESULT insert(uint32_t *phandle, void *pObject, uint8_t type,
                 void *pOwner = nullptr);

  void *deleteHandle(uint32_t handle, const void *pOwner = nullptr);

  void optimize();

  auto getEnumerator(const void *pOwner = nullptr) {
    return Enumerator(this, pOwner);
  }

  uint32_t getNumHandles() const { return count_; }

  uint32_t findHandle(const void *pObject, const void *pOwner = nullptr) {
    auto enumerator = this->getEnumerator(pOwner);
    while (!enumerator.isEnd()) {
      if (pObject == enumerator.getObject()) {
        return enumerator.getHandle();
      }
      enumerator.moveNext();
    }
    return 0;
  }

private:
  ~HandleTable() {
    if (entries_) {
      free(entries_);
    }
  }

  static uint32_t getHandleIndex(uint32_t handle) {
    return (handle & HANDLE_INDEX_MASK);
  }

  static uint32_t getHandleSerial(uint32_t handle) {
    return (handle & HANDLE_SERIAL_MASK) >> HANDLE_SERIAL_SHIFT;
  }

  Entry *getEntry(uint32_t handle) const {
    // Get the handle entry from the table
    auto index = this->getHandleIndex(handle);
    if (index < size_) {
      // Validate the handle serial
      auto pEntry = entries_ + index;
      if (pEntry->Serial == this->getHandleSerial(handle)) {
        return pEntry;
      }
    }
    return nullptr;
  }

  Entry *deleteEntry(Entry *pEntry);

  std::mutex cs_;
  uint32_t size_;
  Entry *entries_;
  Entry *activelist_;
  uint32_t count_;
};
