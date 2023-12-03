#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int size) : ID(0) {
    glGenBuffers(1, &ID);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glCheckError();
    //std::cout << size << std::endl;
}


void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glCheckError();
}
void IndexBuffer::UnBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
}
IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &ID);
    glCheckError();
}