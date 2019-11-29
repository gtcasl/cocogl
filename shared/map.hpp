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
#include "list.hpp"

template <class K, class V, size_t B = 64, class A = StdAllocator>
class TMap : public A {
public:
  using KeyType = K;
  using ValueType = V;
  static const size_t BucketSize = B;
  using Allocator = A;

  typedef TList<TPair<K, V>, A> List;
  typedef typename List::Iter Iter;
  typedef typename List::cIter cIter;

  TMap() {
    for (uint32_t i = 0; i < BucketSize; ++i) {
      m_buckets[i].Iter = m_list.GetEnd();
      m_buckets[i].Size = 0;
    }
  }

  virtual ~TMap() {}

  HRESULT Copy(const TMap &rhs) {
    HRESULT hr;
    for (cIter iter = rhs.GetBegin(), iterEnd = rhs.GetEnd(); iter != iterEnd;
         ++iter) {
      hr = this->Insert(iter->First, iter->Second);
      if (FAILED(hr)) {
        return hr;
      }
    }

    return S_OK;
  }

  HRESULT Insert(const KeyType &key, const ValueType &value) {
    HRESULT hr;

    // Find the bucket the key corresponds to
    Bucket &bucket = this->GetBucket(key);

    // Ensure no duplicates exist
    size_t count = bucket.Size;
    for (Iter iter = bucket.Iter; count--; ++iter) {
      if (iter->First == key) {
        return S_FALSE;
      }
    }

    // Insert the new entry to the front of the current bucket
    hr = m_list.Insert(bucket.Iter, TPair<KeyType, ValueType>(key, value));
    if (FAILED(hr)) {
      return hr;
    }

    // Update the bucket
    --bucket.Iter;
    ++bucket.Size;

    return S_OK;
  }

  HRESULT Reserve(ValueType **ppValue, const KeyType &key) {
    HRESULT hr;

    ASSERT(ppValue);

    // Find the bucket the key corresponds to
    Bucket &bucket = this->GetBucket(key);

    // Ensure no duplicates exist
    size_t count = bucket.Size;
    for (Iter iter = bucket.Iter; count--; ++iter) {
      if (iter->First == key) {
        return S_FALSE;
      }
    }

    // Reserve the new entry to the front of the current bucket
    hr = m_list.Reserve(bucket.Iter);
    if (FAILED(hr)) {
      return hr;
    }

    // Update the bucket
    --bucket.Iter;
    ++bucket.Size;

    bucket.Iter->First = key;
    *ppValue = bucket.Iter->Second;

    return S_OK;
  }

  bool Lookup(const KeyType &key, ValueType *pValue) {
    ASSERT(pValue);

    Bucket &bucket = this->GetBucket(key);
    size_t count = bucket.Size;
    for (Iter iter = bucket.Iter; count--; ++iter) {
      if (iter->First == key) {
        // Move the entry to the front
        m_list.Splice(bucket.Iter, m_list, iter);
        bucket.Iter = iter;

        // Return the value
        *pValue = iter->Second;
        return true;
      }
    }

    return false;
  }

  bool Contains(const KeyType &key) const {
    Bucket &bucket = this->GetBucket(key);
    size_t count = bucket.Size;
    for (Iter iter = bucket.Iter; count--; ++iter) {
      if (iter->First == key) {
        return true;
      }
    }

    return false;
  }

  void Erase(const KeyType &key) {
    Bucket &bucket = this->GetBucket(key);
    size_t count = bucket.Size;
    for (Iter iter = bucket.Iter; count--; ++iter) {
      if (iter->First == key) {
        --bucket.Size;
        if (iter == bucket.Iter) {
          if (bucket.Size) {
            ++bucket.Iter;
          } else {
            bucket.Iter = m_list.GetEnd();
          }
        }

        m_list.Erase(iter);
        return;
      }
    }
  }

  void Clear() {
    for (size_t i = 0; i < BucketSize; ++i) {
      m_buckets[i].Iter = m_list.GetEnd();
      m_buckets[i].Size = 0;
    }

    m_list.Clear();
  }

  bool IsEmpty() const { return m_list.IsEmpty(); }

  size_t GetSize() const { return m_list.GetSize(); }

  HRESULT ToList(List *pOut) const {
    ASSERT(pOut);
    return pOut->Copy(m_list);
  }

  Iter GetBegin() { return m_list.GetBegin(); }

  Iter GetRBegin() { return m_list.GetRBegin(); }

  cIter GetBegin() const { return m_list.GetBegin(); }

  cIter GetRBegin() const { return m_list.GetRBegin(); }

  Iter GetEnd() { return m_list.GetEnd(); }

  Iter GetREnd() { return m_list.GetREnd(); }

  cIter GetEnd() const { return m_list.GetEnd(); }

  cIter GetREnd() const { return m_list.GetREnd(); }

  Iter GetBucketIter(uint32_t index) {
    ASSERT(index < BucketSize);
    return m_buckets[BucketSize].Iter;
  }

  cIter GetBucketIter(uint32_t index) const {
    ASSERT(index < BucketSize);
    return m_buckets[BucketSize].Iter;
  }

  uint32_t GetBucketSize(uint32_t index) {
    ASSERT(index < BucketSize);
    return m_buckets[BucketSize].Size;
  }

  Allocator &GetAllocator() { return m_list.GetAllocator(); }

protected:
  struct Bucket {
    typename List::Iter Iter;
    size_t Size;
  };

  Bucket m_buckets[BucketSize];
  List m_list;

  TMap(const TMap &rhs) {}

  TMap &operator=(const TMap &rhs) { return *this; }

  Bucket &GetBucket(const KeyType &key) {
    auto hash = static_cast<uint32_t>(key);
    uint32_t index = hash % BucketSize;
    return m_buckets[index];
  }
};
