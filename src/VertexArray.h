#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include <glm.hpp>

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;

};


class VertexArrayAttribute {
private:
	unsigned int stride;
	std::vector<unsigned int> sizeArray;
	


public:
	
	VertexArrayAttribute() : stride(0) {};
	void PushAttributef(unsigned int size) {
		stride += size;
		sizeArray.push_back(size);
	}

	inline const std::vector<unsigned int> GetElements() const& { return sizeArray; }
	inline unsigned int GetStride() const { return stride; }
};
class VertexArray
{
private:
	unsigned int ID;



public:
	VertexArray();
	~VertexArray();
	void AddVertexArrayAttributef(const VertexBuffer& vbo, const VertexArrayAttribute& attributes);
	void AddVertexArrayAttributef(const VertexBuffer& vbo, const IndexBuffer& ebo, const VertexArrayAttribute& attributes);
	void Bind() const;
	void UnBind() const;

};

