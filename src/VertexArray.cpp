#include "VertexArray.h"

VertexArray::VertexArray(): ID(0) {

	glGenVertexArrays(1, &ID);
	glCheckError();
	glBindVertexArray(ID);
	glCheckError();

}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &ID);
	glCheckError();
}

void VertexArray::Bind() const {
	glBindVertexArray(ID);
	glCheckError();

}
void VertexArray::UnBind() const {
	glBindVertexArray(0);
	glCheckError();

}
void VertexArray::AddVertexArrayAttributef(const VertexBuffer& vbo, const VertexArrayAttribute& attributes) {
	Bind();
	vbo.Bind();
	const std::vector<unsigned int>& sizeArray = attributes.GetElements();
	unsigned int offSet = 0;
	for (unsigned int i = 0; i < sizeArray.size(); i++) {
		glVertexAttribPointer(i, sizeArray[i], GL_FLOAT, GL_FALSE, attributes.GetStride() * sizeof(float), (void*)(offSet * sizeof(float)));
		glCheckError();
		glEnableVertexAttribArray(i);
		glCheckError();
		offSet += sizeArray[i];
	}
	UnBind();
	vbo.UnBind();
	

}

void VertexArray::AddVertexArrayAttributef(const VertexBuffer& vbo, const IndexBuffer& ibo, const VertexArrayAttribute& attributes) {
	Bind();
	vbo.Bind();
	ibo.Bind();
	const std::vector<unsigned int>& sizeArray = attributes.GetElements();
	unsigned int offSet = 0;
	

	for (unsigned int i = 0; i < sizeArray.size(); i++) {
		glEnableVertexAttribArray(i);
		glCheckError();
		glVertexAttribPointer(i, sizeArray[i], GL_FLOAT, GL_FALSE, attributes.GetStride() * sizeof(float), (void*)(offSet * sizeof(float)));
		glCheckError();
		offSet += sizeArray[i];
	}
	UnBind();
	vbo.UnBind();
	ibo.UnBind();
}
