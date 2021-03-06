#include "gammaCorrectionPass.h"
#include <GL.h>
#include "renderContext.h"
#include "defaultPass.h"

GammaCorrectionPass::GammaCorrectionPass()
{
	this->name = "gammaCorrection";
	this->shader = new Shader("shaders/gammaCorrectionPostFX.vert", "shaders/gammaCorrectionPostFX.frag");
	shader->compile();

	this->colorBuffer = new Texture2D();
	this->colorBuffer->create(GL_RGBA, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE);
	this->colorBuffer->setFiltering(Texture2D::Linear, Texture2D::Linear);
	this->colorBuffer->generateMipMaps();

	this->frameBuffer = new FrameBuffer();
	this->frameBuffer->attachColor(*this->colorBuffer, 0);

	this->frameBuffer->test();

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	this->quadVAO = new VertexArray();
	this->quadVBO = new VertexBuffer();
	this->quadVAO->bind();
	this->quadVBO->uploadData(sizeof(quadVertices), quadVertices, VertexBuffer::Static);
	GL::enableVertexAttribArray(0);
	GL::vertexAttribPointer(0, 2, GL_FLOAT, 4 * sizeof(float), 0, false);
	GL::enableVertexAttribArray(1);
	GL::vertexAttribPointer(1, 2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float), false);
	GL::bindVertexArray(0);
}

GammaCorrectionPass::~GammaCorrectionPass()
{
	delete this->frameBuffer;
	delete this->colorBuffer;
	delete this->quadVAO;
	delete this->quadVBO;
}

void GammaCorrectionPass::apply(RenderContext* context)
{
	this->frameBuffer->activate();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GL::setDepthTest(false);
	this->shader->activate();
	this->shader->setUniform("gamma", this->gamma);
	this->quadVAO->bind();

	// postFX pass
	PostFXPass* defpass = (PostFXPass*)context->lastPass;
	this->shader->activateTexture(*defpass->colorBuffer, "screenTexture", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
