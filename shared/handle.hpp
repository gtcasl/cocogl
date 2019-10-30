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
    ~Enumerator() {
      if (m_pHandles) {
        m_pHandles->Unlock();
        m_pHandles->Release();
      }
    }

    uint8_t GetType() const { return m_pList->Type; }

    void *GetObject() const { return m_pList->pObject; }

    void *GetOwner() const { return m_pList->pOwner; }

    uint32_t GetHandle() const {
      return (m_pList->Serial << HANDLE_SERIAL_SHIFT) |
             ((m_pList - m_pHandles->m_pEntries) & HANDLE_INDEX_MASK);
    }

    bool IsEnd() {
      if (m_pList) {
        return false;
      }

      if (m_pHandles) {
        m_pHandles->Unlock();
        m_pHandles->Release();
        m_pHandles = nullptr;
      }

      return true;
    }

    void MoveNext() {
      if (m_pList) {
        do {
          m_pList = m_pList->pNext;
        } while (m_pList && m_pOwner && (m_pList->pOwner != m_pOwner));
      }
    }

    void *RemoveNext() {
      if (m_pList) {
        void *pObject = m_pList->pObject;
        m_pList = m_pHandles->DeleteEntry(m_pList);
        while (m_pList && m_pOwner && (m_pList->pOwner != m_pOwner)) {
          m_pList = m_pList->pNext;
        }

        return pObject;
      }

      return nullptr;
    }

    void operator=(const Enumerator &rhs) {
      this->Copy(rhs.m_pHandles, rhs.m_pOwner);
    }

  private:
    Enumerator(CHandleTable *pHandles, const void *pOwner) {
      this->Copy(pHandles, pOwner);
    }

    void Copy(CHandleTable *pHandles, const void *pOwner) {
      ASSERT(pHandles);

      pHandles->AddRef();
      m_pHandles = pHandles;
      pHandles->Lock();

      m_pList = pHandles->m_pActiveList;
      m_pOwner = pOwner;

      // Find the next owned entry
      while (m_pList && pOwner && (m_pList->pOwner != pOwner)) {
        m_pList = m_pList->pNext;
      }
    }

    CHandleTable *m_pHandles;
    Entry *m_pList;
    const void *m_pOwner;

    friend class CHandleTable;
  };

  static HRESULT Create(CHandleTable **ppCHandleTable);

  void *GetObject(uint32_t dwHandle, const void *pOwner = nullptr);

  uint8_t GetType(uint32_t dwHandle, const void *pOwner = nullptr);

  HRESULT Insert(uint32_t *pdwHandle, void *pObject, uint8_t type,
                 void *pOwner = nullptr);

  void *Delete(uint32_t dwHandle, const void *pOwner = nullptr);

  void Optimize();

  Enumerator GetEnumerator(const void *pOwner = nullptr) {
    return Enumerator(this, pOwner);
  }

  unsigned GetNumHandles() const { return m_nHandleCount; }

  uint32_t FindHandle(const void *pObject, const void *pOwner = nullptr) {
    CHandleTable::Enumerator enumerator = this->GetEnumerator(pOwner);
    while (!enumerator.IsEnd()) {
      if (pObject == enumerator.GetObject()) {
        return enumerator.GetHandle();
      }

      enumerator.MoveNext();
    }

    return 0;
  }

private:
  CHandleTable();
  ~CHandleTable();

  inline void Lock() { m_CS.lock(); }

  void Unlock() { m_CS.unlock(); }

  static unsigned GetHandleIndex(uint32_t dwHandle) {
    return (dwHandle & HANDLE_INDEX_MASK);
  }

  static unsigned GetHandleSerial(uint32_t dwHandle) {
    return (dwHandle & HANDLE_SERIAL_MASK) >> HANDLE_SERIAL_SHIFT;
  }

  Entry *GetEntry(uint32_t dwHandle) const {
    // Get the handle entry from the table
    const unsigned index = this->GetHandleIndex(dwHandle);
    if (index < m_dwSize) {
      // Validate the handle serial
      Entry *pEntry = m_pEntries + index;
      if (pEntry->Serial == this->GetHandleSerial(dwHandle)) {
        return pEntry;
      }
    }

    return nullptr;
  }

  Entry *DeleteEntry(Entry *pEntry);

  std::mutex m_CS;
  uint32_t m_dwSize;
  Entry *m_pEntries;
  Entry *m_pActiveList;
  unsigned m_nHandleCount;
};
