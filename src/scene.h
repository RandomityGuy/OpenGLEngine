#pragma once
#include "object3d.h"


class Scene : public Object3D
{

public:
	Scene();
	~Scene();

	const int maxDirLights = 1;
	const int maxPointLights = 4;
	const int maxSpotLights = 1;

	int sceneDirLights = 0;
	int scenePointLights = 0;
	int sceneSpotLights = 0;

	virtual void addChild(Object3D* model);
	virtual void removeChild(Object3D* model);
	virtual void render(Shader* shader);
};