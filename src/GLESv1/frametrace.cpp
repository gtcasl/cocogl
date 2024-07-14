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

#include <iostream>
#include <iterator>
#include "frametrace.hpp"
#include "texture.hpp"
#include "rastdata.hpp"
#include <cgltrace.hpp>

static void capture(CGLTrace::drawcall_t* drawcall, uint32_t index, uint8_t *pbVertexData[VERTEX_SIZE]) {
  auto it = drawcall->vertices.find(index);
  if (it != drawcall->vertices.end())
    return;

  auto pvClipPos = reinterpret_cast<const VECTOR4*>(pbVertexData[VERTEX_CLIPPOS]);
  auto pvColors = reinterpret_cast<const ColorARGB*>(pbVertexData[VERTEX_COLOR0]);
  auto pTexCoords = reinterpret_cast<const TEXCOORD2*>(pbVertexData[VERTEX_TEXCOORD0]);

  CGLTrace::vertex_t vertex;

  vertex.pos.x = static_cast<float>(pvClipPos[index].x);
  vertex.pos.y = static_cast<float>(pvClipPos[index].y);
  vertex.pos.z = static_cast<float>(pvClipPos[index].z);
  vertex.pos.w = static_cast<float>(pvClipPos[index].w);

  if (drawcall->states.color_enabled) {
    vertex.color.r = pvColors[index].r / 255.0f;
    vertex.color.g = pvColors[index].g / 255.0f;
    vertex.color.b = pvColors[index].b / 255.0f;
    vertex.color.a = pvColors[index].a / 255.0f;
  } else {
    vertex.color.r = 1.0f;
    vertex.color.g = 1.0f;
    vertex.color.b = 1.0f;
    vertex.color.a = 1.0f;
  }

  if (drawcall->states.texture_enabled) {
    vertex.texcoord.u = static_cast<float>(pTexCoords[index].u);
    vertex.texcoord.v = static_cast<float>(pTexCoords[index].v);
  } else {
    vertex.texcoord.u = 0.0f;
    vertex.texcoord.v = 0.0f;
  }

  drawcall->vertices.emplace(index, vertex);
}

FrameTrace::FrameTrace() {
  trace_ = new CGLTrace();
}

FrameTrace::~FrameTrace() {
  delete trace_;
}

void FrameTrace::beginTrace(const RASTERID& rasterid, const RasterData& rasterData, const Viewport& viewport) {
  if (trace_->drawcalls.empty()
   || !trace_->drawcalls.back().vertices.empty()) {
    trace_->drawcalls.emplace_back();
  }
  CGLTrace::drawcall_t& drawcall = trace_->drawcalls.back();

  drawcall.states.color_enabled = rasterid.Flags.Color;
  drawcall.states.color_format = (ePixelFormat)rasterData.ColorFormat;
  drawcall.states.color_writemask = rasterData.ColorWriteMask;

  drawcall.states.depth_test = rasterid.Flags.DepthTest;
  drawcall.states.depth_format = (ePixelFormat)rasterData.DepthStencilFormat;
  drawcall.states.depth_writemask = rasterid.Flags.DepthWrite;
  drawcall.states.depth_func = cocogfx::CGLTrace::ecompare(rasterid.States.DepthFunc);

  drawcall.states.stencil_test = rasterid.Flags.StencilTest;
  drawcall.states.stencil_func = cocogfx::CGLTrace::ecompare(rasterid.States.StencilFunc);
  drawcall.states.stencil_zpass = cocogfx::CGLTrace::eStencilOp(rasterid.States.StencilZPass);
  drawcall.states.stencil_zfail = cocogfx::CGLTrace::eStencilOp(rasterid.States.StencilZFail);
  drawcall.states.stencil_fail = cocogfx::CGLTrace::eStencilOp(rasterid.States.StencilFail);
  drawcall.states.stencil_ref  = rasterData.StencilRef;
  drawcall.states.stencil_mask = rasterData.StencilMask;
  drawcall.states.stencil_writemask = rasterData.StencilWriteMask;

  drawcall.states.texture_enabled   = (rasterid.Flags.NumTextures != 0);

  drawcall.states.texture_envcolor.b  = rasterData.Samplers[0].cEnvColor[0] / 255.0f;
  drawcall.states.texture_envcolor.g  = rasterData.Samplers[0].cEnvColor[1] / 255.0f;
  drawcall.states.texture_envcolor.r  = rasterData.Samplers[0].cEnvColor[2] / 255.0f;
  drawcall.states.texture_envcolor.a  = 1.0f;

  drawcall.states.texture_envmode   = cocogfx::CGLTrace::eEnvMode(rasterid.Textures[0].EnvMode);
  drawcall.states.texture_minfilter = cocogfx::CGLTrace::eTexFilter(rasterid.Textures[0].MinFilter);
  drawcall.states.texture_magfilter = cocogfx::CGLTrace::eTexFilter(rasterid.Textures[0].MagFilter);
  drawcall.states.texture_addressU  = cocogfx::CGLTrace::eTexAddress(rasterid.Textures[0].AddressU);
  drawcall.states.texture_addressV  = cocogfx::CGLTrace::eTexAddress(rasterid.Textures[0].AddressV);

  drawcall.states.blend_enabled = rasterid.Flags.Blend;
  drawcall.states.blend_src = cocogfx::CGLTrace::eBlendOp(rasterid.States.BlendSrc);
  drawcall.states.blend_dst = cocogfx::CGLTrace::eBlendOp(rasterid.States.BlendDst);

  drawcall.viewport.left = viewport.left;
  drawcall.viewport.right = viewport.right;
  drawcall.viewport.top = viewport.top;
  drawcall.viewport.bottom = viewport.bottom;
  drawcall.viewport.near = viewport.fNear;
  drawcall.viewport.far = viewport.fFar;

  if (rasterid.Flags.NumTextures != 0) {
    auto& surface0 = rasterData.Samplers[0].pMipLevels[0];

    CGLTrace::texture_t texture;
    texture.format = (ePixelFormat)surface0.getFormat();
    texture.width = surface0.getWidth();
    texture.height = surface0.getHeight();
    texture.pixels.resize(surface0.getPitch() * surface0.getHeight());
    memcpy(texture.pixels.data(), surface0.getBits(), texture.pixels.size());

    bool found = false;
    for (auto it : trace_->textures) {
      if (it.second == texture) {
        found = true;
        drawcall.texture_id = it.first;
      }
    }
    if (!found) {
      drawcall.texture_id = trace_->textures.size();
      trace_->textures.emplace(drawcall.texture_id, texture);
    }
  }
}

void FrameTrace::capture(uint32_t i0, uint32_t i1, uint32_t i2, uint8_t *pbVertexData[VERTEX_SIZE]) {
  auto& drawcall = trace_->drawcalls.back();
  ::capture(&drawcall, i0, pbVertexData);
  ::capture(&drawcall, i1, pbVertexData);
  ::capture(&drawcall, i2, pbVertexData);
  drawcall.primitives.push_back({i0, i1, i2});
}

void FrameTrace::endTrace() {
  //--
}

int FrameTrace::load(const char* filename) {
  return trace_->load(filename);
}

int FrameTrace::save(const char* filename) {
  if (!trace_->drawcalls.empty()
   && trace_->drawcalls.back().vertices.empty()) {
     trace_->drawcalls.pop_back();
  }
  return trace_->save(filename);
}