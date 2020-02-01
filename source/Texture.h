#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>


class Texture
{
public:
	
	GLuint id;
	
	GLuint w, h; 
	
	GLuint internalFormat; 
	GLuint imageFormat; 
	
	GLuint wrapS;
	GLuint wrapT;
	GLuint filterMin;
	GLuint filterMax;

	
	Texture();

	// generiraj teksturu iz datoteke
	void generate(GLuint width, GLuint height, unsigned char* data);

	void bind() const;
};

#endif