#pragma once
#include <glad/glad.h>
#include <vector>

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
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	VertexBuffer(VertexBuffer&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	VertexBuffer& operator=(VertexBuffer&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->~VertexBuffer();
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

	void bind();
	void uploadData(GLsizeiptr size, const void* data, BufferDrawType type);
};