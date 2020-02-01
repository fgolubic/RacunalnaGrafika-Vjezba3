#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include"SpriteRenderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
	// Object state
	glm::vec2   position, dimensions, velocity;
	glm::vec3   color;
	GLfloat     rotation;
	GLboolean   isSolid;
	GLboolean   isDestroyed;
	// render state
	Texture   Sprite;
	// Constructor(s)
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer);
};

#endif