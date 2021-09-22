#include "model.h"

void Model::render(Shader* shader)
{
	shader->setUniform("model", this->getAbsoluteTransform());
	if (this->mesh != NULL)
		this->mesh->render(shader);

	Object3D::render(shader);
}