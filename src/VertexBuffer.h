#pragma once
#include "GL_Debugger.h"
class VertexBuffer
{
private:
	unsigned int ID;

public: 
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void UnBind() const;

};

