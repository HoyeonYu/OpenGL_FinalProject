#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "gl/glut.h"
#define MAX_WINDOW 800
using namespace std;

void init();
void resize(int w, int h);
void cameraControl();
void display();
void keyboard(unsigned char key, int x, int y);
void dirKeyboard(int key, int x, int y);
void drawAssistantLine();
void drawName();
void reverseTranslatef(float x, float y, float z);
void reverseRotatef(float angle, float x, float y, float z);
void reverseScalef(float x, float y, float z);

bool isKey1Pressed = false;
float key1CamZdist = 20, key1CamXvec = 0, key1CamRotateAngle = 0;
bool key1CamZdistIncreasing = false;
bool key1CamXvecIncreasing = false;
bool key1CamRotateAngleIncreasing = false;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(MAX_WINDOW, MAX_WINDOW);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Final Project");
	init();
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(dirKeyboard);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}

void init() {
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90, 1, 5, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cameraControl();


	drawAssistantLine();
	drawName();

	glutSwapBuffers();
	glutPostRedisplay();
}

void cameraControl() {
	glRotatef(key1CamRotateAngle, 0, 1, 0);
	gluLookAt(key1CamRotateAngle / 2, 0, key1CamZdist, 0, 0, 0, key1CamXvec, 1, 0);
	reverseRotatef(key1CamRotateAngle, 0, 1, 0);

	if (isKey1Pressed) {
		if (key1CamZdistIncreasing) {
			key1CamZdist += 0.1;

			if (key1CamZdist > 20) {
				key1CamZdistIncreasing = false;
			}

			if (key1CamRotateAngleIncreasing) {
				key1CamRotateAngle += 0.5;

				if (key1CamRotateAngle > 20) {
					key1CamRotateAngleIncreasing = false;
				}
			}

			else {
				key1CamRotateAngle -= 0.5;

				if (key1CamRotateAngle < -20) {
					key1CamRotateAngleIncreasing = true;
				}
			}
		}

		else {
			key1CamZdist -= 0.1;

			if (key1CamZdist < 10) {
				key1CamZdistIncreasing = true;
			}

			if (key1CamRotateAngleIncreasing) {
				key1CamRotateAngle += 0.5;

				if (key1CamRotateAngle > 20) {
					key1CamRotateAngleIncreasing = false;
				}
			}

			else {
				key1CamRotateAngle -= 0.5;

				if (key1CamRotateAngle < -20) {
					key1CamRotateAngleIncreasing = true;
				}
			}
		}

		if (key1CamXvecIncreasing) {
			key1CamXvec += 0.02;

			if (key1CamXvec > 0.5) {
				key1CamXvecIncreasing = false;
			}
		}

		else {
			key1CamXvec -= 0.02;

			if (key1CamXvec < -0.5) {
				key1CamXvecIncreasing = true;
			}
		}
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		isKey1Pressed = !isKey1Pressed;
	}
}

void dirKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_HOME) {
		isKey1Pressed = false;
		key1CamZdist = 20;
		key1CamXvec = 0;
		key1CamRotateAngle = 0;
	}
}

void drawAssistantLine() {
	// x
	// Red: x(-)
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-5, 0, 0);
	glVertex3f(0, 0, 0);
	// Magenta: x(+)
	glColor3f(1, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(5, 0, 0);

	//y
	// Green: y(-)
	glColor3f(0, 1, 0);
	glVertex3f(0, -5, 0);
	glVertex3f(0, 0, 0);
	// Yellow: y(+)
	glColor3f(1, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 5, 0);

	//z
	// Blue: z(-)
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -5);
	glVertex3f(0, 0, 0);
	// Cyan: z(+)
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 5);

	glEnd();
}

void drawName() {
	{	// È£
		const float firstCenterX = -3.5;
		const float firstCenterY = 0.5;
		const float firstCenterZ = 0;

		glTranslatef(firstCenterX, firstCenterY + 2, firstCenterZ);
		glRotatef(-90, 1, 0, 0);
		glColor3f(1, 1, 1);
		glutSolidCone(1, 1, 30, 30);
		glColor3f(0, 0, 0);
		glutWireCone(1, 1, 30, 30);
		reverseRotatef(-90, 1, 0, 0);
		reverseTranslatef(firstCenterX, firstCenterY + 2, firstCenterZ);

		glTranslatef(firstCenterX, firstCenterY + 1.5, firstCenterZ);
		glScalef(3, 0.5, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(3, 0.5, 0.5);
		reverseTranslatef(firstCenterX, firstCenterY + 1.5, firstCenterZ);

		glTranslatef(firstCenterX, firstCenterY, firstCenterZ);
		glColor3f(1, 1, 1);
		glutSolidSphere(1, 30, 30);
		glColor3f(0, 0, 0);
		glutWireSphere(1, 30, 30);
		reverseTranslatef(firstCenterX, firstCenterY, firstCenterZ);

		glTranslatef(firstCenterX, -1.25, firstCenterZ);
		glScalef(0.5, 1, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(0.5, 1, 0.5);
		reverseTranslatef(firstCenterX, -1.25, firstCenterZ);

		glTranslatef(firstCenterX, -2, firstCenterZ);
		glScalef(4.5, 0.5, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(4.5, 0.5, 0.5);
		reverseTranslatef(firstCenterX, -2, firstCenterZ);
	}

	{	// ¿¬
		const float secondCenterX = 2;
		const float secondCenterY = 1.5;
		const float secondCenterZ = 0;

		glTranslatef(secondCenterX, secondCenterY, secondCenterZ);
		glColor3f(1, 1, 1);
		glutSolidSphere(1, 30, 30);
		glColor3f(0, 0, 0);
		glutWireSphere(1, 30, 30);
		reverseTranslatef(secondCenterX, secondCenterY, secondCenterZ);

		glTranslatef(secondCenterX + 2, secondCenterY + 0.5, secondCenterZ);
		glScalef(1.5, 0.5, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(1.5, 0.5, 0.5);
		reverseTranslatef(secondCenterX + 2, secondCenterY + 0.5, secondCenterZ);

		glTranslatef(secondCenterX + 2, secondCenterY - 0.5, secondCenterZ);
		glScalef(1.5, 0.5, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(1.5, 0.5, 0.5);
		reverseTranslatef(secondCenterX + 2, secondCenterY - 0.5, secondCenterZ);

		glTranslatef(secondCenterX + 2.5, secondCenterY - 0.25, secondCenterZ);
		glScalef(0.5, 4, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(0.5, 4, 0.5);
		reverseTranslatef(secondCenterX + 2.5, secondCenterY - 0.25, secondCenterZ);

		glTranslatef(secondCenterX, secondCenterY - 3, secondCenterZ);
		glScalef(0.5, 1.5, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(0.5, 1.5, 0.5);
		reverseTranslatef(secondCenterX, secondCenterY - 3, secondCenterZ);

		glTranslatef(secondCenterX + 1.4375, secondCenterY - 3.5, secondCenterZ);
		glScalef(3.375, 0.5, 0.5);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);
		reverseScalef(3.375, 0.5, 0.5);
		reverseTranslatef(secondCenterX + 1.4375, secondCenterY - 3.5, secondCenterZ);

	}
}

void reverseTranslatef(float x, float y, float z) {
	glTranslatef(-x, -y, -z);
}

void reverseRotatef(float angle, float x, float y, float z) {
	glRotatef(-angle, x, y, z);
}

void reverseScalef(float x, float y, float z) {
	glScalef(1 / x, 1 / y, 1 / z);
}