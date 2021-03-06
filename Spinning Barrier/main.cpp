/*
	To do : UI, img load, code optimizing, put it into webGL
*/

#pragma warning(disable:4996)
#include <ctime>
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

/*
objects
*/
Barrier barrier = Barrier();
std::vector<Enemy*> enemy;

/*
game setting
*/
int score = 0;
int life = 5;
int interval = 0;
float anglebetween = 0.0f;
float enemyspeed = 0.0f;

/*
time based movement
*/
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 0.0f;

int level = 1;

void gameSet(int choose)
{
	switch (choose)
	{
	case 1:
		interval = 2;
		anglebetween = 90.0f;
		enemyspeed = 0.3f;
		break;
	case 2:
		interval = 2;
		anglebetween = 60.0f;
		enemyspeed = 0.5f;
		break;
	case 3:
		interval = 1;
		anglebetween = 30.0f;
		enemyspeed = 1.02;
		break;
	default:
		printf("error\n");
		exit(0);
	}
}

void detecte_collision()
{
	for (int i = 0; i < enemy.size(); i++)
		enemy[i]->collide = barrier.enemyCollide(*enemy[i]);
}

void handle_collision()
{
	for (int i = 0; i < enemy.size();)
	{
		if ((enemy[i]->collide) || (!enemy[i]->collide && (int)(enemy[i]->Distance * 100) < 10))
		{
			(enemy[i]->collide) ? score += 1 : life -= 1;
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

void renderBitmapCharacher(float x, float y, float z, void *font, const char *string)
{

	const char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void display()
{
	/* draw */
	glClear(GL_COLOR_BUFFER_BIT);

	if (level == 1)
	{
		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		renderBitmapCharacher(-0.3, 0.0, 0.0, GLUT_BITMAP_8_BY_13, "Press any key to start");
		renderBitmapCharacher(0.15, -0.95, 0.0, GLUT_BITMAP_8_BY_13, "Developed by Kang-Uk, Seo");
		glPopMatrix();
	}

	else if (level == 2)
	{
		float currentFrame = clock();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		detecte_collision();
		handle_collision();
		updateBarrier();
		updateEnemy();

		if (life == 0)
			level = 3;

		/*
		life count
		*/
		char str[5] = { 0, };
		itoa(life, str, 10);

		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		renderBitmapCharacher(-0.95, 0.8, 0, GLUT_BITMAP_8_BY_13, "Life : ");
		renderBitmapCharacher(-0.7, 0.8, 0, GLUT_BITMAP_8_BY_13, str);

		itoa(score, str, 10);
		renderBitmapCharacher(0.65, 0.8, 0, GLUT_BITMAP_8_BY_13, "Score : ");
		renderBitmapCharacher(0.9, 0.8, 0, GLUT_BITMAP_8_BY_13, str);

		glPopMatrix();

		/*
		Outer Barrier
		*/
		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		drawBarrier(1.0f, 360);
		glPopMatrix();

		/*
		Inner Barrier
		*/
		glPushMatrix();
		glRotatef(barrier.Rotate, 0.0, 0.0, 1.0);
		glColor3d(0.0, 1.0, 0.0);
		drawBarrier(0.3, barrier.AngleBetween);
		glPopMatrix();

		/*
		Protect her!
		*/
		glPushMatrix();
		glColor4d(1.0f, 0.4f, 0.7f, pow(3, life - 5));
		drawBarrier(0.1, 360);
		glPopMatrix();

		/*
		Enemy attacks
		*/
		for (int i = 0; i < enemy.size(); i++)
		{
			glPushMatrix();
			glRotatef(enemy[i]->Rotate, 0.0, 0.0, 1.0);
			glTranslatef(enemy[i]->Distance, 0.0, 0.0);
			glColor3f(1.0, 0.0, 0.0);
			drawEnemy();
			glPopMatrix();
		}
	}

	else if (level == 3)
	{
		char str[5] = { 0, };
		itoa(score, str, 10);

		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		renderBitmapCharacher(-0.3, 0.3, 0, GLUT_BITMAP_8_BY_13, "You blocked ");
		renderBitmapCharacher(0.0, 0, 0, GLUT_BITMAP_8_BY_13, str);
		renderBitmapCharacher(0.3, -0.3, 0, GLUT_BITMAP_8_BY_13, "attacks");
		renderBitmapCharacher(0.0, -0.7, 0, GLUT_BITMAP_8_BY_13, "Press R key to restart");
		renderBitmapCharacher(0.0, -0.8, 0, GLUT_BITMAP_8_BY_13, "Press ESC key to exit");
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'R':
	case 'r':
		if (level == 3)
		{
			score = 0;
			life = 5;
			level = 2;
		}
		break;
	case 27:
		if (level == 3)
			exit(0);
		break;
	default:
		if (level == 1)
			level = 2;
		break;
	}
}

void timer(int value)
{
	if (level == 2)
	{
		int random = rand() % 360;
		enemy.push_back(new Enemy(enemyspeed, random));
	}

	glutPostRedisplay();
	glutTimerFunc(1000 * interval, timer, 0);
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
	srand(time(0));
	/*
	Initialize game settings
	*/
	gameSet(3);
	 
	barrier.AngleBetween = anglebetween;

	Initialize(argc, argv);

	glutDisplayFunc(display);
	glutSpecialFunc(arrowkey);
	glutSpecialUpFunc(arrowkeyUp);
	glutKeyboardFunc(keyboard);
	//glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(1000, timer, 0);
	glutReshapeFunc(reshape);
	glutMainLoop();

    return 0;
}