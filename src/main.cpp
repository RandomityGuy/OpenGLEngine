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
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <refractMesh.h>
#include <refractModel.h>

#define M_PI 3.14158265f

void renderGui()
{
    ImGui::NewFrame();
    //ImGui::Begin("Debug Information");
    //ImGui::SetWindowSize(ImVec2(400, 120));
    //ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);

    //ImGui::Separator();
    //ImGui::Text("GL Renderer: %s", glGetString(GL_RENDERER));
    //ImGui::Text("GL Vendor: %s", glGetString(GL_VENDOR));
    //ImGui::Text("GL Version: %s", glGetString(GL_VERSION));

    //ImGui::End();
    ImGui::Render();
}

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

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->window, true); // true installs callbacks automatically
    ImGui_ImplOpenGL3_Init("#version 430\n");

    Renderer renderer(window);

    glm::vec3 lightPos(-1, -1, -1);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    
    DirLight dirLight;
    dirLight.direction = lightPos;
    dirLight.ambient = ambientColor;
    dirLight.diffuse = diffuseColor;
    dirLight.specular = glm::vec3(1, 1, 1);

    renderer.scene->addChild(dirLight);


    Model model;
    Mesh mesh;
    mesh.load("data/cube.obj");
    model.setMesh(mesh);

    glm::mat4 tform = glm::mat4(1);
   
    model.setTransform(tform);

    renderer.scene->addChild(model);

    Model windowModel;
    Mesh windowMesh;
    windowMesh.load("data/window.obj");
    windowMesh.materials[0].transparent = true;
    windowModel.setMesh(windowMesh);


    glm::mat4 tform2 = glm::translate(tform, glm::vec3(2, 0, 0));
    tform2 = glm::rotate(tform2, (float)M_PI / 2, glm::vec3(0, 0, 1));
    windowModel.setTransform(tform2);

    model.addChild(windowModel);

    Model windowModel2;
    windowModel2.setMesh(windowMesh);

    glm::mat4 tform3 = glm::translate(tform, glm::vec3(-3, 0, 0));
    tform3 = glm::rotate(tform3, (float)M_PI / 2, glm::vec3(0, 0, 1));
    windowModel2.setTransform(tform3);

    model.addChild(windowModel2);


    Model windowModel3;
    windowModel3.setMesh(windowMesh);

    glm::mat4 tform4 = glm::translate(tform, glm::vec3(0, -2, 0));
    windowModel3.setTransform(tform4);

    model.addChild(windowModel3);

    Model windowModel4;
    windowModel4.setMesh(windowMesh);

    glm::mat4 tform5 = glm::translate(tform, glm::vec3(0, 3, 0));
    windowModel4.setTransform(tform5);

    model.addChild(windowModel4);

    double prevT = glfwGetTime();

    while (!window->shouldClose())
    {
        double t = glfwGetTime();
        double dt = t - prevT;
        prevT = t;

        window->processInput();

        float rot = M_PI / 4 * t;

        rot = fmodf(rot, 2 * M_PI);

        glm::mat4 rotmat = glm::rotate(tform, rot, glm::vec3(0, 0, 1));

        model.setTransform(rotmat);

        //glm::mat4 rotmat2 = glm::translate(rotmat, glm::vec3(5 * sinf((float)(M_PI / 4 * t)), 5 * cosf((float)(M_PI / 4 * t)), 0));

        //windowModel.setTransform(rotmat2);

        renderer.update(dt);
        renderer.render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        renderGui();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->swapBuffers();
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}
