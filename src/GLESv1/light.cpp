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
#include "stdafx.h"
#include "light.hpp"
#include "context.hpp"

void CGLContext::ActivateLight(GLenum cap, bool bValue) {
  auto pLight = m_lights.GetBegin() + (cap - GL_LIGHT0);
  Light *pPrevLight = nullptr;
  Light *pCurLight = m_pActiveLights;

  if (bValue) {
    // Add the light to the active list ( sorted by address )
    while (pCurLight && (pCurLight < pLight)) {
      pPrevLight = pCurLight;
      pCurLight = pCurLight->pNext;
    }

    if ((nullptr == pCurLight) || (pCurLight > pLight)) {
      pLight->pNext = pCurLight;

      if (pPrevLight) {
        pPrevLight->pNext = pLight;
      } else {
        m_pActiveLights = pLight;
      }
    }
  } else {
    // Remove the light from the active list
    while (pCurLight) {
      if (pCurLight == pLight) {
        if (pPrevLight) {
          pPrevLight->pNext = pCurLight->pNext;
        } else {
          m_pActiveLights = pCurLight->pNext;
        }

        break;
      }

      pPrevLight = pCurLight;
      pCurLight = pCurLight->pNext;
    }
  }
}
