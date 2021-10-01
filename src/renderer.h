#include "scene.h"
#include "pass.h"
#include <vector>

class Renderer
{
	Window* window;

public:
	Renderer(Window* window);
	~Renderer();

	std::vector<Pass*> passes;
	Scene* scene;

	void render();
};