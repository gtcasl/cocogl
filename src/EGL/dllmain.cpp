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
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#include "stdafx.h"
#include "config.hpp"
#include "context.hpp"
#include "display.hpp"
#include "driver.hpp"
#include "surface.hpp"

static CEGLDriver g_driver;

#ifndef NDEBUG

const TCHAR *SurfaceAttributeToString(EGLint attribute) {
  switch (attribute) {
  case EGL_CONFIG_ID:
    return _T("EGL_CONFIG_ID");
  case EGL_HEIGHT:
    return _T("EGL_HEIGHT");
  case EGL_WIDTH:
    return _T("EGL_WIDTH");
  case EGL_LARGEST_PBUFFER:
    return _T("EGL_LARGEST_PBUFFER");
  case EGL_TEXTURE_FORMAT:
    return _T("EGL_TEXTURE_FORMAT");
  case EGL_MIPMAP_TEXTURE:
    return _T("EGL_MIPMAP_TEXTURE");
  case EGL_MIPMAP_LEVEL:
    return _T("EGL_MIPMAP_LEVEL");
  }

  return _T("UNKNOWN");
}

const TCHAR *BufferToString(EGLint buffer) {
  switch (buffer) {
  case EGL_BACK_BUFFER:
    return _T("EGL_BACK_BUFFER");
  case EGL_SINGLE_BUFFER:
    return _T("EGL_SINGLE_BUFFER");
  }

  return _T("UNKNOWN");
}

const TCHAR *SurfaceTypeToString(EGLint readdraw) {
  switch (readdraw) {
  case EGL_DRAW:
    return _T("EGL_DRAW");
  case EGL_READ:
    return _T("EGL_READ");
  }

  return _T("UNKNOWN");
}

const TCHAR *EngineToString(EGLint engine) {
  switch (engine) {
  case EGL_CORE_NATIVE_ENGINE:
    return _T("EGL_CORE_NATIVE_ENGINE");
  }

  return _T("UNKNOWN");
}

#endif

EGLAPI EGLint EGLAPIENTRY eglGetError() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err = g_driver.GetError();

  __eglLog(_T(" - eglGetError()=%d\r\n"), err);

  return err;
}

EGLAPI void (*EGLAPIENTRY eglGetProcAddress(LPCSTR lpszProcName))() {
  __profileAPI(_T(" - %s( lpszProcName=%s )\n"), _T(__FUNCTION__), lpszProcName);

  typedef void (*PfnProcAddress)();
  
  struct t_procedure {
    LPCSTR lpszName;
    PfnProcAddress pfn;
  };

  static const t_procedure procedures[] = {
      {"glQueryMatrixxOES", reinterpret_cast<PfnProcAddress>(glQueryMatrixxOES)},
      {"glPointSizePointerOES", reinterpret_cast<PfnProcAddress>(glPointSizePointerOES)},
  };

  if (lpszProcName) {
    for (size_t i = 0; i < __countof(procedures); ++i) {
      if (!strcmp(procedures[i].lpszName, lpszProcName)) {
        return procedures[i].pfn;
      }
    }
  }

  return nullptr;
}

EGLAPI EGLDisplay EGLAPIENTRY eglGetDisplay(EGLNativeDisplayType display_id) {
  __profileAPI(_T(" - %s( display_id=%d )\n"), _T(__FUNCTION__), display_id);

  EGLint err;

  uint32_t dwHandle;
  err = g_driver.GetDisplay(&dwHandle, display_id);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLDriver::GetDisplay() failed, err = %d.\r\n"), err);
    return EGL_NO_DISPLAY;
  }

  return reinterpret_cast<EGLDisplay>(dwHandle);
}

EGLAPI EGLBoolean EGLAPIENTRY eglInitialize(EGLDisplay display, EGLint *pMajor,
                                            EGLint *pMinor) {
  __profileAPI(_T(" - %s( display=%d, pMajor=0x%p, pMinor=0x%p )\n"),
               _T(__FUNCTION__), display, pMajor, pMinor);

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Initialize the display
  EGLint err = pDisplay->Initialize(pMajor, pMinor);
  if (__eglFailed(err)) {
    __eglError(err, _T("CDisplay::Initialize() failed, err = %d.\r\n"), err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglTerminate(EGLDisplay display) {
  __profileAPI(_T(" - %s( display=%d )\n"), _T(__FUNCTION__), display);

  // Remove the display object from the handle table
  auto pDisplay = g_driver.UnregisterObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Delete the display
  pDisplay->Release();

  return EGL_TRUE;
}

EGLAPI const char *EGLAPIENTRY eglQueryString(EGLDisplay display, EGLint name) {
  __profileAPI(_T(" - %s( display=%d, name=%d )\n"), _T(__FUNCTION__), display,
               name);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return nullptr;
  }

  LPCSTR lpszValue;

  // Query the display string
  err = pDisplay->QueryString(&lpszValue, name);
  if (__eglFailed(err)) {
    __eglError(err, _T("CDisplay::QueryString() failed, err = %d.\r\n"), err);
    return nullptr;
  }

  return lpszValue;
}

EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigs(EGLDisplay display,
                                            EGLConfig *pConfigs,
                                            EGLint config_size,
                                            EGLint *pNum_config) {
  __profileAPI(_T(" - %s( display=%d, pConfigs=0x%p, config_size=%d, ")
               _T("pNum_config=0x%p )\n"),
               _T(__FUNCTION__), display, pConfigs, config_size, pNum_config);

  return eglChooseConfig(display, nullptr, pConfigs, config_size, pNum_config);
}

EGLAPI EGLBoolean EGLAPIENTRY eglChooseConfig(EGLDisplay display,
                                              const EGLint *pAttrib_list,
                                              EGLConfig *pConfigs,
                                              EGLint config_size,
                                              EGLint *pNum_config) {
  __profileAPI(_T(" - %s( display=%d, pAttrib_list=0x%p, pConfigs=0x%p, ")
               _T("config_size=%d, pNum_config=0x%p )\n"),
               _T(__FUNCTION__), display, pAttrib_list, pConfigs, config_size,
               pNum_config);

  EGLint err;

  // Validate num_config pointer
  if (nullptr == pNum_config) {
    __eglError(EGL_BAD_PARAMETER, _T("num_config is nullptr.\r\n"));
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Choose the display configurations
  err =
      pDisplay->ChooseConfig(pAttrib_list, pConfigs, config_size, pNum_config);
  if (__eglFailed(err)) {
    __eglError(err, _T("CDisplay::ChooseConfig() failed, err = %d.\r\n"), err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigAttrib(EGLDisplay display,
                                                 EGLConfig config,
                                                 EGLint attribute,
                                                 EGLint *pValue) {
  __profileAPI(
      _T(" - %s( display=%d, config=%d, attribute=%d, pValue=0x%p )\n"),
      _T(__FUNCTION__), display, config, attribute, pValue);

  EGLint err;

  // Verify value parameter
  if (nullptr == pValue) {
    __eglError(EGL_BAD_PARAMETER, _T("value is nullptr.\r\n"));
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the config object from the handle table
  auto pConfig = g_driver.TGetObject<CConfig *>(config, pDisplay);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, _T("Invalid config handle.\r\n"));
    return EGL_FALSE;
  }

  // Retrieve the attriubute value
  err = pConfig->GetAtttribute(attribute, pValue);
  if (__eglFailed(err)) {
    __eglError(err, _T("CConfig::GetAttribute() failed, err = %d.\r\n"), err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreateWindowSurface(EGLDisplay display, EGLConfig config,
                       EGLNativeWindowType hWnd, const EGLint *pAttrib_list) {
  __profileAPI(
      _T(" - %s( display=%d, config=%d, hWnd=0x%p, pAttrib_list=0x%p )\n"),
      _T(__FUNCTION__), display, config, hWnd, pAttrib_list);

  EGLint err;

  // Verify that the attribute list is nullptr or empty
  if (pAttrib_list && (*pAttrib_list != EGL_NONE)) {
    __eglError(EGL_BAD_ATTRIBUTE,
               _T("pAttrib_list should be nullptr or empty.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Find the config object from the handle table
  auto pConfig = g_driver.TGetObject<CConfig *>(config, pDisplay);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, _T("Invalid config handle.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Check if the config support this surface type
  if (0 == (pConfig->GetAttribute(EGL_SURFACE_TYPE) & EGL_WINDOW_BIT)) {
    __eglError(EGL_BAD_MATCH,
               _T("The config doesn't support this surface type.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Create a window surface
  CEGLSurface *pSurface;
  err = CEGLSurface::CreateWND(&pSurface, pDisplay, EGL_WINDOW_BIT, pConfig,
                               hWnd);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLSurface::Create() failed, err = %d.\r\n"), err);
    return EGL_NO_SURFACE;
  }

  // Create the new surface handle
  uint32_t dwHandle;
  err = g_driver.RegisterObject(&dwHandle, pSurface, HANDLE_SURFACE, pDisplay);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglError(err, _T("CEGLDriver::RegisterObject() failed, err = %d.\r\n"),
               err);
    return EGL_NO_SURFACE;
  }

  return reinterpret_cast<EGLSurface>(dwHandle);
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePixmapSurface(EGLDisplay display, EGLConfig config,
                       EGLNativePixmapType pixmap, const EGLint *pAttrib_list) {
  __profileAPI(
      _T(" - %s( display=%d, config=%d, pixmap=0x%x, pAttrib_list=0x%p )\n"),
      _T(__FUNCTION__), display, config, pixmap, pAttrib_list);

  EGLint err;

  // Verify that the attribute list is nullptr or empty
  if (pAttrib_list && (*pAttrib_list != EGL_NONE)) {
    __eglError(EGL_BAD_ATTRIBUTE,
               _T("pAttrib_list should be nullptr or empty.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Find the config object from the handle table
  auto pConfig = g_driver.TGetObject<CConfig *>(config, pDisplay);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, _T("Invalid config handle.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Check if the config support this surface type
  if (0 == (pConfig->GetAttribute(EGL_SURFACE_TYPE) & EGL_PIXMAP_BIT)) {
    __eglError(EGL_BAD_MATCH,
               _T("The config doesn't support this surface type.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Create a window surface
  CEGLSurface *pSurface;
  err = CEGLSurface::CreatePXM(&pSurface, pDisplay, EGL_PIXMAP_BIT, pConfig,
                               pixmap);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLSurface::Create() failed, err = %d.\r\n"), err);
    return EGL_NO_SURFACE;
  }

  // Create the new surface handle
  uint32_t dwHandle;
  err = g_driver.RegisterObject(&dwHandle, pSurface, HANDLE_SURFACE, pDisplay);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglError(err, _T("CEGLDriver::RegisterObject() failed, err = %d.\r\n"),
               err);
    return EGL_NO_SURFACE;
  }

  return reinterpret_cast<EGLSurface>(dwHandle);
}

EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferSurface(
    EGLDisplay display, EGLConfig config, const EGLint *pAttrib_list) {
  __profileAPI(_T(" - %s( display=%d, config=%d, pAttrib_list=0x%p )\n"),
               _T(__FUNCTION__), display, config, pAttrib_list);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Find the config object from the handle table
  auto pConfig = g_driver.TGetObject<CConfig *>(config, pDisplay);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, _T("Invalid config handle.\r\n"));
    return EGL_NO_SURFACE;
  }

  // Check if the config support this surface type
  if (0 == (pConfig->GetAttribute(EGL_SURFACE_TYPE) & EGL_PBUFFER_BIT)) {
    __eglError(EGL_BAD_MATCH,
               _T("The config doesn't support this surface type.\r\n"));
    return EGL_NO_SURFACE;
  }

  EGLint width = 0;
  EGLint height = 0;
  EGLint largestPBuffer = EGL_FALSE;
  EGLint texTarget = EGL_NO_TEXTURE;
  EGLint texFormat = EGL_NO_TEXTURE;
  EGLint bGenMipMaps = EGL_FALSE;

  if (pAttrib_list) {
    while (EGL_NONE != *pAttrib_list) {
      EGLint name = *pAttrib_list++;
      EGLint value = *pAttrib_list++;

      switch (name) {
      case EGL_WIDTH:
        width = value;
        break;

      case EGL_HEIGHT:
        height = value;
        break;

      case EGL_LARGEST_PBUFFER:
        largestPBuffer = value;
        break;

      case EGL_TEXTURE_TARGET:
        texTarget = value;
        break;

      case EGL_TEXTURE_FORMAT:
        texFormat = value;
        break;

      case EGL_MIPMAP_TEXTURE:
        bGenMipMaps = value;
        break;

      default:
        __eglError(EGL_BAD_ATTRIBUTE,
                   _T("Invalid pixel buffer attribute: %d\r\n"), name);
        return EGL_NO_SURFACE;
      }
    }
  }

  // Create a window surface
  CEGLSurface *pSurface;
  err = CEGLSurface::CreatePBF(&pSurface, pDisplay, EGL_PBUFFER_BIT, pConfig,
                               width, height, largestPBuffer, texTarget,
                               texFormat, bGenMipMaps);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLSurface::Create() failed, err = %d.\r\n"), err);
    return EGL_NO_SURFACE;
  }

  // Create the new surface handle
  uint32_t dwHandle;
  err = g_driver.RegisterObject(&dwHandle, pSurface, HANDLE_SURFACE, pDisplay);
  if (__eglFailed(err)) {
    __safeRelease(pSurface);
    __eglError(err, _T("CEGLDriver::RegisterObject() failed, err = %d.\r\n"),
               err);
    return EGL_NO_SURFACE;
  }

  return reinterpret_cast<EGLSurface>(dwHandle);
}

EGLAPI EGLBoolean EGLAPIENTRY eglDestroySurface(EGLDisplay display,
                                                EGLSurface surface) {
  __profileAPI(_T(" - %s( display=%d, surface=%d )\n"), _T(__FUNCTION__),
               display, surface);

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Remove the surface object from the handle table
  auto pSurface = g_driver.UnregisterObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, _T("Invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  // Delete the surface
  pSurface->Release();

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglQuerySurface(EGLDisplay display,
                                              EGLSurface surface,
                                              EGLint attribute,
                                              EGLint *pValue) {
  __profileAPI(
      _T(" - %s( display=%d, surface=%d, attribute=%s, pValue=0x%p )\n"),
      _T(__FUNCTION__), display, surface, SurfaceAttributeToString(attribute),
      pValue);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = g_driver.TGetObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, _T("Invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  // Query the surface attributes
  err = pSurface->GetAttribute(pValue, attribute);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLSurface::GetAttribute() failed, err = %d.\r\n"),
               err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglSurfaceAttrib(EGLDisplay display,
                                               EGLSurface surface,
                                               EGLint attribute, EGLint value) {
  __profileAPI(_T(" - %s( display=%d, surface=%d, attribute=%d, value=%d )\n"),
               _T(__FUNCTION__), display, surface,
               SurfaceAttributeToString(attribute), value);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY,
               _T("eglSurfaceAttrib() failed, invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(
        EGL_NOT_INITIALIZED,
        _T("eglSurfaceAttrib() failed, the display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = g_driver.TGetObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE,
               _T("eglSurfaceAttrib() failed, invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  err = pSurface->SetAttribute(attribute, value);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLSurface::SetAttribute() failed, err = %d.\r\n"),
               err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglBindTexImage(EGLDisplay display,
                                              EGLSurface surface,
                                              EGLint buffer) {
  __profileAPI(_T(" - %s( display=%d, surface=%d, buffer=%s )\n"),
               _T(__FUNCTION__), display, surface, BufferToString(buffer));

  EGLint err;

  // Validate the buffer
  if (EGL_BACK_BUFFER != buffer) {
    __eglError(
        EGL_BAD_PARAMETER,
        _T("eglBindTexImage() failed, invalid buffer parameter: %d.\r\n"),
        buffer);
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY,
               _T("eglBindTexImage() failed, invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(
        EGL_NOT_INITIALIZED,
        _T("eglBindTexImage() failed, the display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = g_driver.TGetObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE,
               _T("eglBindTexImage() failed, invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  if (surface == eglGetCurrentSurface(EGL_DRAW)) {
    // Perform an implicit GL flush
    glFlush();
  }

  // Bind the surface to the current texture
  err = pSurface->BindTexture();
  if (__eglFailed(err)) {
    __eglLogError(_T("CEGLSurface::BindTexture() failed, err = %d.\r\n"), err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglReleaseTexImage(EGLDisplay display,
                                                 EGLSurface surface,
                                                 EGLint buffer) {
  __profileAPI(_T(" - %s( display=%d, surface=%d, buffer=%s )\n"),
               _T(__FUNCTION__), display, surface, BufferToString(buffer));

  EGLint err;

  // Validate the buffer
  if (EGL_BACK_BUFFER != buffer) {
    __eglError(
        EGL_BAD_PARAMETER,
        _T("eglBindTexImage() failed, invalid buffer parameter: %d.\r\n"),
        buffer);
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = g_driver.TGetObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, _T("Invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  // Release the bound texture
  err = pSurface->ReleaseTexBound();
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLSurface::ReleaseTexBound() failed, err = %d.\r\n"),
               err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglSwapInterval(EGLDisplay display,
                                              EGLint interval) {
  __profileAPI(_T(" - %s( display=%d, interval=%d )\n"), _T(__FUNCTION__),
               display, interval);
  __unreferenced(interval);

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY,
               _T("eglSwapInterval() failed, invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(
        EGL_NOT_INITIALIZED,
        _T("eglSwapInterval() failed, the display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  auto pContext = g_driver.GetCurrentContext();
  if (pContext) {
    auto pSurface = pContext->GetDrawSurface();
    if (pSurface) {
      // We only support a fixed swap interval value of 1
      // Do nothing
      return EGL_TRUE;
    } else {
      __eglError(EGL_BAD_SURFACE,
                 _T("eglSwapInterval() failed, there is no surface bound to ")
                 _T("the current context.\r\n"));
      return EGL_FALSE;
    }
  } else {
    __eglError(EGL_BAD_CONTEXT,
               _T("eglSwapInterval() failed, there is no current context on ")
               _T("the calling thread.\r\n"));
    return EGL_FALSE;
  }
}

EGLAPI EGLContext EGLAPIENTRY eglCreateContext(EGLDisplay display,
                                               EGLConfig config,
                                               EGLContext share_context,
                                               const EGLint *pAttrib_list) {
  __profileAPI(_T(" - %s( display=%d, config=%d, share_context=%d, ")
               _T("pAttrib_list=0x%p )\n"),
               _T(__FUNCTION__), display, config, share_context, pAttrib_list);

  EGLint err;

  // Verify that the attribute list is nullptr or empty
  if (pAttrib_list && (*pAttrib_list != EGL_NONE)) {
    __eglError(EGL_BAD_ATTRIBUTE,
               _T("pAttrib_list should be nullptr or empty.\r\n"));
    return EGL_NO_CONTEXT;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_NO_CONTEXT;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_NO_CONTEXT;
  }

  // Find the config object from the handle table
  auto pConfig = g_driver.TGetObject<CConfig *>(config, pDisplay);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, _T("Invalid config handle.\r\n"));
    return EGL_NO_CONTEXT;
  }

  CEGLContext *pCtxShared = nullptr;

  if (EGL_NO_CONTEXT != share_context) {
    pCtxShared = g_driver.TGetObject<CEGLContext *>(share_context, pDisplay);
    if (nullptr == pCtxShared) {
      __eglError(EGL_BAD_CONTEXT, _T("Invalid shared context handle.\r\n"));
      return EGL_NO_CONTEXT;
    }
  }

  CEGLContext *pContext;
  err = CEGLContext::Create(&pContext, pDisplay, pConfig, pCtxShared);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLContext::Create() failed, err = %d.\r\n"), err);
    return EGL_NO_CONTEXT;
  }

  uint32_t dwHandle;
  err = g_driver.RegisterObject(&dwHandle, pContext, HANDLE_CONTEXT, pDisplay);
  if (__eglFailed(err)) {
    __safeRelease(pContext);
    __eglError(err, _T("CEGLDriver::RegisterObject() failed, err = %d.\r\n"),
               err);
    return EGL_NO_CONTEXT;
  }

  return reinterpret_cast<EGLContext>(dwHandle);
}

EGLAPI EGLBoolean EGLAPIENTRY eglDestroyContext(EGLDisplay display,
                                                EGLContext context) {
  __profileAPI(_T(" - %s( display=%d, context=%d )\n"), _T(__FUNCTION__),
               display, context);

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Remove the context object from the handle table
  auto pContext = g_driver.UnregisterObject<CEGLContext *>(context, pDisplay);
  if (nullptr == pContext) {
    __eglError(EGL_BAD_CONTEXT, _T("Invalid context handle.\r\n"));
    return EGL_FALSE;
  }

  // Delete the context
  pContext->Release();

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglMakeCurrent(EGLDisplay display,
                                             EGLSurface draw, EGLSurface read,
                                             EGLContext context) {
  EGLint err;

  __profileAPI(_T(" - %s( display=%d, draw=%d, read=%d, context=%d )\n"),
               _T(__FUNCTION__), display, draw, read, context);

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  CEGLSurface *pSurfDraw = nullptr;
  if (draw != EGL_NO_SURFACE) {
    // Find the draw surface object from the handle table
    pSurfDraw = g_driver.TGetObject<CEGLSurface *>(draw, pDisplay);
    if (nullptr == pSurfDraw) {
      __eglError(EGL_BAD_SURFACE, _T("Invalid draw surface handle.\r\n"));
      return EGL_FALSE;
    }
  }

  CEGLSurface *pSurfRead = nullptr;
  if (read != EGL_NO_SURFACE) {
    // Find the read surface object from the handle table
    pSurfRead = g_driver.TGetObject<CEGLSurface *>(read, pDisplay);
    if (nullptr == pSurfRead) {
      __eglError(EGL_BAD_SURFACE, _T("Invalid read surface handle.\r\n"));
      return EGL_FALSE;
    }
  }

  CEGLContext *pContext = nullptr;
  if (context != EGL_NO_CONTEXT) {
    // Find the context object from the handle table
    pContext = g_driver.TGetObject<CEGLContext *>(context, pDisplay);
    if (nullptr == pContext) {
      __eglError(EGL_BAD_SURFACE, _T("Invalid context handle.\r\n"));
      return EGL_FALSE;
    }
  }

  auto dwCurThreadID = std::this_thread::get_id();
  ;

  if (pContext) {
    if (pContext->HasBindings() && pContext->GetThreadID() != dwCurThreadID) {
      __eglError(EGL_BAD_ACCESS,
                 _T("The context is current to some other thread.\r\n"));
      return EGL_FALSE;
    }

    if ((nullptr == pSurfDraw) || (nullptr == pSurfRead)) {
      __eglError(EGL_BAD_MATCH,
                 _T("The context and surface handles are not compatible.\r\n"));
      return EGL_FALSE;
    }

    if (pSurfDraw->GetConfig() != pContext->GetConfig()) {
      __eglError(
          EGL_BAD_MATCH,
          _T("The context and draw surface configurations do not match.\r\n"));
      return EGL_FALSE;
    }

    if (pSurfRead->GetConfig() != pContext->GetConfig()) {
      __eglError(
          EGL_BAD_MATCH,
          _T("The context and read surface configurations do not match.\r\n"));
      return EGL_FALSE;
    }

    // Set the current GL context
    err = EGLERROR_FROM_HRESULT(__glMakeCurrent(pContext->GetNativeData(),
                                                pSurfDraw->GetNativeData(),
                                                pSurfRead->GetNativeData()));
    if (__eglFailed(err)) {
      __eglError(err, _T("__glMakeCurrent() failed, err = %d.\r\n"), err);
      return EGL_FALSE;
    }
  } else {
    if (pSurfDraw || pSurfRead) {
      __eglError(EGL_BAD_MATCH,
                 _T("The context and surface handles are not compatible.\r\n"));
      return EGL_FALSE;
    }

    // Clear the current GL context
    __glMakeCurrent(nullptr, nullptr, nullptr);
  }

  // Set the current EGL context
  g_driver.MakeCurrent(pContext, dwCurThreadID, pSurfDraw, pSurfRead);

  return EGL_TRUE;
}

EGLAPI EGLContext EGLAPIENTRY eglGetCurrentContext() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto pContext = g_driver.GetCurrentContext();
  if (pContext) {
    return reinterpret_cast<EGLContext>(
        g_driver.GetHandle(pContext, pContext->GetDisplay()));
  }

  return EGL_NO_CONTEXT;
}

EGLAPI EGLSurface EGLAPIENTRY eglGetCurrentSurface(EGLint readdraw) {
  __profileAPI(_T(" - %s( readdraw=%s )\n"), _T(__FUNCTION__),
               SurfaceTypeToString(readdraw));

  auto pContext = g_driver.GetCurrentContext();
  if (pContext) {
    switch (readdraw) {
    case EGL_DRAW:
      return reinterpret_cast<EGLSurface>(g_driver.GetHandle(
          pContext->GetDrawSurface(), pContext->GetDisplay()));

    case EGL_READ:
      return reinterpret_cast<EGLSurface>(g_driver.GetHandle(
          pContext->GetReadSurface(), pContext->GetDisplay()));
    }
  }

  return EGL_NO_SURFACE;
}

EGLAPI EGLDisplay EGLAPIENTRY eglGetCurrentDisplay() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto pContext = g_driver.GetCurrentContext();
  if (pContext) {
    return reinterpret_cast<EGLDisplay>(
        g_driver.GetHandle(pContext->GetDisplay()));
  }

  return EGL_NO_DISPLAY;
}

EGLAPI EGLBoolean EGLAPIENTRY eglQueryContext(EGLDisplay display,
                                              EGLContext context,
                                              EGLint attribute,
                                              EGLint *pValue) {
  __profileAPI(
      _T(" - %s( display=%d, context=%d, attribute=%d, pValue=0x%p )\n"),
      _T(__FUNCTION__), context, attribute, pValue);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the context object from the handle table
  auto pContext = g_driver.TGetObject<CEGLContext *>(context, pDisplay);
  if (nullptr == pContext) {
    __eglError(EGL_BAD_CONTEXT, _T("Invalid context handle.\r\n"));
    return EGL_FALSE;
  }

  // Query the context attributes
  err = pContext->GetAttribute(attribute, pValue);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLContext::GetAttribute() failed, err = %d.\r\n"),
               err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglWaitGL() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  glFinish();

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglWaitNative(EGLint engine) {
  __profileAPI(_T(" - %s( engine=%s )\n"), _T(__FUNCTION__),
               EngineToString(engine));

  if (EGL_CORE_NATIVE_ENGINE != engine) {
    __eglError(EGL_BAD_PARAMETER,
               _T("eglWaitNative() failed, invalid engine parameter: %d.\r\n"),
               engine);
    return EGL_FALSE;
  }

  // Validate the current context draw surface
  auto pContext = g_driver.GetCurrentContext();
  if (pContext) {
    auto pSurface = pContext->GetDrawSurface();
    if (!pSurface) {
      __eglError(EGL_BAD_CURRENT_SURFACE,
                 _T("eglWaitNative() failed, render surface not valid.\r\n"));
      return EGL_FALSE;
    }
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay display,
                                             EGLSurface surface) {
  __profileAPI(_T(" - %s( display=%d, surface=%d )\n"), _T(__FUNCTION__),
               display, surface);

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = g_driver.TGetObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, _T("Invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  // Only operate on window surfaces
  if (EGL_WINDOW_BIT != pSurface->GetType()) {
    __eglError(EGL_BAD_SURFACE, _T("Not a window rendering surface.\r\n"));
    return EGL_FALSE;
  }

  // Perform implicit glFlush
  glFlush();

  // Blit the surface to the display
  pSurface->Present();

#ifdef COCOGL_SNAPSHOTS
  static int s_fileno = 0;
  TCHAR szFilePath[360];
  if (s_fileno++ < COCOGL_SNAPSHOTS) {
    _sntprintf(szFilePath, 360, _T("snapshot%d.bmp"), s_fileno);
    pSurface->SaveBitmap(szFilePath);
  }
#endif

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglCopyBuffers(EGLDisplay display,
                                             EGLSurface surface,
                                             EGLNativePixmapType target) {
  __profileAPI(_T(" - %s( display=%d, surface=%d, target=0x%x )\n"),
               _T(__FUNCTION__), display, surface, target);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.TGetObject<CDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, _T("Invalid display handle.\r\n"));
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->IsInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, _T("The display was not initialized.\r\n"));
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = g_driver.TGetObject<CEGLSurface *>(surface, pDisplay);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, _T("Invalid surface handle.\r\n"));
    return EGL_FALSE;
  }

  // Copy the surface bits
  err = pSurface->CopyBuffer(target);
  if (__eglFailed(err)) {
    __eglError(err, _T("CEGLContext::GetAttribute() failed, err = %d.\r\n"),
               err);
    return EGL_FALSE;
  }

  // Perform implicit glFlush before returning
  glFlush();

  return EGL_TRUE;
}
