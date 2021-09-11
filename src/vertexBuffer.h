#pragma once
#include <glad/glad.h>
class VertexBuffer
{
	GLuint id;

public:
	enum BufferDrawType
	{
		Stream = GL_STREAM_DRAW,
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW
	};

	VertexBuffer();
	~VertexBuffer();

	void bind();
	void uploadData(GLsizeiptr size, const void* data, BufferDrawType type);
};