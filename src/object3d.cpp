#include "object3d.h"
#include <glm/vec4.hpp>
#include "scene.h"
#include "pass.h"

Object3D::Object3D()
{
	this->transform = glm::mat4(1);
	this->absTransform = glm::mat4(1);
	this->inverseTransform = glm::mat4(1);
	this->inverseTransposeTransform = glm::mat4(1);
	this->parent = NULL;
}

Object3D::~Object3D()
{
}

void Object3D::setTransform(glm::mat4& transform)
{
	this->transform = transform;
	this->_sync();
}

glm::mat4 Object3D::getTransform()
{
	return this->transform;
}

glm::mat4 Object3D::getAbsoluteTransform()
{
	if (this->_syncTransform)
	{
		glm::mat4 parentTransform = this->parent != NULL ? this->parent->getAbsoluteTransform() : glm::mat4(1);
		this->absTransform = parentTransform * this->transform;
		this->inverseTransform = glm::inverse(this->absTransform);
		this->inverseTransposeTransform = glm::transpose(this->inverseTransform);
		this->_syncTransform = false;
	}
	return this->absTransform;
}

void Object3D::addChild(Object3D* model)
{
	if (model->parent == NULL)
	{
		this->children.push_back(model);
		model->parent = this;
		model->scene = this->scene;
		model->_sync();
	}
	else
	{
		throw std::exception("Object3D already has parent!");
	}
}

void Object3D::removeChild(Object3D* model)
{
	if (model->parent == this)
	{
		auto& it = std::find(this->children.begin(), this->children.end(), model);
		if (it != this->children.end())
		{
			this->children.erase(it);
			model->parent = NULL;
			model->_sync();
		}
	}
}

void Object3D::_sync()
{
	this->scene = this->parent != NULL ? this->parent->scene : NULL;
	this->_syncTransform = true;
	for (auto& child : children)
	{
		child->_sync();
	}
}

void Object3D::render(RenderContext* context)
{
	context->currentPass->shader->setUniform("model", this->getAbsoluteTransform());
}

void Object3D::prepareRender(RenderState* state)
{
	RenderNode rnode = RenderNode(0, this);
	state->renderqueue.push(rnode);
	for (auto& child : children)
	{
		child->prepareRender(state);
	}
}