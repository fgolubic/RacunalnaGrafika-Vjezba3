#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<tuple>
#include"SpriteRenderer.h"
#include"UtilityObject.h"
#include"GameMap.h"
#include"BallObject.h"
#include"ParticleEmitter.h"


// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;



const glm::vec2 PLAYER_PADDLE_SIZE(100, 20);

const GLfloat PLAYER_PADDLE_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const GLfloat BALL_RADIUS = 12.5f;


class Game
{
private:
	SpriteRenderer* renderer;
	std::vector<GameMap> maps;
	ParticleEmitter *particles;
	GLuint mapId;
	GameObject *playerPaddle;
	BallObject *ball;
	//kolizija izmedu dva objekta (deprecated)
	GLboolean detectCollision(GameObject &first, GameObject &second);
	//kolizija izmedu lopte i objekta
	Collision detectCollision(BallObject &one, GameObject &two);
	Direction vectorDirection(glm::vec2 target);
	
	void resetMap();
	void resetPlayerPaddle();

public:
	
	GameState state;
	GLboolean keyboardKeys[1024];
	GLuint width, height;
	
	Game(GLuint width, GLuint height);
	~Game();

	// incijaliziraj igru (ucitaj sve potrebne sjencare, mape i teksture)
	void init();

	//glavna petlja igre
	void processInput(GLfloat dt);

	void update(GLfloat dt);
	void render();

	//rjesi koliziju
	void collide();
};

#endif