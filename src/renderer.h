#pragma once
#include "scene.h"
#include "pass.h"
#include <vector>
#include <map>
#include "passList.h"

class Renderer
{
	Window* window;

public:
	Renderer(Window* window);
	~Renderer();

	PassList* passes;
	PassList* postFXPasses;
	Scene* scene;

	void render();
	void update(float dt);
};