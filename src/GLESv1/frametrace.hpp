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

class RasterData;
class TexUnit;

namespace cocogfx {
struct CGLTrace;
}

class FrameTrace {
public:
    FrameTrace();
    ~FrameTrace();

    void beginTrace(const RASTERID& rasterid, const RasterData& rasterData, const Viewport& viewport);

    void capture(uint32_t i0, uint32_t i1, uint32_t i2, uint8_t *pbVertexData[VERTEX_SIZE]);

    void endTrace();

    int load(const char* filename);

    int save(const char* filename);

private:
    
    cocogfx::CGLTrace* trace_;
};
