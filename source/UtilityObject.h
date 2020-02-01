#ifndef UTILITY_OBJECT_H
#define UTILITY_OBJECT_H

#include <map>
#include <string>

#include "Shader.h"
#include "Texture.h"

//baza podataka per se
class UtilityObject
{
public:
	
	static std::map<std::string, Shader>  shaders;
	static std::map<std::string, Texture> textures;

	//ucitaj i genereiraj sjencare
	static Shader loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	
	static Shader getShader(std::string name);

	//ucitaj i generiraj teksturu iz datoteke
	static Texture loadTexture(const GLchar *file, GLboolean alpha, std::string name);
	 
	static Texture getTexture(std::string name);
	
	static void clear();
private:
	//NEPOTREBNO
	UtilityObject() { }

	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

	static Texture loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif