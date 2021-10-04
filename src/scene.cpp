#include "scene.h"
#include "camera.h"


Scene::Scene(Window* window)
{
	this->scene = this;
	this->camera = new Camera(window, glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
	this->camera->mode = Camera::Normal;

	this->camera->position = glm::vec3(0, 0, 0);

	std::vector<std::string> faces
	{
		"data/skybox/right.jpg",
		"data/skybox/left.jpg",
		"data/skybox/front.jpg",
		"data/skybox/back.jpg",
		"data/skybox/top.jpg",
		"data/skybox/bottom.jpg"
	};


	this->skybox = new Skybox();
	skybox->cubemap = new CubeMap();
	skybox->cubemap->load(faces);
	addChild(*this->skybox);
}

Scene::~Scene()
{
	delete this->camera;
	delete this->skybox->cubemap;
	delete this->skybox;
}

void Scene::addChild(Object3D& model)
{
	model.scene = this;
	Object3D::addChild(model);
}

void Scene::removeChild(Object3D& model)
{
	model.scene = NULL;
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

}
