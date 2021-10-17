#include "scene.h"
#include "pass.h"
#include <vector>
#include <map>

class Renderer
{
	Window* window;

public:
	Renderer(Window* window);
	~Renderer();

	std::map<std::string, Pass*> passes;
	std::map<std::string, Pass*> postFXPasses;
	Scene* scene;

	void render();
	void update(float dt);
};