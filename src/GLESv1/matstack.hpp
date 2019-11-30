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

class CMatrixStack : public CObject {
public:
  ~CMatrixStack();

  static GLenum Create(CMatrixStack **ppMatrixStack, uint8_t size);

  void ToIdentity() {
    pMatrices_[curIndex_].ToIdentity();
    this->SetIdentity(true);
  }

  void SetMatrix(const MATRIX44 &matrix) {
    pMatrices_[curIndex_] = matrix;
    this->SetIdentity(false);
  }

  const MATRIX44 &GetMatrix() const { return pMatrices_[curIndex_]; }

  bool Push() {
    if (curIndex_ + 1 < size_) {
      pMatrices_[curIndex_ + 1] = pMatrices_[curIndex_];
      identityMask_ = (identityMask_ & ~(2 << curIndex_)) |
                      ((identityMask_ & (1 << curIndex_)) << 1);
      ++curIndex_;
      return true;
    }

    return false;
  }

  bool Pop() {
    if (curIndex_ > 0) {
      --curIndex_;
      return true;
    }

    return false;
  }

  bool IsIdentity() const { return identityMask_ & (1 << curIndex_); }

  uint32_t GetSize() const { return size_; }

  template <class T> void TGetTop(T *pDst) const {
    assert(pDst);

    auto pSrc = pMatrices_[curIndex_]._m;
    for (uint32_t i = 0; i < 16; ++i) {
      pDst[i] = Math::TCast<T>(pSrc[i]);
    }
  }

private:
  GLenum Initialize(uint8_t size);

  CMatrixStack();

  void SetIdentity(bool bValue) {
    if (bValue) {
      identityMask_ |= (1 << curIndex_);
    } else {
      identityMask_ &= ~(1 << curIndex_);
    }
  }

  MATRIX44 *pMatrices_;
  uint16_t identityMask_;
  uint8_t curIndex_;
  uint8_t size_;
};
