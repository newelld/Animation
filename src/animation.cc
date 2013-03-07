#include <GL/gl.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Arm.h"
#include "Wheel.h"
#include "Basketball.h"

using namespace std;
void displayCallback();

int glut_win;

Wheel *wheel;
Arm* swingarm;
Basketball* basketball;
glm::mat4 wheel_cf;
glm::mat4 swing_cf;
glm::mat4 basketball_cf;
const float INIT_SWING_ANGLE = 35.0f;
const float GRAVITY = 9.8; 

GLfloat eye[] = {200, 150, 80};

GLfloat light0_pos[] = {0, 5, 10};                    
GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};  

/*--------------------------------*
 * GLUT Reshape callback function *
 *--------------------------------*/
void reshapeCallback (int w, int h){
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (float) w / (float) h, 5.0, 800.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	gluLookAt (25, 20, 0, 0, 0, 0, 0, 0, 1);
	GLfloat mat[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, mat);
}

/*================================================================*
 * Idle Callback function. This is the main engine for simulation *
 *================================================================*/
void idleCallback(){
	static clock_t last_timestamp = 0;
	static float swing_time = 0;
	clock_t current;
	double delta;
	current = clock();
	delta = 1000.0 * (current - last_timestamp)/CLOCKS_PER_SEC;
	if (delta < 25) return;
	wheel_cf = glm::rotate(wheel_cf, 20.0f, 0.0f, 1.0f, 0.0f);
	float angle = INIT_SWING_ANGLE * cos (swing_time * sqrt(swingarm->length()/GRAVITY) * M_PI / 180.0);
	swing_time += 10;
	swing_cf = glm::rotate(angle, 0.0f, 1.0f, 0.0f);
	last_timestamp = current;
	glutSetWindow (glut_win);
	glutPostRedisplay();
}

void myGLInit (){
	glPolygonMode (GL_FRONT, GL_FILL);
	glPolygonMode (GL_BACK, GL_LINE);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glEnable (GL_LIGHTING);
	glEnable (GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
}

/*--------------------------------*
 * GLUT display callback function *
 *--------------------------------*/
void displayCallback (){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv (GL_LIGHT0, GL_POSITION, light0_pos);


	glPushMatrix();
	glTranslatef (0, 0, 10);
	glPushMatrix();
	glMultMatrixf(&swing_cf[0][0]);
	glMultMatrixf(&wheel_cf[0][0]);
	basketball->draw();
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf(&swing_cf[0][0]);
	swingarm->draw();
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers ();
}

void myModelInit (){
	basketball = new Basketball();
	basketball->newInstance();
	basketball->setColor(1.0, 1.0, 1.0);

	wheel = new Wheel();
	wheel->newInstance();
	wheel_cf = glm::translate(wheel_cf, 0.0f, 0.0f, -swingarm->length());

	swingarm = new Arm;
	swingarm->newInstance();
	swingarm->setColor(1.0, 0.65, 0.342);
	swing_cf = glm::rotate(swing_cf, INIT_SWING_ANGLE, 0.0f, 1.0f, 0.0f);
}

int main (int argc, char **argv){
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (0, 0); 
	glut_win = glutCreateWindow ("CS367 Computer Graphics");
	srand (time(0));
	myGLInit ();
	myModelInit ();
	glutIdleFunc (idleCallback);
	glutDisplayFunc (displayCallback);
	glutReshapeFunc (reshapeCallback);
	glutMainLoop ();
}
