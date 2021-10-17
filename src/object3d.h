#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "renderContext.h"

// Render Order Macros
#define RENDER_ORDER_SKY -9999
#define RENDER_ORDER_LIGHTS 1
#define RENDER_ORDER_OPAQUE 0
// Transparent shit has negative order

class Scene;
struct RenderState;

class Object3D
{
	bool _syncTransform = true;

	void _sync();

protected:
	glm::mat4 transform;
	glm::mat4 absTransform;
	glm::mat4 inverseTransform;
	glm::mat4 inverseTransposeTransform;
public:
	Object3D* parent;
	Scene* scene;
	std::vector<Object3D*> children;

	Object3D();
	~Object3D();
	void setTransform(glm::mat4& transform);
	glm::mat4 getTransform();
	glm::mat4 getAbsoluteTransform();
	virtual void addChild(Object3D& model);
	virtual void removeChild(Object3D& model);
	virtual void render(RenderContext* context);
	virtual void prepareRender(RenderState* state);
	virtual void update(float dt);
};