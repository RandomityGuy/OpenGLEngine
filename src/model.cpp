#include "model.h"
#include <glm/vec4.hpp>

Model::Model()
{
	this->mesh = NULL;
	this->transform = glm::mat4(1);
	this->absTransform = glm::mat4(1);
	this->parent = NULL;
}

Model::~Model()
{
}

void Model::setTransform(glm::mat4& transform)
{
	this->transform = transform;
	this->_sync();
}

glm::mat4 Model::getTransform()
{
	return this->transform;
}

glm::mat4 Model::getAbsoluteTransform()
{
	if (this->_syncTransform)
	{
		glm::mat4 parentTransform = this->parent != NULL ? this->parent->getAbsoluteTransform() : glm::mat4(1);
		this->absTransform = parentTransform * this->transform;
		this->_syncTransform = false;
	}
	return this->absTransform;
}

void Model::addChild(Model* model)
{
	if (model->parent == NULL)
	{
		this->children.push_back(model);
		model->parent = this;
		model->_sync();
	}
	else
	{
		throw std::exception("Model already has parent!");
	}
}

void Model::removeChild(Model* model)
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

void Model::_sync()
{
	this->_syncTransform = true;
	for (auto& child : children)
	{
		child->_sync();
	}
}

void Model::render(Shader* shader)
{
	shader->setUniform("model", this->getAbsoluteTransform());
	if (this->mesh != NULL)
		this->mesh->render(shader);
}