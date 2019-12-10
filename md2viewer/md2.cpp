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
#include <fstream>
#include "md2.hpp"

using namespace md2;

static const float sc_vNormals[][3] = {
	#include "anorms.h"
};

MD2Model::MD2Model() {
	lastTime_ = std::chrono::system_clock::now();
	curAnimKey_ = 0;
	curFrame_ = 0;
	nextFrame_ = 0;
}

MD2Model::~MD2Model() {}

void MD2Model::destroy() {
	glDeleteTextures(1, &texture_);
}

bool MD2Model::load(const char *filename) {
  std::ifstream file;
  
  // open filename
  file.open(filename, std::ios::in | std::ios::binary);
  if (file.fail()) {
    std::cerr << "coudln't open file: " << filename << "!" << std::endl;
	  return false;
  }

	// read header file
  md2_header_t header;
	file.read(reinterpret_cast<char*>(&header), sizeof(md2_header_t));

	// verify MD2 file version
	if ((header.id != MD2_ID) && (header.version != MD2_VERSION))	{
    std::cerr << "invalid MD2 file format!" << std::endl;
    file.close();
    return false;
	}

	// read texcoords
	std::vector<md2_st_t> texcoords(header.num_st);
	file.seekg(header.ofs_st, std::ios::beg);
	file.read(reinterpret_cast<char*>(texcoords.data()), texcoords.size() * sizeof(md2_st_t));

	// read triangles data
	std::vector<md2_triangle_t> triangles(header.num_tris);
	file.seekg(header.ofs_tris, std::ios::beg);
	file.read(reinterpret_cast<char*>(triangles.data()), triangles.size() * sizeof(md2_triangle_t));

	// read frames data
	std::vector<char> frameData(header.num_frames * header.framesize);
	file.seekg(header.ofs_frames, std::ios::beg);
	file.read(frameData.data(), frameData.size());

	file.close();

	frames_.resize(header.num_frames);
	for (int f = 0; f < header.num_frames; ++f) {		
		auto& sframe = *reinterpret_cast<md2_frame_t*>(frameData.data() + f * header.framesize);
		auto& dframe = frames_[f];

		// copy vertices
		dframe.resize(header.num_verts);
		for (int i = 0; i < header.num_verts; ++i) {
			auto& v = sframe.verts[i];
			dframe[i].pos.x = (v.pos[0] * sframe.scale[0]) + sframe.translate[0];
      dframe[i].pos.y = (v.pos[1] * sframe.scale[1]) + sframe.translate[1];
      dframe[i].pos.z = (v.pos[2] * sframe.scale[2]) + sframe.translate[2];

			dframe[i].normal.x = sc_vNormals[v.nornal][0];
			dframe[i].normal.y = sc_vNormals[v.nornal][1];
			dframe[i].normal.z = sc_vNormals[v.nornal][2];
		}
	}

	// allocate animation keyframes	
	{
		int numKeys = 0;
		char szName[16] = {0};				
		char szPrevName[16] = {0};

		for (int f = 0; f < header.num_frames; ++f) {
			auto& frame = *reinterpret_cast<md2_frame_t*>(frameData.data() + f * header.framesize);
			strncpy(szName, frame.name, 16);			
			for (char *c = szName; *c; ++c) {
				// parse name without last digits 
				if (!std::isalpha(*c)) {
					*c = '\0';
					if (strcmp(szPrevName, szName) != 0) {
						strncpy(szPrevName, szName, 16);
						++numKeys;
					}
					break;
				}
			}	
		}

		keyframes_.resize(numKeys);		
	}

	// set up animation keyframes
	if (keyframes_.size() > 0)	{
		char szName[16] = {0};

		for (int f = 0, k = 0; f < header.num_frames; ++f) {
			auto& frame = *reinterpret_cast<md2_frame_t*>(frameData.data() + f * header.framesize);
			strncpy(szName, frame.name, 16);
			for (char *c = szName; *c; ++c) {
				// parse name without last digits 
				if (!std::isalpha(*c)) {
					*c = '\0';
					if (f != 0 && strcmp(keyframes_[k].name, szName) != 0) {
						keyframes_[k].end = f - 1;
						++k;
						keyframes_[k].start = f;
						strncpy(keyframes_[k].name, szName, 16);
					} else {
						if (0 == f) {
							strncpy(keyframes_[k].name, szName, 16);
							keyframes_[k].start = 0;
						}
						keyframes_[k].end = f;
					}
					break;
				}
			}
		}
	}

	// load texture
	{
		std::string texfname(filename);
		texfname.replace(texfname.end() - 3, texfname.end(), "tga");
		std::vector<uint8_t> pixels;
		int width, height, bpp;
		if (!LoadTGA(texfname.c_str(), pixels, &width, &height, &bpp))
			return false;
		texture_ = loadTexture(pixels, width, height, bpp);
	}

  // process triangles  
	triangles_.resize(header.num_tris);
	for (int j = 0; j < header.num_tris; ++j) {
		for (int i = 0; i < 3; ++i) {			
			triangles_[j].v[i] = triangles[j].vidx[i];
		}		
	}
	
	positions_.resize(header.num_tris * 3);	
	normals_.resize(header.num_tris * 3);	
	texcoords_.resize(header.num_tris * 3);

	// process texcoords
	auto r_skin_width = 1.0f / header.skin_width;
	auto r_skin_height = 1.0f / header.skin_height;
	for (int j = 0; j < header.num_tris; ++j) {
		for (int i = 0; i < 3; ++i) {			
			auto tidx = triangles[j].tidx[i];
			texcoords_.at(3 * j + i).x = texcoords.at(tidx).s * r_skin_width;
			texcoords_.at(3 * j + i).y = 1.0 - texcoords.at(tidx).t * r_skin_height;
		}
	}
	
	return true;
}

void MD2Model::animate(int animKey) {
	auto& keyframe = keyframes_.at(animKey);

	auto time = this->getTime();
	if (0.0f == time)	{
		curFrame_ = nextFrame_++;

		if (curAnimKey_ != animKey) {
			nextFrame_ = keyframe.start;
			curAnimKey_ = animKey;
		}
	}

	if (curFrame_ == nextFrame_)
		++nextFrame_;

	if (nextFrame_ > keyframe.end)
		nextFrame_ = keyframe.start;

	auto curVerts = frames_[curFrame_].data();
	auto nextVerts = frames_[nextFrame_].data();
	
	for (uint32_t j = 0; j < triangles_.size(); ++j) {
		auto& triangle = triangles_[j];
		for (uint32_t i = 0; i < 3; ++i)	{
			int v = triangle.v[i];
			auto& curVertex = curVerts[v];
			auto& nextVertex = nextVerts[v];
			
			auto& position = positions_.at(3 * j + i);
			auto& normal = normals_.at(3 * j + i);

			position.x = lerp(curVertex.pos.x, nextVertex.pos.x, time);
			position.y = lerp(curVertex.pos.y, nextVertex.pos.y, time);
			position.z = lerp(curVertex.pos.z, nextVertex.pos.z, time);

			normal.x = lerp(curVertex.normal.x, nextVertex.normal.x, time);
			normal.y = lerp(curVertex.normal.y, nextVertex.normal.y, time);
			normal.z = lerp(curVertex.normal.z, nextVertex.normal.z, time);
		}
	}
}

void MD2Model::render() {
	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);	
	glBindTexture(GL_TEXTURE_2D, texture_);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, positions_.data());
	glNormalPointer(GL_FLOAT, 0, normals_.data());
  glTexCoordPointer(2, GL_FLOAT, 0, texcoords_.data());

  glDrawArrays(GL_TRIANGLES, 0, positions_.size());
}

const char* MD2Model::getAnimation(int animKey) const {
	return keyframes_.at(animKey).name;
}

int MD2Model::getNumAnimKeys() const {
	return keyframes_.size();
}

float MD2Model::getTime() {
	auto time = std::chrono::system_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(time - lastTime_).count();
	if (elapsedTime > 200) {
		lastTime_ = time;
		return 0.0f;
	}
	return elapsedTime * 0.005f;
}