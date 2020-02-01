#define GLAD_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "Game.h"
#include "UtilityObject.h"
#include<cstdio>



void checkKeys(GLFWwindow* window, int key, int scancode, int action, int mode);


const GLuint SCREEN_WIDTH = 1080;

const GLuint SCREEN_HEIGHT = 600;

//glavni objekt igre
Game arcadeGame(SCREEN_WIDTH, SCREEN_HEIGHT);


int main(int argc, char *argv[])
{
	

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arcade Game", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Load all OpenGL functions using the glfw loader function
	// If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
	// Alternative use the builtin loader, e.g. if no other loader function is available
	/*
	if (!gladLoadGL()) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
	*/

	// glad populates global constants after loading to indicate,
	// if a certain extension/version is available.
	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	glfwSetKeyCallback(window, checkKeys);

	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	arcadeGame.init();


	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;


	arcadeGame.state = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		
		arcadeGame.processInput(deltaTime);

	
		arcadeGame.update(deltaTime);

		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		arcadeGame.render();

		glfwSwapBuffers(window);
	}

	
	UtilityObject::clear();

	glfwTerminate();
	return 0;
}

void checkKeys(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			arcadeGame.keyboardKeys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			arcadeGame.keyboardKeys[key] = GL_FALSE;
	}
}