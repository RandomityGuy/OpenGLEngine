#include "refractModel.h"
#include "scene.h"

void RefractModel::render(RenderContext* context)
{
	if (context->currentPass->name != "refract")
		return;
	context->currentPass->shader->setUniform("model", this->getAbsoluteTransform());
	context->currentPass->shader->setUniform("modelInverseTranspose", this->inverseTransposeTransform);
	if (this->mesh != NULL)
		this->mesh->render(context);
}

void RefractModel::prepareRender(RenderState* state)
{
	float renderOrder = RENDER_ORDER_OPAQUE;
	bool isTransparent = true;
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

void RefractModel::setMesh(RefractMesh& mesh)
{
	this->mesh = &mesh;
}
