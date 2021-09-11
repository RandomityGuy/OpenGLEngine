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

    Texture2D texture;
    texture.setWrap(Texture2D::Wrap::Repeat, Texture2D::Wrap::Repeat);
    texture.setFiltering(Texture2D::LinearMipMapLinear, Texture2D::Linear);
    texture.load("data/container.jpg");
    texture.generateMipMaps();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    VertexBuffer VBO;
    VertexArray VAO;
    IndexBuffer EBO;

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    VAO.bind();
    VBO.uploadData(sizeof(vertices), vertices, VertexBuffer::BufferDrawType::Static);

    EBO.uploadData(sizeof(indices), indices, IndexBuffer::BufferDrawType::Static);

    GL::vertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(float), 0, false);
    GL::enableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    GL::bindBuffer(GL::BufferTarget::ArrayBuffer, 0);

    VertexArray lightVAO;
    lightVAO.bind();
    VBO.bind();
    GL::vertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(float), 0, false);
    GL::enableVertexAttribArray(0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    GL::bindVertexArray(0);

    glm::mat4 model = glm::mat4(1);

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    GL::setDepthTest(true);

    Camera camera = Camera(window, glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
    camera.mode = Camera::Normal;

    camera.position = glm::vec3(0, 0, 0);

    double prevT = glfwGetTime();

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

        model = glm::rotate(glm::mat4(1) , (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        VAO.bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader->setUniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            shader->setUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("model", model);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }


        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightingShader->activate();
        lightingShader->setUniform("view", camera.view);
        lightingShader->setUniform("projection", camera.projection);
        lightingShader->setUniform("model", model);
        lightVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);


        window->swapBuffers();
        glfwPollEvents();
    }

    delete shader;

    glfwTerminate();

    return 0;
}
