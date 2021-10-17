#include "vertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::uploadData(GLsizeiptr size, const void* data, BufferDrawType type)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}

void VertexBuffer::uploadData(GLsizeiptr size, const void* data, int offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void* VertexBuffer::getData(BufferIOType type)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	return glMapBuffer(GL_ARRAY_BUFFER, type);
}

void VertexBuffer::unmapData()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}