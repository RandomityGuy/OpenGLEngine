#include "model.h"
#include "scene.h"

void Model::render(Shader* shader)
{
	shader->setUniform("model", this->getAbsoluteTransform());
	if (this->mesh != NULL)
		this->mesh->render(shader);
}

void Model::prepareRender(RenderState* state)
{
	int renderOrder = RENDER_ORDER_OPAQUE;
	bool isTransparent = false;
	for (auto& mat : this->mesh->materials)
	{
		if (mat.alpha != 1)
		{
			isTransparent = true;
			break;
		}
	}
	if (isTransparent)
	{
		glm::vec3 pos = glm::vec3(this->getAbsoluteTransform()[3]);
		glm::vec3 dir = pos - this->scene->camera->position;
		renderOrder = -(glm::dot(dir, dir));
	}
	RenderNode rnode = RenderNode(renderOrder, this);
	state->renderqueue.push(rnode);
	for (auto& child : children)
	{
		child->prepareRender(state);
	}
}
