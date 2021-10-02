#pragma once
#include <glad/glad.h>
#include <vector>
class IndexBuffer
{
	GLuint id;

public:
	enum BufferDrawType
	{
		Stream = GL_STREAM_DRAW,
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW
	};

	IndexBuffer();
	~IndexBuffer();
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	IndexBuffer& operator=(IndexBuffer&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			glDeleteBuffers(1, &this->id);
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

	void bind();
	void uploadData(GLsizeiptr size, const void* data, BufferDrawType type);
};