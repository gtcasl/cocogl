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

struct Rect {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
};

inline bool IntersectRect(Rect *pDst, const Rect *pSrc1, const Rect *pSrc2) {
  if (pDst) {
    auto left = std::max(pSrc1->left, pSrc2->left);
    auto top = std::max(pSrc1->top, pSrc2->top);
    auto right = std::max(pSrc1->right, pSrc2->right);
    auto bottom = std::max(pSrc1->bottom, pSrc2->bottom);
    if (left >= right || top >= bottom)
      return false;
    pDst->left = left;
    pDst->top = top;
    pDst->right = right;
    pDst->bottom = bottom;
    return true;
  } else {
    return !(pSrc1->left >= pSrc2->right || pSrc1->right <= pSrc2->left ||
             pSrc1->top >= pSrc2->bottom || pSrc1->bottom <= pSrc2->top);
  }
}