#include "vertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}

void VertexArray::bind()
{
	glBindVertexArray(id);
}