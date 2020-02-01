#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include<windows.h>

#include<gl/freeglut.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string>
#include <vector>
#include<glm/glm.hpp>
#include<math.h>
#include "SOIL2/SOIL.h"

using namespace std;

struct Source {
	float x, y, z;
	int numOfParticles;
	double size;
	float velocity;
};

struct Particle {
	float x, y, z;
	float r, g, b;
	float velocity;
	int t;
	float sX, sY, sZ;
	float osX, osY, osZ;
	double angle; double size;
};

vector<Particle> particles;
Source source;
GLuint window;
GLuint sub_width = 1024, sub_height = 512;

void myDisplay();
void myIdle();
void myReshape(int width, int height);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myMouse(int button, int state, int x, int y);

void draw();
void drawParticle(Particle particle);
GLuint LoadTexture(const char *filename, int w, int h);

glm::vec3 ociste(0.0, 0.0, 75.0);

int main(int argc, char **argv)
{
	source.x = 0.0; source.y = 0.0f; source.z = 0.0;
	source.numOfParticles = 30;
	source.size = 0.6;
	source.velocity = 0.7;

	srand(time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(sub_width, sub_height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Particles");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutIdleFunc(myIdle);
	GLuint tex = LoadTexture("./smoke.bmp", 50, 50);

	//Blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glutMainLoop();
	return 0;
}

void myReshape(int width, int height)
{
	sub_width = width;                      	
	sub_height = height;						

	glViewport(0, 0, sub_width, sub_height);	

	
	glMatrixMode(GL_PROJECTION);                
	glLoadIdentity();                          

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 150.0f);

	glMatrixMode(GL_MODELVIEW);                 
	glLoadIdentity();                          
}

int t = 0;

void myDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(ociste.x, ociste.y, -ociste.z);

	draw();

	glutSwapBuffers();
}

int currentTime = 0; int previousTime = 0;

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 30) {

		// Stvori nove particles
		if (source.numOfParticles > 0) {
			int n = rand() % source.numOfParticles + 1;
			for (int j = 0; j < n; j++) {
				double x, y, z;
				x = (rand() % 2000) / (double)1000 - 1;
				y = (rand() % 2000) / (double)1000 - 1;
				z = (rand() % 2000) / (double)1000 - 1;

				Particle particle;
				particle.x = source.x;
				particle.z = source.z;
				particle.y = source.y;

				particle.r = 1.0f;
				particle.g = 1.0f;
				particle.b = 1.0f;

				particle.velocity = source.velocity;
				particle.sX = x; particle.sY = y; particle.sZ = z;
				particle.t = 70 + (rand() % 31);
				particle.size = source.size;
				particles.push_back(particle);
			}
		}
		// Pomakni particles
		for (int j = particles.size() - 1; j >= 0; j--) {

			//Rotacija
			glm::vec3 s(0.0, 0.0, 1.0);
			glm::vec3 os;
			glm::vec3 e;

			e.x = particles.at(j).x - ociste.x;
			e.y = particles.at(j).y - ociste.y;
			e.z = particles.at(j).z - ociste.z;

			os = glm::cross(s, e);

			double apsS = sqrt(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0));
			double apsE = sqrt(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0));

			double angle = acos(glm::dot(s, e) / (apsS*apsE));

			particles.at(j).angle = angle / (2 * M_PI) * 360;
			particles.at(j).osX = os.x;
			particles.at(j).osY = os.y;
			particles.at(j).osZ = os.z;

			// Translacija
			particles.at(j).sY += 0.1;
			particles.at(j).y += particles.at(j).velocity * particles.at(j).sY;
			particles.at(j).z += particles.at(j).velocity * particles.at(j).sZ;
			particles.at(j).x += particles.at(j).velocity * particles.at(j).sX;

			//Smanji zivot
			particles.at(j).t--;

			if (particles.at(j).t < 90 && particles.at(j).t > 80) {

				particles.at(j).r = 1.0f;

				particles.at(j).g = 1.0f;

				particles.at(j).b = 0.0f;

			}

			else if (particles.at(j).t < 80) {

				particles.at(j).r = 1.0f;

				particles.at(j).g = 0.0f;

				particles.at(j).b = 0.0f;

			}

			if (particles.at(j).t <= 0) {
				particles.erase(particles.begin() + j);
			}
		}

		myDisplay();
		previousTime = currentTime;
	}
}

void draw() {
	for (int j = 0; j < particles.size(); j++) {
		drawParticle(particles.at(j));
	}
}

void drawParticle(Particle particle) {

	glColor3f(particle.r, particle.g, particle.b);
	glTranslatef(particle.x, particle.y, particle.z);
	glRotatef(particle.angle, particle.osX, particle.osY, particle.osZ);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-particle.size, -particle.size, 0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-particle.size, +particle.size, 0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(+particle.size, +particle.size, 0.0);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(+particle.size, -particle.size, 0.0);

	glEnd();
	glRotatef(-particle.angle, particle.osX, particle.osY, particle.osZ);
	glTranslatef(-particle.x, -particle.y, -particle.z);
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {

	//pozicija izvora
	if (theKey == 'a') source.x -= 0.5;
	if (theKey == 'd') source.x += 0.5;
	if (theKey == 's') source.y -= 0.5;
	if (theKey == 'w') source.y += 0.5;

	// kolicina cestica
	if (theKey == 'q' && source.numOfParticles > 0) source.numOfParticles--;
	if (theKey == 'e' && source.numOfParticles < 49) source.numOfParticles++;

	// velicina cestica
	if (theKey == '+' && source.size < 4.0) source.size += 0.01;
	if (theKey == '-' && source.size > 0.02) source.size -= 0.01;

	// brzina cestica
	if (theKey == 'y') source.velocity += 0.1;
	if (theKey == 'x') source.velocity -= 0.1;
}

void myMouse(int button, int state, int x, int y)
{
	// Wheel reports as button 3(scroll up) and button 4(scroll down)
	if ((button == 3) || (button == 4)) // It's a wheel event
	{
		// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
		if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events

		if (button == 3)
			source.z += 0.5;
		else
			source.z -= 0.5;

	}
}

GLuint LoadTexture(const char * filename, int w, int h)
{
	GLuint texture;

	unsigned char* data;

	data = SOIL_load_image(filename, &w, &h, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}
