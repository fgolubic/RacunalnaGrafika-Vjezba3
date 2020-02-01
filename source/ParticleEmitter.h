#ifndef PARTICLE_EMMITER_H
#define PARTICLE_EMMITER_H

#include"Shader.h"
#include"Texture.h"
#include"GameObject.h"
#include<vector>
#include<stdlib.h>

struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 colour;
	GLfloat ttl;

	Particle()
		:position(0.0f), velocity(0.0f), colour(1.0f), ttl(0.0f) { }
};

class ParticleEmitter
{
public:
	ParticleEmitter(Shader& shader, Texture& texture, GLuint amount);
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();
private:
	std::vector<Particle> particles;
	GLuint amount;
	Shader shader;
	Texture texture;
	GLuint VAO;
	// Initializes buffer and vertex attributes
	void init();
	// Vrati index prve nekorištenje cestice (ttl <= 0)
	GLuint firstUnusedParticle();
	//ponovno stvori cesticu
	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
#endif
