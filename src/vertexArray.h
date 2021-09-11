#pragma once
#include <glad/glad.h>

class VertexArray
{
	GLuint id;

public:
	VertexArray();
	~VertexArray();

	void bind();
};