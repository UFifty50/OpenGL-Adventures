#include <vector>
#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.hpp"
#include "texture.hpp"

#include "text2D.hpp"


Text2D::Text2D(std::string const& texturePath) {
    // init texture
    Text2DTextureID = ::loadTexture(texturePath);

    // init VBO
    glGenBuffers(1, &Text2DVertexBufferID);
    glGenBuffers(1, &Text2DUVBufferID);

    // init shaders
    Text2DShaderID = ::loadShaders("shaders/Text2D.vert", "shaders/Text2D.frag");

    // init texture sampler uniform
    Text2DUniformID = glGetUniformLocation(Text2DShaderID, "Text2DTextureSampler");
}

void Text2D::printText2D(std::string const& text, int x, int y, int size) const {
    unsigned int length = text.length();

    // fill buffers
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> UVs;
        
    for (unsigned int i = 0; i < length; i++) {
        auto vertexUpLeft = glm::vec2(x + i * size, y + size);
        auto vertexUpRight = glm::vec2(x + i * size + size, y + size);
        auto vertexDownRight = glm::vec2(x + i * size + size, y);
        auto vertexDownLeft = glm::vec2(x + i * size, y);

        vertices.push_back(vertexUpLeft);
        vertices.push_back(vertexDownLeft);
        vertices.push_back(vertexUpRight);

        vertices.push_back(vertexDownRight);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexDownLeft);

        char character = text[i];
        float UVx = (character % 16) / 16.0f;
        float UVy = (character / 16) / 16.0f;

        auto uvUpLeft = glm::vec2(UVx, UVy);
        auto uvUpRight = glm::vec2(UVx + 1.0f / 16.0f, UVy);
        auto uvDownRight = glm::vec2(UVx + 1.0f / 16.0f, UVy + 1.0f / 16.0f);
        auto uvDownLeft = glm::vec2(UVx, UVy + 1.0f / 16.0f);

        UVs.push_back(uvUpLeft);
        UVs.push_back(uvDownLeft);
        UVs.push_back(uvUpRight);

        UVs.push_back(uvDownRight);
        UVs.push_back(uvUpRight);
        UVs.push_back(uvDownLeft);
    }

    glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);


    // bind shader
    glUseProgram(Text2DShaderID);

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    ::glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
    glUniform1i(Text2DUniformID, 0);

    // attribute buffer 1: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // attribute buffer 2: UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw
    ::glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    ::glDisable(GL_BLEND);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Text2D::cleanUp() const {
    glDeleteBuffers(1, &Text2DVertexBufferID);
    glDeleteBuffers(1, &Text2DUVBufferID);
    ::glDeleteTextures(1, &Text2DTextureID);
    glDeleteProgram(Text2DShaderID);
}
