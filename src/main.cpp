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
    texture.load("data/container2.png");
    texture.generateMipMaps();

    Texture2D textureSpecular;
    textureSpecular.setWrap(Texture2D::Wrap::Repeat, Texture2D::Wrap::Repeat);
    textureSpecular.setFiltering(Texture2D::LinearMipMapLinear, Texture2D::Linear);
    textureSpecular.load("data/container2_specular.png");
    textureSpecular.generateMipMaps();

    Texture2D textureEmissive;
    textureEmissive.setWrap(Texture2D::Wrap::Repeat, Texture2D::Wrap::Repeat);
    textureEmissive.setFiltering(Texture2D::LinearMipMapLinear, Texture2D::Linear);
    textureEmissive.load("data/matrix.jpg");
    textureEmissive.generateMipMaps();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

    GL::vertexAttribPointer(0, 3, GL_FLOAT, 2 * 3 * sizeof(float) + 2 * sizeof(float), 0, false);
    GL::enableVertexAttribArray(0);

    GL::vertexAttribPointer(1, 3, GL_FLOAT, 2 * 3 * sizeof(float) + 2 * sizeof(float), 3 * sizeof(float), false);
    GL::enableVertexAttribArray(1);

    GL::vertexAttribPointer(2, 2, GL_FLOAT, 2 * 3 * sizeof(float) + 2 * sizeof(float), 2 * 3 * sizeof(float), false);
    GL::enableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    GL::bindBuffer(GL::BufferTarget::ArrayBuffer, 0);

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

    glm::vec3 lightPos(-0.2f, -1.0f, -0.3f);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

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

            glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
            glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);

            shader->activateTexture(texture, "material.diffuse", 0);
            shader->activateTexture(textureSpecular, "material.specular", 1);
            shader->activateTexture(textureEmissive, "material.emission", 2);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader->setUniform("dirLight.ambient",ambientColor);
            shader->setUniform("dirLight.diffuse", diffuseColor); // darken diffuse light a bit
            shader->setUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("material.diffuse", 0);
            shader->setUniform("material.shininess", 32.0f);
            shader->setUniform("model", model);
            shader->setUniform("dirLight.direction", lightPos);
            shader->setUniform("viewPos", camera.position);

            shader->setUniform("pointLights[0].position", pointLightPositions[0]);
            shader->setUniform("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            shader->setUniform("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader->setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("pointLights[0].constant", 1.0f);
            shader->setUniform("pointLights[0].linear", 0.09);
            shader->setUniform("pointLights[0].quadratic", 0.032);
            // point light 2
            shader->setUniform("pointLights[1].position", pointLightPositions[1]);
            shader->setUniform("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            shader->setUniform("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader->setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("pointLights[1].constant", 1.0f);
            shader->setUniform("pointLights[1].linear", 0.09);
            shader->setUniform("pointLights[1].quadratic", 0.032);
            // point light 3
            shader->setUniform("pointLights[2].position", pointLightPositions[2]);
            shader->setUniform("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            shader->setUniform("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader->setUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("pointLights[2].constant", 1.0f);
            shader->setUniform("pointLights[2].linear", 0.09);
            shader->setUniform("pointLights[2].quadratic", 0.032);
            // point light 4
            shader->setUniform("pointLights[3].position", pointLightPositions[3]);
            shader->setUniform("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            shader->setUniform("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader->setUniform("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("pointLights[3].constant", 1.0f);
            shader->setUniform("pointLights[3].linear", 0.09);
            shader->setUniform("pointLights[3].quadratic", 0.032);
            // spotLight
            shader->setUniform("spotLight.position", camera.position);
            shader->setUniform("spotLight.direction", camera.direction);
            shader->setUniform("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
            shader->setUniform("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniform("spotLight.constant", 1.0f);
            shader->setUniform("spotLight.linear", 0.09);
            shader->setUniform("spotLight.quadratic", 0.032);
            shader->setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            shader->setUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }


        window->swapBuffers();
        glfwPollEvents();
    }

    delete shader;

    glfwTerminate();

    return 0;
}
