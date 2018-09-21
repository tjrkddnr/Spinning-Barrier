// Spinning Barrier.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <stdio.h>
#include "gl_helper.h"
#include "Init.h"

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void arrowkey(int key, int x, int y)
{
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	Initialize(argc, argv);

	glutDisplayFunc(display);
	glutSpecialFunc(arrowkey);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

    return 0;
}

