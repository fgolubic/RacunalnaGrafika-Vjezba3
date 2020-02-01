#pragma once
#include"GameObject.h"

class BallObject : public GameObject
{
public:
	
	GLfloat radius;
	GLboolean isStuck;
	GLboolean isBallVelocityChanged;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite);

	glm::vec2 move(GLfloat dt, GLuint window_width);
	void reset(glm::vec2 position, glm::vec2 velocity);
};