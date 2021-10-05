#include "light.h"
#include "scene.h"
#include "pass.h"

void DirLight::render(RenderContext* context)
{
    if (context->currentPass->name != "fwd")
        return;
    Shader* shader = context->currentPass->shader;
    this->scene->sceneDirLights++;
    if (this->scene->maxDirLights < this->scene->sceneDirLights)
        throw new std::exception("DirLight count exceeded");

    shader->setUniform("dirLight.direction", this->direction);
    shader->setUniform("dirLight.ambient", this->ambient);
    shader->setUniform("dirLight.diffuse", this->diffuse);
    shader->setUniform("dirLight.specular", this->specular);
    shader->setUniform("dirLight.enabled", true);
}

void DirLight::prepareRender(RenderState* state)
{
    RenderNode rnode = RenderNode(RENDER_ORDER_LIGHTS, this);
    state->renderqueue.push(rnode);
}

void PointLight::render(RenderContext* context)
{
    if (context->currentPass->name != "fwd")
        return;
    Shader* shader = context->currentPass->shader;
    this->scene->scenePointLights++;
    if (this->scene->maxPointLights < this->scene->scenePointLights)
        throw new std::exception("PointLight count exceeded");

    std::string pointLightIndex = std::to_string(this->scene->scenePointLights - 1);

    std::string begin = std::string("pointLights[");

    glm::mat4 tform = this->getAbsoluteTransform();
    glm::vec3 position = glm::vec3(tform[3]);

    shader->setUniform(begin + pointLightIndex + "].position", position);
    shader->setUniform(begin + pointLightIndex + "].ambient", this->ambient);
    shader->setUniform(begin + pointLightIndex + "].diffuse", this->diffuse);
    shader->setUniform(begin + pointLightIndex + "].specular", this->specular);
    shader->setUniform(begin + pointLightIndex + "].constant", this->constant);
    shader->setUniform(begin + pointLightIndex + "].linear", this->linear);
    shader->setUniform(begin + pointLightIndex + "].quadratic", this->quadratic);
    shader->setUniform(begin + pointLightIndex + "].enabled", true);
}

void PointLight::prepareRender(RenderState* state)
{
    RenderNode rnode = RenderNode(RENDER_ORDER_LIGHTS, this);
    state->renderqueue.push(rnode);
}

void SpotLight::render(RenderContext* context)
{
    if (context->currentPass->name != "fwd")
        return;
    Shader* shader = context->currentPass->shader;
    this->scene->sceneSpotLights++;
    if (this->scene->maxSpotLights < this->scene->sceneSpotLights)
        throw new std::exception("SpotLight count exceeded");

    glm::mat4 tform = this->getAbsoluteTransform();
    glm::vec3 position = glm::vec3(tform[3]);

    shader->setUniform("spotLight.position", position);
    shader->setUniform("spotLight.direction", this->direction);
    shader->setUniform("spotLight.ambient", this->ambient);
    shader->setUniform("spotLight.diffuse", this->diffuse);
    shader->setUniform("spotLight.specular", this->specular);
    shader->setUniform("spotLight.constant", this->constant);
    shader->setUniform("spotLight.linear", this->linear);
    shader->setUniform("spotLight.quadratic", this->quadratic);
    shader->setUniform("spotLight.cutOff", this->cutOff);
    shader->setUniform("spotLight.outerCutOff", this->outerCutOff);
    shader->setUniform("spotLight.enabled", true);
}

void SpotLight::prepareRender(RenderState* state)
{
    RenderNode rnode = RenderNode(RENDER_ORDER_LIGHTS, this);
    state->renderqueue.push(rnode);
}

