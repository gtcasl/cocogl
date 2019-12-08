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

class HandleTable {
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
    Entry *pNext;  // Next active entry
  };

public:

  class Enumerator {
  public:
    uint8_t getType() const {
      return list_->Type;
    }

    void *getObject() const {
      return list_->pObject;
    }

    bool isEnd() const {
      return (nullptr == list_);
    }

    void moveNext() {
      list_ = list_->pNext;
    }

  private:
    Enumerator(Entry * list) : list_(list) {}

    Entry *list_;

    friend class HandleTable;
  };  

  HandleTable();

  ~HandleTable();

  void *getObject(uint32_t handle) const;

  uint8_t getType(uint32_t handle) const;

  HRESULT insert(uint32_t *phandle, void *pObject, uint8_t typer);

  void *deleteHandle(uint32_t handle);

  void optimize();

  auto getEnumerator() {
    return Enumerator(activelist_);
  }

  uint32_t getHandle(const Enumerator& e) const {
    return (e.list_->Serial << HANDLE_SERIAL_SHIFT) |
            ((e.list_ - entries_) & HANDLE_INDEX_MASK);
  }

  uint32_t getSize() const {
    return size_;
  }

  uint32_t findHandle(const void *pObject);

  void clear();

private:

  static uint32_t getHandleIndex(uint32_t handle) {
    return (handle & HANDLE_INDEX_MASK);
  }

  static uint32_t getHandleSerial(uint32_t handle) {
    return (handle & HANDLE_SERIAL_MASK) >> HANDLE_SERIAL_SHIFT;
  }

  Entry *getEntry(uint32_t handle) const;

  Entry *deleteEntry(Entry *pEntry);

  uint32_t capacity_;
  Entry *entries_;
  Entry *activelist_;
  uint32_t size_;
};
