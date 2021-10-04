#include "model.h"
#include "scene.h"

void Model::render(RenderContext* context)
{
	context->currentPass->shader->setUniform("model", this->getAbsoluteTransform());
	context->currentPass->shader->setUniform("modelInverseTranspose", this->inverseTransposeTransform);
	context->currentPass->shader->activateCubemap(*this->scene->skybox->cubemap, "skybox", 4);
	if (this->mesh != NULL)
		this->mesh->render(context);
}

void Model::prepareRender(RenderState* state)
{
	float renderOrder = RENDER_ORDER_OPAQUE;
	bool isTransparent = false;
	for (auto& mat : this->mesh->materials)
	{
		if (mat.alpha != 1 || mat.transparent)
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

void Model::setMesh(Mesh& mesh)
{
	this->mesh = &mesh;
}
