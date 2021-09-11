#include "indexBuffer.h"
#include <glad/glad.h>

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &id);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &id);
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::uploadData(GLsizeiptr size, const void* data, BufferDrawType type)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);
}