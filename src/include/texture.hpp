#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>


// GLuint loadPNG(const char * imagePath);
GLuint loadBMP(const char* imagePath);
GLuint loadDDS(const char* imagePath);

#endif
