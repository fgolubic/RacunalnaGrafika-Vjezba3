#include"Utils.hpp"
#include <GL/glut.h>

int widthW = 750;
int heightW = 750;
int currentTime = 0;
int previousTime = 0;
ObjectS* object;
TrajectoryS* trajectory;
float t = 0.0;
int controlPoint = 1;
glm::vec3 center;

void setParameters() {
	t += 0.003;

	if (1.0f <= t)
	{
		controlPoint++;
		t = 0.0f;
	}

	if (trajectory->R.size() <= controlPoint + 2)
	{
		controlPoint = 1;
		t = 0.0f;
	}
}
void renderPath()
{
	
	glBegin(GL_POINTS);
	for (auto& p : trajectory->trajectoryPoints)
	{
		glVertex3f(p.x  * 0.03f , p.y * 0.03f , p.z * 0.03f);
	}
	glEnd();
	
}

void init()
{
	std::vector<VertexS> splinePoints = readTrajectory();

	trajectory = new TrajectoryS(splinePoints);

	object = load();

	center = glm::vec3(0, 0, 0);

	for (auto& p : object->polygons)
	{
		center += p.v1.getGlmVector()+p.v2.getGlmVector()+p.v3.getGlmVector();

	}
	center /= 3.0f * object->polygons.size();
	
}


void myDisplay(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	renderPath();

	glm::vec3 startingRotation =  glm::vec3(0,0,1);
	glm::vec3 goalRotation = trajectory->derivateP(t, controlPoint);

	float fi = getAngle(startingRotation, goalRotation);
	glm::vec3 axis = glm::cross(startingRotation, goalRotation);

	glPushMatrix();
	glScalef(0.03f, 0.03f, 0.03f);
	glRotatef(fi, axis.x, axis.y, axis.z );

	//tangent draw
	glm::vec3 tangent = normalize(trajectory->derivateP(t, controlPoint));
	glColor3f(0.0, 1.0, 0.0);
	glm::vec3 start = trajectory->calculatePosition(t, controlPoint);
	glm::vec3 end = start +  2.0f * tangent;
	glBegin(GL_LINES);
	glVertex3f(start.x , start.y , start.z);
	glVertex3f(end.x , end.y , end.z );
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (auto & p : object->polygons)
	{
		glm::vec3 v1 = ((p.v1.getGlmVector()) - center) + (trajectory->calculatePosition(t, controlPoint));
		glm::vec3 v2 = ((p.v2.getGlmVector()) - center) + (trajectory->calculatePosition(t, controlPoint));
		glm::vec3 v3 = ((p.v3.getGlmVector()) - center)  + (trajectory->calculatePosition(t, controlPoint));

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}
	glEnd();
	glPopMatrix();
	
	setParameters();
	
	glutSwapBuffers();
}

void myDisplayDCM(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	renderPath();

	glPushMatrix();
	glScalef(0.03f, 0.03f, 0.03f);
	//tangent drawing -- RED
	glm::vec3 tangent = normalize(trajectory->derivateP(t, controlPoint));
	glColor3f(1.0, 0.0, 0.0);
	glm::vec3 start = trajectory->calculatePosition(t, controlPoint);
	glm::vec3 end = start + 2.0f *  tangent;
	glBegin(GL_LINES);
	glVertex3f(start.x , start.y , start.z );
	glVertex3f(end.x , end.y , end.z);
	glEnd();

	// draw normal -- BLUE
	glm::vec3 normal = normalize(glm::cross(tangent, trajectory->secondDerivativeP(t, controlPoint)));
	glColor3f(0.0, 0.0, 1.0);
	glm::vec3 startN = trajectory->calculatePosition(t, controlPoint);
	glm::vec3 endN = start +  normal;

	glBegin(GL_LINES);
	glVertex3f(startN.x , startN.y , startN.z );
	glVertex3f(endN.x , endN.y , endN.z );
	glEnd();

	// binormal
	glm::vec3 binormal = normalize(glm::cross(tangent, normal));
	glm::vec3 startB = trajectory->calculatePosition(t, controlPoint);
	glm::vec3 endB = start +  binormal;

	//rotation matrix
	glm::mat3x3 R = glm::mat3x3(tangent, normal, binormal);

	//draw object
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
	for (auto & p : object->polygons)
	{
		glm::vec3 v1 = ((p.v1.getGlmVector() - center)*inverse(R)) + (trajectory->calculatePosition(t, controlPoint)) ;
		glm::vec3 v2 = ((p.v2.getGlmVector() - center)*inverse(R)) + (trajectory->calculatePosition(t, controlPoint)) ;
		glm::vec3 v3 = ((p.v3.getGlmVector() - center)*inverse(R)) + (trajectory->calculatePosition(t, controlPoint)) ;

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}
	glEnd();
	glPopMatrix();
	
	setParameters();

	glutSwapBuffers();
}


void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1) {
		myDisplayDCM();
		previousTime = currentTime;
	}
}

int main(int argc, char** argv)
{
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(widthW, heightW);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Trajectory");
	gluLookAt(1, 0, 1, 4, 3, 3, 0, 1, 0);
	glutDisplayFunc(myDisplayDCM);
	glutIdleFunc(myIdle);
	glutMainLoop();
	return 0;
}