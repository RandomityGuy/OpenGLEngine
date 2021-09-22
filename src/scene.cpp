#include "scene.h"


Scene::Scene()
{
	this->scene = this;
}

Scene::~Scene()
{
}

void Scene::addChild(Object3D* model)
{
	model->scene = this;
	Object3D::addChild(model);
}

void Scene::removeChild(Object3D* model)
{
	model->scene = NULL;
	Object3D::removeChild(model);
}

void Scene::render(Shader* shader)
{
	sceneDirLights = 0;
	scenePointLights = 0;
	sceneSpotLights = 0;
	shader->setUniform("dirLight.enabled", false);
	shader->setUniform("pointLights[0].enabled", false);
	shader->setUniform("pointLights[1].enabled", false);
	shader->setUniform("pointLights[2].enabled", false);
	shader->setUniform("pointLights[3].enabled", false);
	shader->setUniform("spotLight.enabled", false);
	Object3D::render(shader);
}
