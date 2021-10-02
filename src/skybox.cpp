#include "skybox.h"
#include <scene.h>
#include "GL.h"

Skybox::Skybox() : Object3D::Object3D()
{
	this->shader = new Shader("shaders/skybox.vert", "shaders/skybox.frag");
    this->shader->compile();

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    this->vao = new VertexArray();
    vao->bind();
    this->vbo = new VertexBuffer();
    this->vbo->uploadData(sizeof(skyboxVertices), skyboxVertices, VertexBuffer::Static);
    GL::enableVertexAttribArray(0);
    GL::vertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(float), 0, false);
    GL::bindVertexArray(0);
}

Skybox::~Skybox()
{
	Object3D::~Object3D();
	delete this->shader;
    delete this->vbo;
    delete this->vao;
}

void Skybox::render(RenderContext* context)
{
    GL::depthFunc(GL::CompareFunc::LessThanEquals);
    this->shader->activate();
    glm::mat4 view = glm::mat4(glm::mat3(context->camera->view));
    this->shader->setUniform("view", view);
    this->shader->setUniform("projection", context->camera->projection);
    this->vao->bind();
    this->shader->activateCubemap(*this->cubemap, "skybox", 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    GL::bindVertexArray(0);
    GL::depthFunc(GL::CompareFunc::LessThan);
    context->currentPass->shader->activate(); // Change the shader back
}

void Skybox::prepareRender(RenderState* state)
{
	RenderNode rnode = RenderNode(RENDER_ORDER_SKY, this);
	state->renderqueue.push(rnode);
}
