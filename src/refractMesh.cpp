#include "refractMesh.h"
#include "shader.h"
#include "renderContext.h"
#include "GL.h"
#include <defaultPass.h>

void RefractMesh::render(RenderContext* context)
{
    this->generateBuffers();
    this->loadMaterialTextures();
    // Set the camera and the transform uniforms from the model class, not here

    GL::setDepthTest(false);
    DefaultPass* pass = (DefaultPass*)context->passList["fwd"];

    context->currentPass->shader->setUniform("viewPos", context->camera->position);
    context->currentPass->shader->setUniform("ior", 1.5f);
    context->currentPass->shader->setUniform("resolution", glm::vec2(800, 600));
    context->currentPass->shader->activateTexture(*pass->colorBuffer, "env", 0);
    
    for (auto& vgroup : vertexGroups)
    {
        vgroup.VAO->bind();
        glDrawElements(GL_TRIANGLES, vgroup.indices.size(), GL_UNSIGNED_INT, 0);
        GL::bindVertexArray(0);
    }
    GL::setDepthTest(true);

}
