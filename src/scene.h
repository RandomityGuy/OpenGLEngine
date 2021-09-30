#pragma once
#include "object3d.h"
#include <queue>
#include "camera.h"
#include "window.h"


struct RenderNode
{
	int priority;
	Object3D* obj;

	RenderNode(int priority, Object3D* obj) : priority(priority), obj(obj)
	{

	}
};

struct CompareRenderOrder {
	bool operator()(RenderNode const& p1, RenderNode const& p2)
	{
		return p1.priority < p2.priority;
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

	virtual void prepareRender(RenderState* state);
	virtual void addChild(Object3D* model);
	virtual void removeChild(Object3D* model);
	virtual void render(Shader* state);
};