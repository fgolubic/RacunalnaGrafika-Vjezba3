#include "BallObject.h"


BallObject::BallObject()
	:GameObject(), radius(13.0f), isStuck(true),isBallVelocityChanged(GL_FALSE) { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite)
	:GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), radius(radius), isStuck(true) { }

glm::vec2 BallObject::move(GLfloat dt, GLuint window_width)
{
	
	if (!this->isStuck)
	{
		
		this->position += this->velocity * dt;

		// izasla iz prozora
		if (this->position.x <= 0.0f)
		{
			this->velocity.x = -this->velocity.x;
			this->position.x = 0.0f;
		}
		else if (this->position.x + this->dimensions.x >= window_width)
		{
			this->velocity.x = -this->velocity.x;
			this->position.x = window_width - this->dimensions.x;
		}
		if (this->position.y <= 0.0f)
		{
			this->velocity.y = -this->velocity.y;
			this->position.y = 0.0f;
			isBallVelocityChanged = GL_TRUE;
		}

	}
	return this->position;
}

// vrati loptu na pocetno mjesto
void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->isStuck = true;
	this->isBallVelocityChanged = GL_FALSE;
}