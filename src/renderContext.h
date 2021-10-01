#pragma once
#include "pass.h"
#include <vector>
#include <camera.h>

struct RenderContext
{
	Pass* currentPass;
	std::vector<Pass*> passList;

	Camera* camera;
};