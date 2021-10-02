#pragma once
#include "object3d.h"
#include "cubeMap.h"

class Skybox : public Object3D
{

	VertexBuffer* vbo;
	VertexArray* vao;

public:
	CubeMap* cubemap;
	Shader* shader;

	Skybox();
	~Skybox();

	virtual void render(RenderContext* context);
	virtual void prepareRender(RenderState* state);
};