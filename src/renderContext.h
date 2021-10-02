#pragma once
#include "pass.h"
#include <vector>
#include <camera.h>
#include <map>

struct RenderContext
{
	Pass* currentPass;
	std::map<std::string,Pass*> passList;

	Camera* camera;
};