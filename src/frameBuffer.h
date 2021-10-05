#pragma once
#include <glad/glad.h>
#include <texture2d.h>
#include <renderBuffer.h>
#include <vector>

class Pass;
class DefaultPass;
class PostFXPass;

class FrameBuffer
{
	friend class Pass;
	friend class DefaultPass;
	friend class PostFXPass;
	GLuint id;
public:

	FrameBuffer();
	~FrameBuffer();
	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	FrameBuffer(FrameBuffer&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	FrameBuffer& operator=(FrameBuffer&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			glDeleteFramebuffers(1, &this->id);
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

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