#pragma once
#include "defaultPass.h"

class RefractPass : public DefaultPass {
public:
	RefractPass();
	~RefractPass();

	virtual void apply(RenderContext* context);
};