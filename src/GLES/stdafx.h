//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#pragma once

#define GL_GLEXT_PROTOTYPES

//#define COCOGL_RASTER_PROFILE
//#define COCOGL_PIXEDPOINT
//#define COCOGL_API_PROFILE

#include <string.h>
#include <mutex>
#include <chrono>
#include <climits>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <platform.hpp>
#include <utils.hpp>
#include <object.hpp>
#include <malloc.hpp>
#include <handle.hpp>
#include <list.hpp>
#include <array.hpp>
#include <map.hpp>
#include <rect.hpp>
#include <color.hpp>
#include <bitmap.hpp>
#include <format.hpp>
#include <fixed.hpp>
#include <int24.hpp>
#include <math.hpp>
#include <logger.hpp>
#include <profiler.hpp>
#include <color.inl>
#include <fixed.inl>
#include <format.inl>
#include <math.inl>
#include "glstub.hpp"
#include "types.hpp"
#include "utility.hpp"

extern CLogger glLogger;