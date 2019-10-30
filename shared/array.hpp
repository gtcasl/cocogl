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

template <bool Active, class T, unsigned Size, class Allocator>
class TInlineArrayBuffer : public Allocator {
public:
  T *GetBuffer();
};

template <class T, unsigned Size, class Allocator>
class TInlineArrayBuffer<false, T, Size, Allocator> : public Allocator {
public:
  T *GetBuffer() { return nullptr; }
};

template <class T, unsigned Size, class Allocator>
class TInlineArrayBuffer<true, T, Size, Allocator> : public Allocator {
public:
  T *GetBuffer() { return m_buffer; }

private:
  T m_buffer[Size];
};

template <class T, unsigned GrowBy = 1, unsigned InitialCapacity = 0,
          class Allocator = StdAllocator>
class TArray : public Allocator {
protected:
  struct Node : public Allocator {
    T Data;
  };

  enum {
#ifdef _WIN32_WCE
    CACHE_LINE = 32, // Bytes
#else
    CACHE_LINE = 64, // Bytes
#endif
  };

  template <typename U>
  using detect_Copy_t = decltype(std::declval<U &>().Copy(std::declval<T &>()));

  typedef TInlineArrayBuffer<((InitialCapacity > 0) &&
                              ((InitialCapacity * sizeof(Node)) < CACHE_LINE))
                                 ? true
                                 : false,
                             Node, InitialCapacity, Allocator>
      InlineBuffer;

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

    T &operator*() const { return m_pNode->Data; }

    T *operator->() const { return &m_pNode->Data; }

    operator T *() const { return &m_pNode->Data; }

    Iter &operator++() {
      ++m_pNode;
      return *this;
    }

    Iter operator++(int) {
      Iter tmp(*this);
      ++(*this);
      return tmp;
    }

    Iter &operator--() {
      --m_pNode;
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

    friend class TArray<T, GrowBy, InitialCapacity, Allocator>;

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

    bool operator==(const cIter &rhs) const { return m_pNode == rhs.m_pNode; }

    bool operator!=(const cIter &rhs) const { return m_pNode != rhs.m_pNode; }

    const T &operator*() const { return m_pNode->Data; }

    const T *operator->() const { return &m_pNode->Data; }

    operator const T *() const { return &m_pNode->Data; }

    cIter &operator++() {
      ++m_pNode;
      return *this;
    }

    cIter operator++(int) {
      cIter tmp(*this);
      ++(*this);
      return tmp;
    }

    cIter &operator--() {
      --m_pNode;
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

    friend class TArray<T, GrowBy, InitialCapacity, Allocator>;

  private:
    const Node *m_pNode;
  };

  TArray() : m_pBuffer(NULL), m_size(0), m_capacity(0) {
    static_assert(GrowBy > 0, "invalid GrowBy value");
  }

  virtual ~TArray() { this->Clear(true); }

  HRESULT Reserve(unsigned size) {
    if (size > m_capacity) {
      return this->Allocate(size);
    }

    return S_OK;
  }

  HRESULT Resize(unsigned size, const T &value) {
    return __Resize(size, &value);
  }

  HRESULT Resize(unsigned size) { return __Resize(size); }

  HRESULT SetAt(unsigned index, const T &value) {
    HRESULT hr;
    if (index >= m_size) {
      hr = this->Resize(index + 1);
      if (FAILED(hr)) {
        return hr;
      }
    }

    (*this)[index] = value;

    return S_OK;
  }

  HRESULT SetAt(unsigned index, const T &value, const T &defValue) {
    HRESULT hr;

    if (index >= m_size) {
      hr = this->Resize(index + 1, defValue);
      if (FAILED(hr)) {
        return hr;
      }
    }

    (*this)[index] = value;

    return S_OK;
  }

  bool GetAt(unsigned index, T *pValue) const {
    if (index < m_size) {
      if (pValue) {
        *pValue = (*this)[index];
      }

      return true;
    }

    return false;
  }

  HRESULT Add(const T &value) {
    HRESULT hr;

    const unsigned index = m_size;
    hr = this->Resize(index + 1);
    if (SUCCEEDED(hr)) {
      (*this)[index] = value;
    }

    return hr;
  }

  HRESULT Copy(const TArray<T, GrowBy, InitialCapacity, Allocator> &rhs) {
    HRESULT hr;

    hr = this->Resize(rhs.m_size);
    if (SUCCEEDED(hr)) {
      for (unsigned i = 0, n = rhs.m_size; i < n; ++i) {
        if
          constexpr(is_detected_v<detect_Copy_t, T>) {
            hr = m_pBuffer[i].Data.Copy(rhs.m_pBuffer[i].Data);
            if (FAILED(hr)) {
              return hr;
            }
          }
        else {
          m_pBuffer[i].Data = rhs.m_pBuffer[i].Data;
        }
      }
    }

    return hr;
  }

  void Erase(unsigned index) {
    if (index < m_size) {
      for (unsigned j = index + 1, n = m_size; j < n; ++index, ++j) {
        m_pBuffer[index].Data = m_pBuffer[j].Data;
      }

      --m_size;
    }
  }

  void Erase(const Iter &iter) { this->Erase(iter.m_pNode - m_pBuffer); }

  void Erase(const T &item) {
    for (unsigned i = 0; i < m_size; ++i) {
      if (m_pBuffer[i].Data == item) {
        this->Erase(i);
      }
    }
  }

  bool Contains(const T &item) const {
    for (unsigned i = 0; i < m_size; ++i) {
      if (m_pBuffer[i].Data == item) {
        return true;
      }
    }

    return false;
  }

  void Clear(bool bFreeMemory = false) {
    if (bFreeMemory) {
      if (m_pBuffer) {
        if (m_capacity != InitialCapacity) {
          delete[] m_pBuffer;
        }
        m_pBuffer = NULL;
      }
      m_capacity = 0;
    }
    m_size = 0;
  }

  bool IsEmpty() const { return (0 == m_size); }

  unsigned GetSize() const { return m_size; }

  T &operator[](unsigned index) {
    ASSERT(index < m_size);
    return m_pBuffer[index].Data;
  }

  const T &operator[](unsigned index) const {
    ASSERT(index < m_size);
    return m_pBuffer[index].Data;
  }

  Iter GetBegin() { return Iter(m_pBuffer); }

  cIter GetBegin() const { return cIter(m_pBuffer); }

  Iter GetRBegin() { return Iter(m_pBuffer + m_size - 1); }

  cIter GetRBegin() const { return cIter(m_pBuffer + m_size - 1); }

  Iter GetEnd() { return Iter(m_pBuffer + m_size); }

  cIter GetEnd() const { return cIter(m_pBuffer + m_size); }

  Iter GetREnd() { return Iter(m_pBuffer - 1); }

  cIter GetREnd() const { return cIter(m_pBuffer - 1); }

  T &GetFront() {
    ASSERT(0 < m_size);
    return m_pBuffer[0];
  }

  const T &GetFront() const {
    ASSERT(0 < m_size);
    return m_pBuffer[0].Data;
  }

  T &GetBack() {
    ASSERT(0 < m_size);
    return m_pBuffer[m_size - 1].Data;
  }

  const T &GetBack() const {
    ASSERT(0 < m_size);
    return m_pBuffer[m_size - 1].Data;
  }

protected:
  TArray(const TArray<T, GrowBy, InitialCapacity, Allocator> &rhs) {}

  TArray<T, GrowBy, InitialCapacity, Allocator> &
  operator=(const TArray<T, GrowBy, InitialCapacity, Allocator> &rhs) {
    return *this;
  }

  HRESULT __Resize(unsigned size, const T *pValue = NULL) {
    HRESULT hr;

    if (size > m_size) {
      if (size > m_capacity) {
        hr = this->Allocate(size);
        if (FAILED(hr)) {
          return hr;
        }
      }

      if (pValue) {
        for (unsigned i = m_size; i < size; ++i) {
          if
            constexpr(is_detected_v<detect_Copy_t, T>) {
              hr = m_pBuffer[i].Data.Copy(*pValue);
              if (FAILED(hr)) {
                return hr;
              }
            }
          else {
            m_pBuffer[i].Data = *pValue;
          }
        }
      }
    }

    m_size = size;

    return S_OK;
  }

  HRESULT Allocate(unsigned size) {
    HRESULT hr = S_OK;

    unsigned capacity;

    if (size <= InitialCapacity) {
      capacity = InitialCapacity;
    } else {
      capacity = m_capacity;

      while (capacity < size) {
        capacity += GrowBy;
      }
    }

    Node *pBuffer;

    if (capacity == InitialCapacity) {
      pBuffer = m_inlineBuffer.GetBuffer();
    } else {
      pBuffer = new Node[capacity];
      if (NULL == pBuffer) {
        hr = E_OUTOFMEMORY;
        goto Cleanup;
      }
    }

    if (m_pBuffer) {
      for (unsigned i = 0, n = m_size; i < n; ++i) {
        if
          constexpr(is_detected_v<detect_Copy_t, T>) {
            hr = pBuffer[i].Data.Copy(m_pBuffer[i].Data);
            if (FAILED(hr)) {
              goto Cleanup;
            }
          }
        else {
          pBuffer[i].Data = m_pBuffer[i].Data;
        }
      }

      if (m_capacity != InitialCapacity) {
        delete[] m_pBuffer;
      }
    }

  Cleanup:

    if (SUCCEEDED(hr)) {
      m_pBuffer = pBuffer;
      m_capacity = capacity;
    } else {
      if (pBuffer && (m_capacity != InitialCapacity)) {
        delete[] pBuffer;
      }
    }

    return hr;
  }

  InlineBuffer m_inlineBuffer;
  Node *m_pBuffer;
  unsigned m_size;
  unsigned m_capacity;
};
