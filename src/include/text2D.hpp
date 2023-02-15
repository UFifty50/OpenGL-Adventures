#ifndef TEXT2D_HPP
#define TEXT2D_HPP

#include <string>


class Text2D {
private:
    unsigned int Text2DTextureID;
    unsigned int Text2DVertexBufferID;
    unsigned int Text2DUVBufferID;
    unsigned int Text2DShaderID;
    unsigned int Text2DUniformID;

public:
    explicit Text2D(std::string const& texturePath);

    void printText2D(std::string const& text, int x, int y, int size) const;
    void cleanUp() const;
};

#endif