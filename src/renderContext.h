#pragma once
#include "pass.h"
#include <vector>
#include <camera.h>
#include <map>
#include "passList.h"

struct RenderContext
{
	Pass* currentPass;
	Pass* lastPass;
	PassList* passList;

	Camera* camera;
};