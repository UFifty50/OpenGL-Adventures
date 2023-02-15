#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AnEngineConfig.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include "texture.hpp"
#include "VBOindexer.hpp"
#include "text2D.hpp"

using namespace glm;


int main() {
    GLFWwindow* win = ::window(1024, 768, "Tutorial 05 - Texturing", nullptr, nullptr);
    
    if (win == nullptr) {
        return -1;
    }

    ::glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);
    ::glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  //  glfwSetScrollCallback(win, (GLFWscrollfun)scrollCallback);
    ::glfwPollEvents();
    ::glfwSetCursorPos(win, 1024/2, 768/2);

    ::glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LESS);
    ::glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = ::loadShaders("shaders/Vshader1.vert", "shaders/Fshader1.frag");

    GLuint matrixID = glGetUniformLocation(programID, "ModelViewProjection");
    GLuint modelMatrixID = glGetUniformLocation(programID, "Model");
    GLuint viewMatrixID = glGetUniformLocation(programID, "View");
  //  GLuint modelViewMatrixID = glGetUniformLocation(programID, "ModelView");

    GLuint texture = ::loadDDS("textures/uvmap.DDS");

    GLuint textureID = glGetUniformLocation(programID, "textureSampler");
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    bool res = ::loadOBJ("obj/suzanne.obj", vertices, uvs, normals, DDS);

    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexedVertices;
    std::vector<glm::vec2> indexedUVs;
    std::vector<glm::vec3> indexedNormals;

    ::fast_indexVBO(vertices, uvs, normals, indices, indexedVertices, indexedUVs, indexedNormals);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedUVs.size() * sizeof(glm::vec2), &indexedUVs[0], GL_STATIC_DRAW);

    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);

    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    glUseProgram(programID);
    GLuint lightID = glGetUniformLocation(programID, "world_LightPos");

    Text2D text2D("textures/charMap.DDS");

    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int frames = 0;
    float lastTime = ::glfwGetTime();
    char fps[256];
    ::sprintf(fps, "0 ms/frame\t0 fps");

    do {
        // print fps
        auto currentTime = ::glfwGetTime();
        auto deltaTime = controls::DeltaTime();
        frames++;
        if (currentTime - lastTime >= 1.0) {
            //std::string fps = std::format("{} ms/frame\t{} fps", 1000.0 / deltaTime, 1.0 / deltaTime);
            ::sprintf(fps, "%f ms/frame\t%d fps", deltaTime, (int)(1.0 / deltaTime));
            ::printf("%s\n", fps);
            frames = 0;
            lastTime += 1.0;
        }

        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        
        controls::computeMatricesFromInputs(win, (float)deltaTime);
        glm::mat4 projectionMatrix = controls::getProjectionMatrix();
        glm::mat4 viewMatrix = controls::getViewMatrix();
        glm::mat4 modelMatrix = glm::mat4(1.0);
        glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
        
        glm::vec3 lightPos(4, 4, 4);
        glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
        
        glActiveTexture(GL_TEXTURE0);
        ::glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureID, 0);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(
            1,                  // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
        );

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

        ::glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            nullptr           // element array buffer offset
        );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        text2D.printText2D(std::string(fps), 5, 550, 30);

        ::glfwSwapBuffers(win);
        ::glfwPollEvents();

    } while (::glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             ::glfwWindowShouldClose(win) == 0);

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteProgram(programID);
    ::glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VertexArrayID);
    text2D.cleanUp();
    ::glfwTerminate();

    return 0;
}
