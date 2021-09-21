#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"

class Model
{
	glm::mat4 transform;
	glm::mat4 absTransform;
	bool _syncTransform = true;

	Model* parent;

	void _sync();
public:
	std::vector<Model*> children;
	Mesh* mesh;

	Model();
	~Model();
	void setTransform(glm::mat4& transform);
	glm::mat4 getTransform();
	glm::mat4 getAbsoluteTransform();
	void addChild(Model* model);
	void removeChild(Model* model);
	void render(Shader* shader);
};