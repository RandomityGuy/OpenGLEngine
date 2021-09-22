#pragma once
#include "object3d.h"

class Model : public Object3D
{
public:
	Mesh* mesh;
	virtual void render(Shader* shader);
};