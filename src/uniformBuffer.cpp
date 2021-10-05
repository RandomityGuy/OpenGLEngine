#include "uniformBuffer.h"

UniformBuffer::UniformBuffer()
{
	glGenBuffers(1, &this->id);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void UniformBuffer::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
}

void UniformBuffer::allocate(int size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
