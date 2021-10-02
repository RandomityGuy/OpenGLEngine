#pragma once
#include <glad/glad.h>
#include <texture2d.h>
#include <renderBuffer.h>

class FrameBuffer
{
	GLuint id;
public:

	FrameBuffer();
	~FrameBuffer();

	enum AttachmentType
	{
		Depth = GL_DEPTH_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	void activate();
	void attachColor(Texture2D& tex, int index);
	void attach(AttachmentType type, Texture2D& tex);
	void attach(AttachmentType type, RenderBuffer& buffer);
	void test();
};