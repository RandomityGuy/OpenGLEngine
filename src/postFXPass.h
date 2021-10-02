#pragma once
#include "pass.h"
#include <vertexBuffer.h>
#include <vertexArray.h>

class PostFXPass : public Pass
{
public:
	PostFXPass();
	~PostFXPass();
	Texture2D* colorBuffer;
	VertexBuffer* quadVBO;
	VertexArray* quadVAO;

	void apply(RenderContext* context);
};