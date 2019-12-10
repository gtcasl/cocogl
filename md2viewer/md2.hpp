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

#include <vector> 

#define MD2_ID (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')

#define MD2_VERSION 8

#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			  32

namespace md2 {

struct md2_header_t {
  int id;      // magic number. must be equal to "IPD2"
  int version; // md2 version. must be equal to 8

  int skin_width;   // width of the texture
  int skin_height;  // height of the texture
  int framesize;    // size of one frame in bytes

  int num_skins;  // number of textures
  int num_verts;  // number of vertices
  int num_st;     // number of texture coordinates
  int num_tris;   // number of triangles
  int num_glcmds; // number of opengl commands
  int num_frames; // total number of frames

  int ofs_skins;  // offset to skin names (64 bytes each)
  int ofs_st;     // offset to s-t texture coordinates
  int ofs_tris;   // offset to triangles
  int ofs_frames; // offset to frame data
  int ofs_glcmds; // offset to opengl commands
  int ofs_end;    // offset to the end of file
};

struct md2_st_t { 
  uint16_t s;
  uint16_t t; 
};

struct md2_triangle_t {
  uint16_t vidx[3]; // indexes to triangle's vertices
  uint16_t tidx[3]; // indexes to vertices' texture coorinates
} ;

struct md2_vertex_t {
  uint8_t pos[3];   // compressed vertex' (x, y, z) coordinates
  uint8_t nornal;   // index to a normal vector for the lighting
};

struct md2_frame_t {
  float scale[3];         // scale values
  float translate[3];     // translation vector
  char name[16];          // frame name
  md2_vertex_t verts[1];  // first vertex of this frame
};

struct md2_anim_t {
  int first_frame; // first frame of the animation
  int last_frame;  // number of frames
  int fps;         // number of frames per second
};

enum class md2_animtype {
  STAND,
  RUN,
  ATTACK,
  PAIN_A,
  PAIN_B,
  PAIN_C,
  JUMP,
  FLIP,
  SALUTE,
  FALLBACK,
  WAVE,
  POINT,
  CROUCH_STAND,
  CROUCH_WALK,
  CROUCH_ATTACK,
  CROUCH_PAIN,
  CROUCH_DEATH,
  DEATH_FALLBACK,
  DEATH_FALLFORWARD,
  DEATH_FALLBACKSLOW,
  BOOM,
  MAX_ANIMATIONS
};

///////////////////////////////////////////////////////////////////////////////

class MD2Model {
public:
  MD2Model();
  ~MD2Model();

  bool load(const char *filename);

  void destroy();

  void animate(int animKey);

  void render();

  const char* getAnimation(int animKey) const;
	
	int getNumAnimKeys() const;

private:

float getTime();

  struct triangle_t {
    uint16_t v[3];
  };

  struct vectex_t {
    vec3_t pos;
    vec3_t normal;
  };

  using frame_t = std::vector<vectex_t>;

  struct keyframe_t {
    char name[16];
    int start;
		int end;		
  };

  std::vector<frame_t> frames_;
  std::vector<keyframe_t> keyframes_;

  std::vector<triangle_t> triangles_;
  std::vector<vec3_t> positions_;
  std::vector<vec3_t> normals_;
  std::vector<vec2_t> texcoords_;

  int curAnimKey_;
  int curFrame_;
  int nextFrame_;
  
  GLuint texture_;

  std::chrono::system_clock::time_point lastTime_;
};

}