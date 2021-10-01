#pragma once
#include "pass.h"

class DefaultPass : public Pass
{
public:
	DefaultPass();
	~DefaultPass();
	Texture2D* colorBuffer;
	RenderBuffer* renderBuffer;

	void apply(RenderContext* context);
};