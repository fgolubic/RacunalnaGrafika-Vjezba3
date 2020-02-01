#pragma once
#include"GameObject.h"
#include"UtilityObject.h"
#include<vector>
#include<fstream>
#include<sstream>
class GameMap
{
public:
	std::vector<GameObject> bricks; //zapravo mapa je samo raspored blokova

	GameMap();

	void load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);

	
	void draw(SpriteRenderer& renderer);

	GLboolean isCompleted();

private:
	//inicijaliziraj mapu iz podataka o lokaciji plocica
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);

};

