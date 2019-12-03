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

class MatrixStack : public Object {
public:
  ~MatrixStack();

  static GLenum Create(MatrixStack **ppMatrixStack, uint8_t size);

  void toIdentity() {
    pMatrices_[curIndex_].toIdentity();
    this->setIdentity(true);
  }

  void setMatrix(const MATRIX44 &matrix) {
    pMatrices_[curIndex_] = matrix;
    this->setIdentity(false);
  }

  const MATRIX44 &getMatrix() const { return pMatrices_[curIndex_]; }

  bool push() {
    if (curIndex_ + 1 < size_) {
      pMatrices_[curIndex_ + 1] = pMatrices_[curIndex_];
      identityMask_ = (identityMask_ & ~(2 << curIndex_)) |
                      ((identityMask_ & (1 << curIndex_)) << 1);
      ++curIndex_;
      return true;
    }

    return false;
  }

  bool pop() {
    if (curIndex_ > 0) {
      --curIndex_;
      return true;
    }

    return false;
  }

  bool isIdentity() const { return identityMask_ & (1 << curIndex_); }

  uint32_t getSize() const { return size_; }

  template <typename T> void getTop(T *pDst) const {
    assert(pDst);

    auto pSrc = pMatrices_[curIndex_]._m;
    for (uint32_t i = 0; i < 16; ++i) {
      pDst[i] = static_cast<T>(pSrc[i]);
    }
  }

private:
  GLenum initialize(uint8_t size);

  MatrixStack();

  void setIdentity(bool bValue) {
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
