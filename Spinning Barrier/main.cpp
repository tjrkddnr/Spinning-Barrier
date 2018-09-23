
#include <stdio.h>
#include <cstdlib>
#include <vector>
//#include <map>
#include <cmath>
#include <ctime>
#include "gl_helper.h"
#include "Drawing.h"
#include "Init.h"
#include "Barrier.h"
#include "Enemy.h"

/*
key mapping
*/
enum {LEFT, RIGHT, UP, DOWN};
bool arrow[4] = { false, };
//std::map<int, bool> keys;

Barrier barrier;
std::vector<Enemy*> enemy;

/*
time based movement
*/
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 0.0f;

void detecte_collision()
{
	for (int i = 0; i < enemy.size(); i++)
	{
		/*
		casting float type into int to handle the precision error
		*/
		int _rotate_begin = (int)barrier.Rotate % 360;
		if (_rotate_begin < 0)
			_rotate_begin += 360;
		int _rotate_end = _rotate_begin + (int)barrier.AngleBetween;

		if (!enemy[i]->collide && 25 < (int)(enemy[i]->Distance * 100) && (int)(enemy[i]->Distance * 100) < 30)
			if (_rotate_begin < (int)enemy[i]->Rotate && (int(enemy[i]->Rotate) < _rotate_end))
				enemy[i]->collide = true;
	}
}

void handle_collision()
{
	for (int i = 0; i < enemy.size();)
	{
		if ((enemy[i]->collide) || (!enemy[i]->collide && (int)(enemy[i]->Distance * 100) < 5))
		{
			//(enemy[i]->collide) ? score += 1 : life -= 1;
			delete(enemy[i]);
			std::vector<Enemy*>::iterator iter = enemy.begin();
			iter += i;
			enemy.erase(iter);
		}
		else
			i++;
	}
}

void updateBarrier()
{
	float _deltaTime = deltaTime / CLOCKS_PER_SEC;

	/*
	update barrier position
	*/
	if (arrow[LEFT]) { barrier.updateRotate(_deltaTime); }
	if (arrow[RIGHT]) { barrier.updateRotate(-_deltaTime); }
}

void updateEnemy()
{
	float _deltaTime = deltaTime / CLOCKS_PER_SEC;

	/*
	update enemy position
	*/
	for (int i = 0; i < enemy.size(); i++)
		enemy[i]->updateDistance(_deltaTime);
}

void display()
{
	float currentFrame = clock();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	detecte_collision();
	handle_collision();
	updateBarrier();
	updateEnemy();

	/* draw */
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
	glRotatef(barrier.Rotate, 0.0, 0.0, 1.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawBarrier(0.3, barrier.AngleBetween);
	glPopMatrix();
	
	/*
	Protect her!
	*/


	/*
	Enemy attacks
	*/
	for (int i = 0; i < enemy.size(); i++)
	{
		glPushMatrix();
		glRotatef(enemy[i]->Rotate, 0.0, 0.0, 1.0);
		glTranslatef(enemy[i]->Distance, 0.0, 0.0);
		glColor3f(1.0, 1.0, 0.0);
		drawEnemy();
		glPopMatrix();
	}
	

	glutSwapBuffers();
	glutPostRedisplay();
}

void arrowkey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		arrow[LEFT] = true;
		break;

	case GLUT_KEY_RIGHT:
		arrow[RIGHT] = true;
		break;
	}
	glutPostRedisplay();
}

void arrowkeyUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		arrow[LEFT] = false;
		break;

	case GLUT_KEY_RIGHT:
		arrow[RIGHT] = false;
		break;
	}
}

/*
void keyboard(unsigned char key, int x, int y)
{
	keys[key] = true;
	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
	keys[key] = false;
	glutPostRedisplay();
}
*/

void timer(int value)
{
	int random = rand() % 360;
	enemy.push_back(new Enemy(0.5f, random));

	glutPostRedisplay();
	glutTimerFunc(1000, timer, 0);
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
	glutSpecialUpFunc(arrowkeyUp);
	//glutKeyboardFunc(keyboard);
	//glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutMainLoop();

    return 0;
}