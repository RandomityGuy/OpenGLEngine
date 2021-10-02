#pragma once
#include <glad/glad.h>
#include <vector>

class FrameBuffer;

class RenderBuffer
{
	friend class FrameBuffer;
	GLuint id;

public:
	int width;
	int height;

	RenderBuffer(GLenum format, int width, int height);
	~RenderBuffer();
	RenderBuffer(const RenderBuffer&) = delete;
	RenderBuffer& operator=(const RenderBuffer&) = delete;

	RenderBuffer(RenderBuffer&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	RenderBuffer& operator=(RenderBuffer&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->~RenderBuffer();
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

};