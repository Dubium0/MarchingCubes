#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size) : ID(0) {
    glGenBuffers(1, &ID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    UnBind();
}
void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VertexBuffer::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &ID);
}