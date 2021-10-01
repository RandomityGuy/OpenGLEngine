#include "frameBuffer.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &this->id);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &this->id);
}

void FrameBuffer::activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void FrameBuffer::attachColor(Texture2D& tex, int index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, tex.id, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::attach(AttachmentType type, Texture2D& tex)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)type, GL_TEXTURE_2D, tex.id, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::attach(AttachmentType type, RenderBuffer& buffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)type, GL_RENDERBUFFER, buffer.id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}