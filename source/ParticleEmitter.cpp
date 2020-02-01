﻿#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(Shader &shader, Texture &texture, GLuint amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->init();
}

void ParticleEmitter::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
	// dodaj nove
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}

	// ažuriraj sve čestice
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.ttl -= dt;

		if (p.ttl > 0.0f)
		{	
			// azuriraj cesticu
			p.position -= p.velocity * dt;
			p.colour.a -= dt * 2.5;
		}

	}


}


void ParticleEmitter::Draw()
{
	//  "additive blending" za lijepo preklapanje cestica
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();

	for (Particle particle : this->particles)
	{
		if (particle.ttl > 0.0f)
		{

			this->shader.setVector2f("offset", particle.position);
			this->shader.setVector4f("color", particle.colour);
			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

		}
	}

	//resetiraj blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleEmitter::init()
{
	// mreza cestice
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);

	//punjenje VBO-a za mrezu
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	// podesavanje atributa mreze
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// stvori osnovne cestice
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}


GLuint lastUsedParticle = 0;

GLuint ParticleEmitter::firstUnusedParticle()
{
	
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].ttl <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].ttl <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	
	lastUsedParticle = 0;
	return 0;
}

void ParticleEmitter::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{

	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.position = object.position + random + offset;


	particle.colour = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.ttl = 1.0f;

	particle.velocity = object.velocity * 0.1f;
}