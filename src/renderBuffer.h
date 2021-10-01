#pragma once
#include <glad/glad.h>

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

};