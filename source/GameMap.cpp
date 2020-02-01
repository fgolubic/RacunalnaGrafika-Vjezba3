#include "GameMap.h"



GameMap::GameMap()
{
}

void GameMap::load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
	
	this->bricks.clear();
	
	GLuint tileCode;
	GameMap level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) 
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) 
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameMap::draw(SpriteRenderer & renderer)
{
	for (GameObject &tile : this->bricks) {

		if (!tile.isDestroyed) {

			tile.Draw(renderer); 

		}

	}
	
}

GLboolean GameMap::isCompleted()
{
	for (auto& b : bricks) {

		if (!b.isSolid && !b.isDestroyed) {

			return GL_FALSE;

		}

	}

	return GL_TRUE;
}

void GameMap::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = levelHeight / height;
			
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			
			if (tileData[y][x] == 1) // cvrsto
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size,
					UtilityObject::getTexture("block_solid"),
					glm::vec3(0.8f, 0.8f, 0.7f)
				);
				obj.isSolid = GL_TRUE;
				this->bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(0.0f); // original: crna
				if (tileData[y][x] == 2)
					color = glm::vec3(0.4f, 0.7f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.9f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.9f, 0.9f, 0.5f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.2f);
				else if (tileData[y][x] == 6)
					color = glm::vec3(1.0f, 1.0f, 1.0f);  //bijela
				else if (tileData[y][x] == 7)
					color = glm::vec3(1.0f, 0.0f, 0.0f); //crvena


				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);

				this->bricks.push_back(
					GameObject(pos, size, UtilityObject::getTexture("block"), color)
				);

			}

		}

	}

}
