#include "SpriteRenderer.h"




SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData()
{
	// Configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	this->initRenderData();
}

void SpriteRenderer::DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	// Prepare transformations
	this->shader.use();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));  // prva translacija (skaliranje, rotacija translacija

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // izvor rotacije u sredinu kvadrata
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // rotiraj
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // vrati izvor rotacije

	model = glm::scale(model, glm::vec3(size, 1.0f)); // skaliraj

	this->shader.setMatrix4("model", model);

	// render textured quad
	this->shader.setVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}