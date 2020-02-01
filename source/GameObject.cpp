#include "GameObject.h"



GameObject::GameObject()
	:position(0, 0), dimensions(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), Sprite(), isSolid(false), isDestroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color, glm::vec2 velocity)
	:position(pos), dimensions(size), velocity(velocity), color(color), rotation(0.0f), Sprite(sprite), isSolid(false), isDestroyed(false) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->position, this->dimensions, this->rotation, this->color);
}