#pragma once
#include "object3d.h"
#include "refractMesh.h"

class RefractModel : public Object3D
{
public:
	RefractMesh* mesh;
	virtual void render(RenderContext* context);
	virtual void prepareRender(RenderState* state);
	void setMesh(RefractMesh& mesh);
};