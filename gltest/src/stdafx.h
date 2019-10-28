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
DISABLE_WARNING_PUSH
DISABLE_WARNING_ANONYMOUS_STRUCT
#include <platform.hpp>
#include <string.h>

#ifdef _WIN32_WCE
#include <aygshell.h>
#endif
#include <EGL\egl.h>
#include <GLES\gl.h>
#include <math.h>

DISABLE_WARNING_POP
#include "gdifont.hpp"
#include "resource.h"

#include "fixed.hpp"
#include "mesh.hpp"
#include "shared.hpp"
#include "texture.hpp"
#include "timer.hpp"
