#pragma once
#include <string>
#include "shader.h"
#include "frameBuffer.h"

struct RenderContext;

class Pass
{
public:
	std::string name;
	FrameBuffer* frameBuffer;
	Shader* shader;

	Pass();
	~Pass();

	virtual void apply(RenderContext* context);
	virtual void postApply(RenderContext* context);
};