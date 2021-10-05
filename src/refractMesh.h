#pragma once
#include "mesh.h"


class RefractMesh : public Mesh
{
public:
	virtual void render(RenderContext* context);
};