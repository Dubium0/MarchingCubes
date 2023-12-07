#pragma once
#include "GL_Debugger.h"
#include <iostream>
#include <string>

#include <stb_image.h>

class Texture {
public:
	Texture(const std::string& _path, GLenum target,GLint _wrapParam, GLint _filterMode);
	~Texture();
	void Bind(unsigned int channel) const ;
	void UnBind() const;
	inline unsigned int GetID () const { return id; }
	inline std::string GetPath() const { return path; }
	inline  int GetWIDTH() const { return WIDTH; }
	inline  int GetHEIGHT() const { return HEIGHT; }
	inline  int GetNR_CHANNELS() const { return NR_CHANNELS; }
	inline void SetTypeName(const std::string& type) { typeName = type; };
	inline std::string GetTypeName() const { return typeName; };
	inline void SetModelPath(const std::string& path) { modelPath = path; };
	inline std::string GetModelPath() const { return modelPath; };

private:
	unsigned int id;
	std::string typeName; // doesn't have spesify 
	std::string modelPath;
	std::string path;
	unsigned char* data;
	int WIDTH, HEIGHT, NR_CHANNELS;
};