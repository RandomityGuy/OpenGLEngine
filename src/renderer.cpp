#include "renderer.h"
#include "defaultPass.h"
#include "postFXPass.h"

Renderer::Renderer(Window* window) : window(window)
{
	this->scene = new Scene(window);
	DefaultPass* defpass = new DefaultPass();
	this->passes.insert(std::make_pair(defpass->name,defpass));

	PostFXPass* postFX = new PostFXPass();
	this->postFXPasses.insert(std::make_pair(postFX->name, postFX));
}

Renderer::~Renderer()
{
	for (auto& pass : passes)
		delete pass.second;

	for (auto& pass : postFXPasses)
		delete pass.second;

	delete scene;
}

void Renderer::render()
{
	sceneRenderState.renderqueue = std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder>();

	this->scene->prepareRender(&sceneRenderState);

	this->scene->sceneDirLights = 0;
	this->scene->scenePointLights = 0;
	this->scene->sceneSpotLights = 0;

	for (auto& pass : this->passes)
	{
		std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder> currentQueue = sceneRenderState.renderqueue;

		RenderContext context;
		context.currentPass = pass.second;
		context.passList = this->passes;
		context.camera = this->scene->camera;

		pass.second->apply(&context);

		this->scene->camera->render(&context);

		while (!currentQueue.empty())
		{
			RenderNode next = currentQueue.top();
			next.obj->render(&context);
			currentQueue.pop();
		}
	}

	for (auto& pass : this->postFXPasses)
	{
		std::priority_queue<RenderNode, std::vector<RenderNode>, CompareRenderOrder> currentQueue = sceneRenderState.renderqueue;

		RenderContext context;
		context.currentPass = pass.second;
		context.passList = this->passes;
		context.camera = this->scene->camera;

		pass.second->apply(&context);
	}

	GLint currentFBO = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
	glBlitNamedFramebuffer(currentFBO, 0, 0, 0, this->window->width, this->window->height, 0, 0, this->window->width, this->window->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
