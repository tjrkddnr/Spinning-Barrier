// Spinning Barrier.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <stdio.h>
#include <cmath>
#include <ctime>
#include "gl_helper.h"
#include "Drawing.h"
#include "Init.h"

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
	
	glPushMatrix();
	glRotatef(rotate, 0.0, 0.0, 1.0);
	drawBarrier(0.3, 90);
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

int main(int argc, char **argv)
{
	Initialize(argc, argv);

	glutDisplayFunc(display);
	glutSpecialFunc(arrowkey);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(Passivemouse);
	glutMainLoop();

    return 0;
}

