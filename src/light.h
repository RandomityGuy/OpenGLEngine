#pragma once
#include "object3d.h"
#include <glm/glm.hpp>

class DirLight : public Object3D
{
public:
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;

	virtual void render(Shader* shader);
	virtual void prepareRender(RenderState* state);
};

class PointLight : public Object3D
{
public:
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;

	virtual void render(Shader* shader);
	virtual void prepareRender(RenderState* state);
};

class SpotLight : public Object3D
{
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	virtual void render(Shader* shader);
	virtual void prepareRender(RenderState* state);
};