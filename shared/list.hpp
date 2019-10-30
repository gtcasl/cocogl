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

template <class T1, class T2> struct TPair {
  T1 First;
  T2 Second;

  typedef T1 Type1;
  typedef T2 Type2;

  TPair(const T1 &first, const T2 &second) {
    this->First = first;
    this->Second = second;
  }
};

template <class T, class Allocator = StdAllocator>
class TList : public Allocator {
protected:
  struct Node : public Allocator {
    T Data;
    Node *pNext;
    Node *pPrev;

    Node(const T &data) : Data(data), pNext(NULL), pPrev(NULL) {}
  };

public:
  typedef T Type;

  class cIter;
  class Iter {
  public:
    Iter() : m_pNode(NULL) {}

    Iter &operator=(const Iter &rhs) {
      m_pNode = rhs.m_pNode;
      return *this;
    }

    bool operator==(const Iter &rhs) const { return m_pNode == rhs.m_pNode; }

    bool operator!=(const Iter &rhs) const { return m_pNode != rhs.m_pNode; }

    T &operator*() const {
      ASSERT(m_pNode);
      return m_pNode->Data;
    }

    T *operator->() const {
      ASSERT(m_pNode);
      return &m_pNode->Data;
    }

    operator T *() const {
      ASSERT(m_pNode);
      return &m_pNode->Data;
    }

    Iter &operator++() {
      ASSERT(m_pNode);
      m_pNode = m_pNode->pNext;
      return *this;
    }

    Iter operator++(int) {
      Iter tmp(*this);
      ++(*this);
      return tmp;
    }

    Iter &operator--() {
      ASSERT(m_pNode);
      m_pNode = m_pNode->pPrev;
      return *this;
    }

    Iter operator--(int) {
      Iter tmp(*this);
      --(*this);
      return tmp;
    }

    Iter operator+(int steps) const {
      Iter tmp(*this);
      if (steps > 0) {
        while (steps--) {
          ++tmp;
        }
      } else {
        while (steps++) {
          --tmp;
        }
      }

      return tmp;
    }

    Iter operator-(int steps) const {
      Iter tmp(*this);
      if (steps > 0) {
        while (steps--) {
          --tmp;
        }
      } else {
        while (steps++) {
          ++tmp;
        }
      }

      return tmp;
    }

  protected:
    Iter(Node *pNode) : m_pNode(pNode) {}

    friend class TList<T, Allocator>;

  private:
    Node *m_pNode;
    friend class cIter;
  };

  class cIter {
  public:
    cIter() : m_pNode(NULL) {}

    cIter(const Iter &rhs) : m_pNode(rhs.m_pNode) {}

    cIter &operator=(const cIter &rhs) {
      m_pNode = rhs.m_pNode;
      return *this;
    }

    bool operator==(const cIter &rhs) const { return (m_pNode == rhs.m_pNode); }

    bool operator!=(const cIter &rhs) const { return (m_pNode != rhs.m_pNode); }

    const T &operator*() const {
      ASSERT(m_pNode);
      return m_pNode->Data;
    }

    const T *operator->() const {
      ASSERT(m_pNode);
      return &m_pNode->Data;
    }

    operator const T *() const {
      ASSERT(m_pNode);
      return &m_pNode->Data;
    }

    cIter &operator++() {
      ASSERT(m_pNode);
      m_pNode = m_pNode->pNext;
      return *this;
    }

    cIter operator++(int) {
      cIter tmp(*this);
      ++(*this);
      return tmp;
    }

    cIter &operator--() {
      ASSERT(m_pNode);
      m_pNode = m_pNode->pPrev;
      return *this;
    }

    cIter operator--(int) {
      cIter tmp(*this);
      --(*this);
      return tmp;
    }

    cIter operator+(int steps) const {
      cIter tmp(*this);
      if (steps > 0) {
        while (steps--) {
          ++tmp;
        }
      } else {
        while (steps++) {
          --tmp;
        }
      }

      return tmp;
    }

    cIter operator-(int steps) const {
      cIter tmp(*this);
      if (steps > 0) {
        while (steps--) {
          --tmp;
        }
      } else {
        while (steps++) {
          ++tmp;
        }
      }

      return tmp;
    }

  protected:
    cIter(const Node *pNode) : m_pNode(pNode) {}

    friend class TList<T, Allocator>;

  private:
    const Node *m_pNode;
  };

  TList() : m_size(0) {
    m_pEnd = (Node *)((uint8_t *)&m_pEndNext - offsetof(Node, pNext));
    m_pEnd->pPrev = m_pEnd->pNext = m_pEnd;
  }

  virtual ~TList() { this->Clear(); }

  HRESULT CopyBack(const TList<T, Allocator> &rhs) {
    HRESULT hr;

    for (TList::cIter iter = rhs.GetBegin(), iterEnd = rhs.GetEnd();
         iter != iterEnd; ++iter) {
      __retOnFail(this->PushBack(*iter));
    }

    return S_OK;
  }

  HRESULT CopyFront(const TList<T, Allocator> &rhs) {
    HRESULT hr;

    for (TList::cIter iter = rhs.GetBegin(), iterEnd = rhs.GetEnd();
         iter != iterEnd; ++iter) {
      __retOnFail(this->PushFront(*iter));
    }

    return S_OK;
  }

  HRESULT PushBack(const T &item) {
    Node *const pNew = new Node(item);
    if (NULL == pNew) {
      return E_OUTOFMEMORY;
    }

    pNew->pNext = m_pEnd;
    pNew->pPrev = m_pEnd->pPrev;
    m_pEnd->pPrev->pNext = pNew;
    m_pEnd->pPrev = pNew;

    ++m_size;

    return S_OK;
  }

  HRESULT PushFront(const T &item) {
    Node *const pNew = new Node(item);
    if (NULL == pNew) {
      return E_OUTOFMEMORY;
    }

    pNew->pNext = m_pEnd->pNext;
    pNew->pPrev = m_pEnd;
    m_pEnd->pNext->pPrev = pNew;
    m_pEnd->pNext = pNew;

    ++m_size;

    return S_OK;
  }

  void PopBack() {
    ASSERT(m_size);

    Node *const pTail = m_pEnd->pPrev;
    pTail->pPrev->pNext = m_pEnd;
    m_pEnd->pPrev = pTail->pPrev;

    delete pTail;

    --m_size;
  }

  void PopFront() {
    ASSERT(m_size);

    Node *const pHead = m_pEnd->pNext;
    pHead->pNext->pPrev = m_pEnd;
    m_pEnd->pNext = pHead->pNext;

    delete pHead;

    --m_size;
  }

  // Insert an item before the given iterator
  HRESULT Insert(const Iter &iter, const T &item) {
    Node *const pNew = new Node(item);
    if (NULL == pNew) {
      return E_OUTOFMEMORY;
    }

    Node *const pNode = iter.m_pNode;
    ASSERT(pNode);
    pNew->pNext = pNode;
    pNew->pPrev = pNode->pPrev;
    pNode->pPrev->pNext = pNew;
    pNode->pPrev = pNew;

    ++m_size;

    return S_OK;
  }

  void Splice(const Iter &iterDst, TList &listSrc, const Iter &iterSrc) {
    Node *const pSrc = iterSrc.m_pNode;
    ASSERT(pSrc);

    Node *const pDst = iterDst.m_pNode;
    ASSERT(pDst);

    if ((pSrc == pDst) && (this == &listSrc)) {
      return;
    }

    // Detach the source node
    pSrc->pNext->pPrev = pSrc->pPrev;
    pSrc->pPrev->pNext = pSrc->pNext;

    // Insert the source node into its new location
    pSrc->pNext = pDst;
    pSrc->pPrev = pDst->pPrev;
    pDst->pPrev->pNext = pSrc;
    pDst->pPrev = pSrc;

    if (this != &listSrc) {
      ++m_size;
      --listSrc.m_size;
    }
  }

  void Erase(const Iter &iter) {
    Node *const pNode = iter.m_pNode;
    ASSERT(pNode);

    pNode->pPrev->pNext = pNode->pNext;
    pNode->pNext->pPrev = pNode->pPrev;

    delete pNode;

    --m_size;
  }

  void Erase(const Iter &iterFirst, const Iter &iterLast) {
    for (Iter iter = iterFirst; iter != iterLast;) {
      Iter iterCur = iter++;
      this->Erase(iterCur);
    }
  }

  void Erase(const T &item) {
    for (Iter iter = this->GetBegin(), iterEnd = this->GetEnd();
         iter != iterEnd;) {
      Iter iterCur = iter++;
      if (*iterCur == item) {
        this->Erase(iterCur);
      }
    }
  }

  Iter Find(const T &item) {
    Iter iter = this->GetBegin();
    for (Iter iterEnd = this->GetEnd(); iter != iterEnd; ++iter) {
      if (*iter == item) {
        break;
      }
    }
    return iter;
  }

  cIter Find(const T &item) const {
    cIter iter = this->GetBegin();
    for (cIter iterEnd = this->GetEnd(); iter != iterEnd; ++iter) {
      if (*iter == item) {
        break;
      }
    }
    return iter;
  }

  bool Contains(const T &item) const {
    return (this->Find(item) != this->GetEnd());
  }

  void Clear() { this->Erase(this->GetBegin(), this->GetEnd()); }

  HRESULT Copy(const TList<T, Allocator> &rhs) {
    HRESULT hr;

    for (TList::cIter iter = rhs.GetBegin(), iterEnd = rhs.GetEnd();
         iter != iterEnd;) {
      hr = this->PushBack(*iter++);
      if (FAILED(hr)) {
        return hr;
      }
    }

    return S_OK;
  }

  HRESULT Merge(const TList<T, Allocator> &rhs) {
    HRESULT hr;

    for (TList::cIter iter = rhs.GetBegin(), iterEnd = rhs.GetEnd();
         iter != iterEnd;) {
      T object = *iter++;
      if (!this->Contains(object)) {
        hr = this->PushBack(object);
        if (FAILED(hr)) {
          return hr;
        }
      }
    }

    return S_OK;
  }

  template <typename Compare> void Sort(Compare compare) {
    for (TList::Iter iterStart = this->GetBegin(), iterEnd = this->GetEnd(),
                     iter = iterStart;
         iter != iterEnd;) {
      TList::Iter iterNext = iter++;

      for (TList::Iter iterCur = this->GetBegin(); iterCur != iterNext;
           ++iterCur) {
        if (compare(*iterNext, *iterCur)) {
          this->Splice(iterCur, *this, iterNext);
          break;
        }
      }
    }
  }

  bool IsEmpty() const { return (0 == m_size); }

  size_t GetSize() const { return m_size; }

  Iter GetBegin() {
    ASSERT(m_pEnd);
    return Iter(m_pEnd->pNext);
  }

  cIter GetBegin() const {
    ASSERT(m_pEnd);
    return cIter(m_pEnd->pNext);
  }

  Iter GetEnd() { return Iter(m_pEnd); }

  cIter GetEnd() const { return cIter(m_pEnd); }

  Iter GetRBegin() {
    ASSERT(m_pEnd);
    return Iter(m_pEnd->pPrev);
  }

  cIter GetRBegin() const {
    ASSERT(m_pEnd);
    return cIter(m_pEnd->pPrev);
  }

  Iter GetREnd() { return Iter(m_pEnd); }

  cIter GetREnd() const { return cIter(m_pEnd); }

  T &GetFront() {
    ASSERT(m_pEnd && m_pEnd->pNext);
    return m_pEnd->pNext->Data;
  }

  const T &GetFront() const {
    ASSERT(m_pEnd && m_pEnd->pNext);
    return m_pEnd->pNext->Data;
  }

  T &GetBack() {
    ASSERT(m_pEnd && m_pEnd->pPrev);
    return m_pEnd->pPrev->Data;
  }

  const T &GetBack() const {
    ASSERT(m_pEnd && m_pEnd->pPrev);
    return m_pEnd->pPrev->Data;
  }

protected:
  TList(const TList<T, Allocator> &rhs) {}

  TList<T, Allocator> &operator=(const TList<T, Allocator> &rhs) {
    return *this;
  }

  Node *m_pEnd;
  Node *m_pEndNext;
  Node *m_pEndPrev;
  size_t m_size;
};
