#pragma once
#include "postFXPass.h"

class GammaCorrectionPass : public PostFXPass
{
public:
	GammaCorrectionPass();
	~GammaCorrectionPass();
	float gamma = 2.2;

	virtual void apply(RenderContext* context);
};