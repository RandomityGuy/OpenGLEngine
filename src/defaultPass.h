#pragma once
#include "pass.h"

class DefaultPass : public Pass
{
public:
	DefaultPass();
	~DefaultPass();
	Texture2D* colorBuffer;
	RenderBuffer* renderBuffer;

	virtual void apply(RenderContext* context);
};