#include "refractPass.h"
#include "GL.h"
#include "renderContext.h"

RefractPass::RefractPass()
{
	this->name = "refract";
	this->shader = new Shader("shaders/refract.vert", "shaders/refract.frag");
	shader->compile();

	this->colorBuffer = new Texture2D();
	this->colorBuffer->create(GL_RGBA, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE);
	this->colorBuffer->setFiltering(Texture2D::Linear, Texture2D::Linear);
	this->colorBuffer->generateMipMaps();

	this->renderBuffer = new RenderBuffer(GL_DEPTH_STENCIL, 800, 600);
	this->frameBuffer = new FrameBuffer();
	this->frameBuffer->attachColor(*this->colorBuffer, 0);
	this->frameBuffer->attach(FrameBuffer::DepthStencil, *this->renderBuffer);

	this->frameBuffer->test();
}

RefractPass::~RefractPass()
{
	delete this->shader;
	delete this->colorBuffer;
	delete this->renderBuffer;
	delete this->frameBuffer;
}

void RefractPass::apply(RenderContext* context)
{
	this->frameBuffer->activate();

	GL::setDepthTest(true);
	GL::setBlending(true);
	GL::blendFunc(GL::BlendMode::SrcAlpha, GL::BlendMode::OneMinusSrcAlpha, GL::BlendMode::SrcAlpha, GL::BlendMode::OneMinusSrcAlpha);
	GL::setClearColor(0, 0, 0, 0);
	GL::clear((int)GL::ClearTarget::Color | (int)GL::ClearTarget::Depth | (int)GL::ClearTarget::Stencil);

	this->shader->activate();
}
