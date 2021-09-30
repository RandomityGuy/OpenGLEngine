#include "scene.h"
#include "camera.h"


Scene::Scene(Window* window)
{
	this->scene = this;
	this->camera = new Camera(window, glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
	this->camera->mode = Camera::Normal;

	this->camera->position = glm::vec3(0, 0, 0);
}

Scene::~Scene()
{
	delete this->camera;
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

void Scene::prepareRender(RenderState* state)
{
	for (auto& child : children)
	{
		child->prepareRender(state);
	}
}

void Scene::render(Shader* shader)
{
	sceneRenderState.shader = shader;
	sceneRenderState.renderqueue = std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder>();

	this->prepareRender(&sceneRenderState);

	sceneDirLights = 0;
	scenePointLights = 0;
	sceneSpotLights = 0;

	shader->setUniform("view", camera->view);
	shader->setUniform("projection", camera->projection);
	shader->setUniform("viewPos", camera->position);

	shader->setUniform("dirLight.enabled", false);
	shader->setUniform("pointLights[0].enabled", false);
	shader->setUniform("pointLights[1].enabled", false);
	shader->setUniform("pointLights[2].enabled", false);
	shader->setUniform("pointLights[3].enabled", false);
	shader->setUniform("spotLight.enabled", false);

	while (!sceneRenderState.renderqueue.empty())
	{
		RenderNode next = sceneRenderState.renderqueue.top();
		next.obj->render(shader);
		sceneRenderState.renderqueue.pop();
	}
}
