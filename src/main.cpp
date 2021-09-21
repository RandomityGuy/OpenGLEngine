#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "window.h"
#include "shader.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "indexBuffer.h"
#include "GL.h"
#include "texture2d.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <camera.h>
#include "mesh.h"
#include "model.h"


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef OPENGL_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
#endif

    Window* window = new Window();

    window->activate();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    Shader* shader = new Shader("shaders/default.vert", "shaders/default.frag");
    shader->compile();

    Shader* lightingShader = new Shader("shaders/default.vert", "shaders/light.frag");
    lightingShader->compile();


    Model model;
    Mesh mesh;
    mesh.load("data/cube.obj");
    model.mesh = &mesh;

    model.setTransform(glm::mat4(0.05));


    GL::setDepthTest(true);

    Camera camera = Camera(window, glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
    camera.mode = Camera::Normal;

    camera.position = glm::vec3(0, 0, 0);

    double prevT = glfwGetTime();

    glm::vec3 lightPos(0, 0, 1);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 ambientColor = glm::vec3(0.5f, 0.5f, 0.5f);

    while (!window->shouldClose())
    {
        double t = glfwGetTime();
        double dt = t - prevT;
        prevT = t;

        window->processInput();

        camera.update(dt);

        GL::setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::clear(GL::ClearTarget::Color | GL::ClearTarget::Depth);

        // draw our first triangle
        shader->activate();

        shader->setUniform("view", camera.view);
        shader->setUniform("projection", camera.projection);
        // shader->setUniform("viewPos", camera.position);

        shader->setUniform("dirLight.direction", lightPos);
        shader->setUniform("dirLight.ambient", ambientColor);
        shader->setUniform("dirLight.diffuse", diffuseColor); // darken diffuse light a bit
        //shader->setUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));


        //shader->setUniform("pointLights[0].position", pointLightPositions[0]);
        //shader->setUniform("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        //shader->setUniform("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        //shader->setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader->setUniform("pointLights[0].constant", 1.0f);
        //shader->setUniform("pointLights[0].linear", 0.09);
        //shader->setUniform("pointLights[0].quadratic", 0.032);
        //// point light 2
        //shader->setUniform("pointLights[1].position", pointLightPositions[1]);
        //shader->setUniform("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        //shader->setUniform("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        //shader->setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader->setUniform("pointLights[1].constant", 1.0f);
        //shader->setUniform("pointLights[1].linear", 0.09);
        //shader->setUniform("pointLights[1].quadratic", 0.032);
        //// point light 3
        //shader->setUniform("pointLights[2].position", pointLightPositions[2]);
        //shader->setUniform("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        //shader->setUniform("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        //shader->setUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader->setUniform("pointLights[2].constant", 1.0f);
        //shader->setUniform("pointLights[2].linear", 0.09);
        //shader->setUniform("pointLights[2].quadratic", 0.032);
        //// point light 4
        //shader->setUniform("pointLights[3].position", pointLightPositions[3]);
        //shader->setUniform("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        //shader->setUniform("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        //shader->setUniform("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader->setUniform("pointLights[3].constant", 1.0f);
        //shader->setUniform("pointLights[3].linear", 0.09);
        //shader->setUniform("pointLights[3].quadratic", 0.032);
        //// spotLight
        //shader->setUniform("spotLight.position", camera.position);
        //shader->setUniform("spotLight.direction", camera.direction);
        //shader->setUniform("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        //shader->setUniform("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader->setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader->setUniform("spotLight.constant", 1.0f);
        //shader->setUniform("spotLight.linear", 0.09);
        //shader->setUniform("spotLight.quadratic", 0.032);
        //shader->setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        //shader->setUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        model.render(shader);

        window->swapBuffers();
        glfwPollEvents();
    }

    delete shader;

    glfwTerminate();

    return 0;
}
