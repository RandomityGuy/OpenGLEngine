#include "defaultPass.h"
#include "GL.h"
#include "renderContext.h"

DefaultPass::DefaultPass()
{
	this->name = "fwd";
	this->shader = new Shader("shaders/default.vert", "shaders/default.frag");
	shader->compile();

	this->colorBuffer = new Texture2D();
	this->colorBuffer->create(GL_RGBA, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE);

	this->renderBuffer = new RenderBuffer(GL_DEPTH_STENCIL, 800, 600);
	this->frameBuffer = new FrameBuffer();
	this->frameBuffer->attachColor(*this->colorBuffer, 0);
	this->frameBuffer->attach(FrameBuffer::DepthStencil, *this->renderBuffer);
}

DefaultPass::~DefaultPass()
{
	delete this->shader;
	delete this->colorBuffer;
	delete this->renderBuffer;
	delete this->frameBuffer;
}

void DefaultPass::apply(RenderContext* context)
{
	this->frameBuffer->activate();

	GL::setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GL::clear((int)GL::ClearTarget::Color | (int)GL::ClearTarget::Depth | (int) GL::ClearTarget::Stencil);

	this->shader->activate();

	this->shader->setUniform("view", context->camera->view);
	this->shader->setUniform("projection", context->camera->projection);
	this->shader->setUniform("viewPos", context->camera->position);

	this->shader->setUniform("dirLight.enabled", false);
	this->shader->setUniform("pointLights[0].enabled", false);
	this->shader->setUniform("pointLights[1].enabled", false);
	this->shader->setUniform("pointLights[2].enabled", false);
	this->shader->setUniform("pointLights[3].enabled", false);
	this->shader->setUniform("spotLight.enabled", false);
}
