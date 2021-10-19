#include "renderer.h"
#include "defaultPass.h"
#include "postFXPass.h"
#include "refractPass.h"
#include "gammaCorrectionPass.h"
#include "passList.h"

Renderer::Renderer(Window* window) : window(window)
{
	this->scene = new Scene(window);
	
	this->passes = new PassList();
	this->postFXPasses = new PassList();

	DefaultPass* defpass = new DefaultPass();
	this->passes->addPass(defpass);

	PostFXPass* postFX = new PostFXPass();
	this->postFXPasses->addPass(postFX);

	GammaCorrectionPass* gammaCorrection = new GammaCorrectionPass();
	this->postFXPasses->addPass(gammaCorrection);
}

Renderer::~Renderer()
{
	delete passes;
	delete postFXPasses;
	delete scene;
}

void Renderer::render()
{
	sceneRenderState.renderqueue = std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder>();

	this->scene->prepareRender(&sceneRenderState);

	this->scene->sceneDirLights = 0;
	this->scene->scenePointLights = 0;
	this->scene->sceneSpotLights = 0;

	Pass* lastPass = NULL;
	for (auto& pass : *this->passes)
	{
		std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder> currentQueue = sceneRenderState.renderqueue;

		RenderContext context;
		context.currentPass = pass.pass;
		context.passList = this->passes;
		context.camera = this->scene->camera;
		context.lastPass = lastPass;

		pass.pass->apply(&context);

		this->scene->camera->render(&context);

		while (!currentQueue.empty())
		{
			RenderNode next = currentQueue.top();
			next.obj->render(&context);
			currentQueue.pop();
		}

		pass.pass->postApply(&context);

		lastPass = pass.pass;
	}

	lastPass = NULL;

	for (auto& pass : *this->postFXPasses)
	{
		std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder> currentQueue = sceneRenderState.renderqueue;

		RenderContext context;
		context.currentPass = pass.pass;
		context.passList = this->passes;
		context.camera = this->scene->camera;
		context.lastPass = lastPass;

		pass.pass->apply(&context);
		pass.pass->postApply(&context);

		lastPass = pass.pass;
	}

	GLint currentFBO = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
	glBlitNamedFramebuffer(currentFBO, 0, 0, 0, this->window->width, this->window->height, 0, 0, this->window->width, this->window->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::update(float dt)
{
	this->scene->update(dt);
}
