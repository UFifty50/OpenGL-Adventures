#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <GL/glew.h>


// GLuint loadPNG(const char * imagePath);
GLuint loadBMP(std::string const& imagePath);
GLuint loadDDS(const char* imagePath);
GLuint loadTexture(std::string const& imagePath);

#endif
