#include "Game.h"


GLboolean Game::detectCollision(GameObject & first, GameObject & second)
{
	bool collisionX;
	bool collisionY;

	if (first.position.x + first.dimensions.x >= second.position.x
		&& second.position.x + second.dimensions.x >= first.position.x) {

		collisionX = true;
	}

	if (first.position.y + first.dimensions.y >= second.position.y
		&& second.position.y + second.dimensions.y >= first.position.y) {

		collisionY = true;
	}
	return collisionX && collisionY;
}

Collision Game::detectCollision(BallObject & one, GameObject & two)
{
	// Get center point circle first 
	glm::vec2 center(one.position + one.radius);

	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.dimensions.x / 2, two.dimensions.y / 2);
	glm::vec2 aabb_center(
		two.position.x + aabb_half_extents.x,
		two.position.y + aabb_half_extents.y
	);

	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;

	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;

	if (glm::length(difference) <= one.radius)
		return std::make_tuple((GLboolean)GL_TRUE, vectorDirection(difference), difference);
	else
		return std::make_tuple((GLboolean)GL_FALSE, UP, glm::vec2(0, 0));
}

Direction Game::vectorDirection(glm::vec2 target)
{
	glm::vec2 directions[] = {
	   glm::vec2(0.0f, 1.0f),	// up
	   glm::vec2(1.0f, 0.0f),	// right
	   glm::vec2(0.0f, -1.0f),	// down
	   glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), directions[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void Game::resetMap()
{
	if (this->mapId == 0)this->maps[0].load("levels/one.mp", this->width, this->height * 0.5f);
	else if (this->mapId == 1)
		this->maps[1].load("levels/two.mp", this->width, this->height * 0.5f);
	else if (this->mapId == 2)
		this->maps[2].load("levels/three.mp", this->width, this->height * 0.5f);
	else if (this->mapId == 3)
		this->maps[3].load("levels/four.mp", this->width, this->height * 0.5f);

}

void Game::resetPlayerPaddle()
{
	playerPaddle->dimensions = PLAYER_PADDLE_SIZE;
	playerPaddle->position = glm::vec2(this->width / 2 - PLAYER_PADDLE_SIZE.x / 2, this->height - PLAYER_PADDLE_SIZE.y);
	ball->reset(playerPaddle->position + glm::vec2(PLAYER_PADDLE_SIZE.x / 2 - BALL_RADIUS, -2 * BALL_RADIUS), INITIAL_BALL_VELOCITY);
	ball->Sprite = UtilityObject::getTexture("face");
	ball->isBallVelocityChanged = GL_FALSE;
}

Game::Game(GLuint width, GLuint height)
	: state(GAME_ACTIVE), keyboardKeys(), width(width), height(height)
{

}

Game::~Game()
{
	delete renderer;
	delete playerPaddle;
	delete ball;
	delete particles;
}

void Game::init()
{
	// ucitaj sjencare
	UtilityObject::loadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	UtilityObject::loadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
	
	
	// podesi sjencare
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
	UtilityObject::getShader("sprite").use().setInteger("sprite", 0);
	UtilityObject::getShader("sprite").setMatrix4("projection", projection);
	UtilityObject::getShader("particle").use().setInteger("sprite", 0);
	UtilityObject::getShader("particle").setMatrix4("projection", projection);

	//Ucitaj teksture
	UtilityObject::loadTexture("textures/background.jpg", GL_FALSE, "background");
	UtilityObject::loadTexture("textures/awesomeface.png", GL_TRUE, "face");
	UtilityObject::loadTexture("textures/shockedFace.png", GL_TRUE, "shockedface");
	UtilityObject::loadTexture("textures/block_texture.png", GL_FALSE, "block");
	UtilityObject::loadTexture("textures/block_wall.png", GL_FALSE, "block_solid");
	UtilityObject::loadTexture("textures/paddle.png", true, "paddle");
	UtilityObject::loadTexture("textures/particle.png", GL_TRUE, "particle");


	Shader shader = UtilityObject::getShader("sprite");
	renderer = new SpriteRenderer(shader);
	shader = UtilityObject::getShader("particle");
	Texture texture = UtilityObject::getTexture("particle");
	particles = new ParticleEmitter(shader, texture, 1000);


	// Ucitaj mape
	GameMap one; one.load("maps/one.mp", this->width, this->height * 0.5);
	GameMap two; two.load("maps/two.mp", this->width, this->height * 0.5);
	GameMap three; three.load("maps/three.mp", this->width, this->height * 0.5);
	GameMap four; four.load("maps/four.mp", this->width, this->height * 0.5);
	GameMap five; five.load("maps/five.mp", this->width, this->height * 0.5);

	this->maps.push_back(one);
	this->maps.push_back(two);
	this->maps.push_back(three);
	this->maps.push_back(four);
	this->maps.push_back(five);
	this->mapId = 0;

	glm::vec2 playerPos = glm::vec2(
		this->width / 2 - PLAYER_PADDLE_SIZE.x / 2,
		this->height - PLAYER_PADDLE_SIZE.y
	);
	playerPaddle = new GameObject(playerPos, PLAYER_PADDLE_SIZE, UtilityObject::getTexture("paddle"));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_PADDLE_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		UtilityObject::getTexture("face"));

}

void Game::update(GLfloat dt)
{
	
	if (this->maps[mapId].isCompleted()) {

		this->mapId++;
		if (this->mapId == this->maps.size()) this->mapId = 1;
		this->resetPlayerPaddle();
		return;
	}

	ball->move(dt, this->width);
	// Check for collisions
	this->collide();

	particles->Update(dt, *ball, 5, glm::vec2(ball->radius / 2));

	if (ball->position.y >= this->height) // Did ball reach bottom edge?
	{
		this->resetMap();
		this->resetPlayerPaddle();
	}

	if (ball->isBallVelocityChanged == GL_TRUE) {

		if (UtilityObject::getTexture("face").id == ball->Sprite.id) {

			ball->Sprite = UtilityObject::getTexture("shockedface");
		}
		else {

			ball->Sprite = UtilityObject::getTexture("face");
		}

		ball->isBallVelocityChanged = GL_FALSE;
	}
	

}


void Game::processInput(GLfloat dt)
{
	if (this->state == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_PADDLE_VELOCITY * dt;
		
		if (this->keyboardKeys[GLFW_KEY_LEFT])
		{
			if (playerPaddle->position.x >= 0) {

				playerPaddle->position.x -= velocity;

				if (ball->isStuck) {
					ball->position.x -= velocity;
				}

			}
			
		}
		if (this->keyboardKeys[GLFW_KEY_RIGHT])
		{
			if (playerPaddle->position.x <= this->width - playerPaddle->dimensions.x) {

				playerPaddle->position.x += velocity;

				if (ball->isStuck) {
					ball->position.x += velocity;
				}

			}
			
		}

		if (this->keyboardKeys[GLFW_KEY_SPACE])
			ball->isStuck = false;
	}
}

void Game::render()
{
	if (this->state == GAME_ACTIVE) {

		Texture texture = UtilityObject::getTexture("background");
		renderer->DrawSprite(texture, glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);

		
		this->maps[this->mapId].draw(*renderer);

		
		playerPaddle->Draw(*renderer);
			
		particles->Draw();
		ball->Draw(*renderer);
	}
}

void Game::collide()
{
	for (GameObject &box : this->maps[this->mapId].bricks)
	{
		if (!box.isDestroyed)
		{
			Collision collision = detectCollision(*ball, box);
			if (std::get<0>(collision)) 
			{
				
				if (!box.isSolid)
					box.isDestroyed = GL_TRUE;

				// Rezolucija kolizije
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				if (dir == LEFT || dir == RIGHT)
				{
					ball->velocity.x = -ball->velocity.x;
					
					GLfloat penetration = ball->radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						ball->position.x += penetration; 
					else
						ball->position.x -= penetration; 



				}
				else //kolizija s blokom horizontalno
				{
					ball->velocity.y = -ball->velocity.y; 
					
					
					GLfloat penetration = ball->radius - std::abs(diff_vector.y);
					if (dir == UP)
						ball->position.y -= penetration; 
					else
						ball->position.y += penetration; 

					ball->isBallVelocityChanged = GL_TRUE;
				}

				
			}
		}
	}

	//kolizija lopte sa palicom

	Collision result = detectCollision(*ball, *playerPaddle);


	if (!ball->isStuck && std::get<0>(result))
	{
		// provjera lokacije kolizije
		GLfloat centerBoard = playerPaddle->position.x + playerPaddle->dimensions.x / 2;
		GLfloat distance = (ball->position.x + ball->radius) - centerBoard;
		GLfloat percentage = distance / (playerPaddle->dimensions.x / 2);


		//pomakni loptu
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = ball->velocity;
		ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		ball->velocity.y = -ball->velocity.y;
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
		
		ball->velocity.y = -1 * abs(ball->velocity.y);
		ball->isBallVelocityChanged = GL_TRUE;
	}

}
