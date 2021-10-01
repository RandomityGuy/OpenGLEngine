#include "renderBuffer.h"

RenderBuffer::RenderBuffer(GLenum format, int width, int height) : width(width), height(height)
{
	glGenRenderbuffers(1, &this->id);
	glBindRenderbuffer(GL_RENDERBUFFER, this->id);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &this->id);
}

