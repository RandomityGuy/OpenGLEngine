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
#include "renderer.h"
#include "skybox.h"


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

    Renderer renderer(window);

    glm::vec3 lightPos(-1, -1, -1);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    
    DirLight dirLight;
    dirLight.direction = lightPos;
    dirLight.ambient = ambientColor;
    dirLight.diffuse = diffuseColor;
    dirLight.specular = glm::vec3(1, 1, 1);

    renderer.scene->addChild(&dirLight);


    Model model;
    Mesh mesh;
    mesh.load("data/cube.obj");
    model.mesh = &mesh;

    glm::mat4 tform = glm::mat4(1);
   
    model.setTransform(tform);

    renderer.scene->addChild(&model);

    Model model2;
    Mesh mesh2;
    mesh2.load("data/window.obj");
    mesh2.materials[0].transparent = true;
    model2.mesh = &mesh2;

    glm::mat4 tform2 = glm::translate(tform, glm::vec3(0, 5, 0));
    model2.setTransform(tform2);

    renderer.scene->addChild(&model2);

    Model model3;
    model3.mesh = &mesh2;

    glm::mat4 tform3 = glm::translate(tform, glm::vec3(2, 8, 0));
    model3.setTransform(tform3);

    renderer.scene->addChild(&model3);

    double prevT = glfwGetTime();

    while (!window->shouldClose())
    {
        double t = glfwGetTime();
        double dt = t - prevT;
        prevT = t;

        window->processInput();

        renderer.scene->camera->update(dt);
        renderer.render();

        window->swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
