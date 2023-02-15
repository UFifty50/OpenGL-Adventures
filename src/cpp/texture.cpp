#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <string.h>
#include <array>
#include <utility>

#include "GL/glew.h"
#include "GLFW/glfw3.h"


GLuint loadBMP(std::string const& imagePath) {
    std::cout << "Reading image " << imagePath << std::endl;
	
	std::ifstream file;

    std::string header;
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;

    std::string data;

    file.open(imagePath, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
        std::cout << imagePath << " could not be opened."<< std::endl;
        return -1;
    }

	file.read(header.data(), 54);
	if (file.eof()) {
		std::cout << imagePath << " is not a correct BMP file." << std::endl;
		file.close();
		return -1;
	}

    if (header.at(0) != 'B' || header.at(1) != 'M') {
		std::cout << imagePath << " is not a correct BMP file." << std::endl;
        file.close();
        return -1;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (imageSize == 0) imageSize = width*height*3;
    if (dataPos == 0) dataPos = 54;


	file.read(data.data(), imageSize);
    file.close();

    GLuint textureID;
    ::glGenTextures(1, &textureID);

    ::glBindTexture(GL_TEXTURE_2D, textureID);
    ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.c_str());

    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

constexpr unsigned int FOURCC_DXT1 = 0x31545844;
constexpr unsigned int FOURCC_DXT3 = 0x33545844;
constexpr unsigned int FOURCC_DXT5 = 0x35545844;

GLuint loadDDS(const char* imagePath) {

    unsigned char header[124];

	FILE *fp; 


	fp = fopen(imagePath, "rb"); 
	if (fp == nullptr){
		printf("%s could not be opened.", imagePath);
		return -1;
	}

    char filecode[4];
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp);
		return -1;
	}

	fread(&header, 124, 1, fp); 

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

 
	unsigned char* buffer;
	unsigned int bufsize;

	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 

	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) { 
	    case FOURCC_DXT1: 
	    	format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
	    	break; 
	    case FOURCC_DXT3: 
	    	format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
	    	break; 
	    case FOURCC_DXT5: 
	    	format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
	    	break; 
	    default: 
	    	free(buffer); 
	    	return 0; 
	}

	GLuint textureID;
	glGenTextures(1, &textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3) / 4) * ((height+3) / 4) * blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width /= 2; 
		height /= 2; 

		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureID;
}

GLuint loadTexture(std::string const& imagePath) {
	if (imagePath.ends_with(".dds") || imagePath.ends_with(".DDS") ||
	    imagePath.ends_with(".Dds") || imagePath.ends_with(".dDs") ||
		imagePath.ends_with(".ddS") || imagePath.ends_with(".DdS") ||
		imagePath.ends_with(".dDS") || imagePath.ends_with(".DDs")) {
		return ::loadDDS(imagePath.c_str());
	} else {
		return ::loadBMP(imagePath);
	}
}
