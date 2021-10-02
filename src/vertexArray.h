#pragma once
#include <glad/glad.h>
#include <vector>

class VertexArray
{
	GLuint id;

public:
	VertexArray();
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	VertexArray(VertexArray&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	VertexArray& operator=(VertexArray&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->~VertexArray();
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

	void bind();
};