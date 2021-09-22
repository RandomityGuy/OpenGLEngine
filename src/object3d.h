#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"

class Scene;

class Object3D
{
	glm::mat4 transform;
	glm::mat4 absTransform;
	bool _syncTransform = true;

	void _sync();
public:
	Object3D* parent;
	Scene* scene;
	std::vector<Object3D*> children;

	Object3D();
	~Object3D();
	void setTransform(glm::mat4& transform);
	glm::mat4 getTransform();
	glm::mat4 getAbsoluteTransform();
	virtual void addChild(Object3D* model);
	virtual void removeChild(Object3D* model);
	virtual void render(Shader* shader);
};