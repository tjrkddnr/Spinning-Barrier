// Spinning Barrier.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <stdio.h>
#include <cmath>
#include <ctime>
#include "gl_helper.h"
#include "Drawing.h"
#include "Init.h"

struct enemy {
	int degree;
	float velocity;
	float radius;

	enemy()
	{
		this->degree = 0;
		this->velocity = 0.01f;
		this->radius = 0.97f;
	}
};

enemy e;

GLfloat rotate = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool first = true;
float lastX = 0.0f;

void display()
{
	float currentFrame = clock();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	glClear(GL_COLOR_BUFFER_BIT);
	
	/*
	Outer Barrier
	*/
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	drawBarrier(1.0f, 360);
	glPopMatrix();

	/*
	Inner Barrier
	*/
	glPushMatrix();
	glRotatef(rotate, 0.0, 0.0, 1.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawBarrier(0.3, 30);
	glPopMatrix();
	
	/*
	Protect her!
	*/


	/*
	Enemy attacks
	*/
	glPushMatrix();
	glRotatef(e.degree, 0.0, 0.0, 1.0);
	glTranslatef(0.0, e.radius, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	drawEnemy();
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}

void arrowkey(int key, int x, int y)
{
	float _deltaTime = deltaTime / CLOCKS_PER_SEC;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		rotate += _deltaTime * 800;
		break;

	case GLUT_KEY_RIGHT:
		rotate -= _deltaTime * 800;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

	glutPostRedisplay();
}

void Passivemouse(int x, int y)
{
	if (first)
	{
		lastX = x;
		first = false;
	}

	float offsetX = x - lastX;
	lastX = x;

	float _deltaTime = deltaTime / CLOCKS_PER_SEC;
	rotate -= offsetX;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	GLdouble wfactor = (GLdouble)w / (GLdouble)DIM;
	GLdouble hfactor = (GLdouble)h / (GLdouble)DIM;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * wfactor, 1.0 * wfactor, -1.0 * hfactor, 1.0 * hfactor, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void timer(int value)
{
	printf("%lf\n", e.radius);
	e.radius -= e.velocity;
	glutPostRedisplay();
	glutTimerFunc((int)1005, timer, 0);
}

int main(int argc, char **argv)
{
	Initialize(argc, argv);

	e.degree = 60;

	glutDisplayFunc(display);
	glutSpecialFunc(arrowkey);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(Passivemouse);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();

    return 0;
}

