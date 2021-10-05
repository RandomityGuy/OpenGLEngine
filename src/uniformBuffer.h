#pragma once
#include <glad/glad.h>
#include <vector>

class UniformBuffer
{
	GLuint id;

public:
	UniformBuffer();
	~UniformBuffer();

	void bind();
	void allocate(int size);
};