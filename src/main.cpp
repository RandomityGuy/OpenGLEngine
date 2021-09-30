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
#include "object3d.h"
#include <examples/viewer/stb_image.h>
#include "model.h"
#include "scene.h"
#include "light.h"


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

    stbi_set_flip_vertically_on_load(true);

    Shader* shader = new Shader("shaders/default.vert", "shaders/default.frag");
    shader->compile();

    Shader* lightingShader = new Shader("shaders/default.vert", "shaders/light.frag");
    lightingShader->compile();


    Scene scene = Scene(window);

    glm::vec3 lightPos(1, 1, -1);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    
    DirLight dirLight;
    dirLight.direction = lightPos;
    dirLight.ambient = ambientColor;
    dirLight.diffuse = diffuseColor;
    dirLight.specular = glm::vec3(1, 1, 1);

    scene.addChild(&dirLight);


    Model model;
    Mesh mesh;
    mesh.load("data/perseverance/perseverance.obj");
    model.mesh = &mesh;

    glm::mat4 tform = glm::mat4(1);
   
    model.setTransform(tform);

    scene.addChild(&model);

    GL::setDepthTest(true);

    double prevT = glfwGetTime();

    while (!window->shouldClose())
    {
        double t = glfwGetTime();
        double dt = t - prevT;
        prevT = t;

        window->processInput();

        scene.camera->update(dt);

        GL::setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::clear(GL::ClearTarget::Color | GL::ClearTarget::Depth);

        // draw our first triangle
        shader->activate();

        RenderState state;
        state.shader = shader;

        scene.render(shader);

        window->swapBuffers();
        glfwPollEvents();
    }

    delete shader;

    glfwTerminate();

    return 0;
}
