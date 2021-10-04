#pragma once
#include "object3d.h"
#include <queue>
#include "camera.h"
#include "window.h"
#include <skybox.h>


struct RenderNode
{
	float priority;
	Object3D* obj;

	RenderNode(float priority, Object3D* obj) : priority(priority), obj(obj)
	{

	}
};

struct CompareRenderOrder {
	bool operator()(RenderNode const& p1, RenderNode const& p2)
	{
		// Render the positive things according to normal priority (bigger number goes first)
		// Then the negative numbers are for transparency ordering, where more negative number should be drawn first (since its far)
		return p1.priority >= 0 && p2.priority >= 0 ? p1.priority < p2.priority : (p1.priority < 0 && p2.priority < 0) ? p1.priority > p2.priority : p1.priority < p2.priority;
	}
};

struct RenderState
{
	Shader* shader;
	std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder> renderqueue;
};

static RenderState sceneRenderState;

class Scene : public Object3D
{

public:
	Scene(Window* window);
	~Scene();

	const int maxDirLights = 1;
	const int maxPointLights = 4;
	const int maxSpotLights = 1;

	int sceneDirLights = 0;
	int scenePointLights = 0;
	int sceneSpotLights = 0;

	Camera* camera;
	Skybox* skybox;

	virtual void prepareRender(RenderState* state);
	virtual void addChild(Object3D* model);
	virtual void removeChild(Object3D* model);
	virtual void render(Shader* state);
};