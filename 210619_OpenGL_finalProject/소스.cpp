#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "Mesh.h"
#include "gl/glut.h"
#define STB_IMAGE_IMPLEMENTATION
#include "image.h"
#define MAX_WINDOW 800
#define DIMX	255
#define DIMY	256
using namespace std;

void init();
void resize(int w, int h);
void cameraControl();
void lightInit();
void lightControl(int lightSourceType);
void display();
void keyboard(unsigned char key, int x, int y);
void dirKeyboard(int key, int x, int y);
void drawAssistantLine();
void drawName();
void reverseTranslatef(float x, float y, float z);
void reverseRotatef(float angle, float x, float y, float z);
void reverseScalef(float x, float y, float z);
void textureInit(const char* fileName);
unsigned char* LoadMeshFromFile(const char* texFile);
void torusRotateControl();
void torusSegmentControl();
void drawMyTorus(double r, double c, int rSeg, int cSeg);
void drawEgg(double r, double c, int rSeg, int cSeg);
void catControl();

bool isKey1Pressed = false;
float key1CamZdist = 20, key1CamXvec = 0, key1CamRotateAngle = 0;
bool key1CamZdistIncreasing = false;
bool key1CamXvecIncreasing = false;
bool key1CamRotateAngleIncreasing = false;

int lightSourceType = 1;

bool isKey7Pressed = false;
float key7TorusAngle = 0;

bool isKey8Pressed = false;
float key8TorusSegment = 6;
bool isKey8TorusSegmentIncreasing = false;

bool isKey0Pressed = false;
float key0CatPosition = 0;
bool isKey0CatPositionIncreasing = false;
Mesh meshHeart, meshCat;

int w = 50, h = 50;

unsigned char header[54];
unsigned int dataPos;
int width, height;
unsigned int imageSize;

const char* textureFileNameArr[3] = { "bamboo.png", "color3.jpg", "chess.png" };
GLuint texID;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(MAX_WINDOW, MAX_WINDOW);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Final Project");
	init();
	textureInit(textureFileNameArr[lightSourceType]);
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
	meshHeart.LoadMesh("love.obj");
	meshCat.LoadMesh("cat.obj");
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	lightInit();

	cameraControl();
	lightControl(lightSourceType);
	catControl();
	torusRotateControl();
	torusSegmentControl();

	glDisable(GL_TEXTURE_2D);

	if (lightSourceType != 0) {
		drawName();
	}

	glRotatef(key7TorusAngle, 0, 0, 1);
	drawMyTorus(1.5, 6, 4, (int)key8TorusSegment);

	if (lightSourceType == 0) {
		drawEgg(1.5, 6, 4, (int)key8TorusSegment);
	}

	reverseRotatef(key7TorusAngle, 0, 0, 1);

	glutSwapBuffers();
	glutPostRedisplay();
}

void cameraControl() {
	glRotatef(key1CamRotateAngle, 0, 1, 0);
	glTranslatef(0, key1CamZdist / 5, 0);
	gluLookAt(0, 0, key1CamZdist, 0, 0, 0, key1CamXvec, 1, 0);
	reverseTranslatef(0, key1CamZdist / 5, 0);
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

void lightInit() {
	glEnable(GL_LIGHTING);

	GLfloat light0_ambient[] = { 1, 0, 0, 1 };
	GLfloat light0_diffuse[] = { 1, 0, 0, 1 };
	GLfloat light0_specular[] = { 1, 0, 0, 1 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	GLfloat light1_ambient[] = { 0, 0, 1, 1 };
	GLfloat light1_diffuse[] = { 0, 0, 1, 1 };
	GLfloat light1_specular[] = { 0, 0, 1, 1 };

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	GLfloat light2_ambient[] = { 0, 1, 0, 1 };
	GLfloat light2_diffuse[] = { 0, 1, 0, 1 };
	GLfloat light2_specular[] = { 0, 1, 0, 1 };

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
}

void lightControl(int lightSourceType) {
	float lightSource0Position[] = { -20, -10, 20, 1 };
	float lightSource1Position[] = { 20, -10, 20, 1 };
	float lightSource2Position[] = { 0, 10, 20, 1 };

	if (lightSourceType == 0) {
		// Point Light
		glLightfv(GL_LIGHT0, GL_POSITION, lightSource0Position);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0);

		glLightfv(GL_LIGHT1, GL_POSITION, lightSource1Position);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0);

		glLightfv(GL_LIGHT2, GL_POSITION, lightSource2Position);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0);
	}

	if (lightSourceType == 1) {
		// Directional Light
		lightSource0Position[3] = 0;
		glRotatef(45, 1, 0, -1);
		glLightfv(GL_LIGHT0, GL_POSITION, lightSource0Position);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
		reverseRotatef(45, 1, 0, -1);

		lightSource1Position[3] = 0;
		glRotatef(45, -1, 0, -1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightSource1Position);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180);
		reverseRotatef(45, -1, 0, -1);

		lightSource2Position[3] = 0;
		glRotatef(45, -1, 0, -1);
		glLightfv(GL_LIGHT2, GL_POSITION, lightSource2Position);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180);
		reverseRotatef(45, -1, 0, -1);
	}

	if (lightSourceType == 2) {
		// Spot Light
		float spotLight0Direction[] = { 1, 0.5, -1 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightSource0Position);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotLight0Direction);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20);

		float spotLight1Direction[] = { -1, 0.5, -1 };
		glLightfv(GL_LIGHT1, GL_POSITION, lightSource1Position);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotLight1Direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20);

		float spotLight2Direction[] = { 0, -0.5, -1 };
		glLightfv(GL_LIGHT2, GL_POSITION, lightSource2Position);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotLight2Direction);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 20);
	}

	GLfloat material_ambient[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat material_diffuse[] = { 1, 1, 1, 1 };
	GLfloat material_specular[] = { 0, 0, 0, 1 };
	GLfloat material_shininess[] = { 5 };
	GLfloat material_emission[] = { 0.2, 0.2, 0.2, 1 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		isKey1Pressed = !isKey1Pressed;
	}

	if (key == '3') {
		lightSourceType = 0;
		textureInit(textureFileNameArr[0]);
	}

	if (key == '4') {
		lightSourceType = 1;
		textureInit(textureFileNameArr[1]);
	}

	if (key == '5') {
		lightSourceType = 2;
		textureInit(textureFileNameArr[2]);
	}

	if (key == '7') {
		isKey7Pressed = !isKey7Pressed;
	}

	if (key == '8') {
		isKey8Pressed = !isKey8Pressed;
	}

	if (key == '0') {
		isKey0Pressed = !isKey0Pressed;
	}
}

void dirKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_HOME) {
		isKey1Pressed = false;
		key1CamZdist = 20;
		key1CamXvec = 0;
		key1CamRotateAngle = 0;

		isKey7Pressed = false;
		key7TorusAngle = 0;

		isKey8Pressed = false;
		key8TorusSegment = 6;

		isKey0Pressed = false;
		key0CatPosition = 0;
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

void textureInit(const char* fileName) {
	unsigned char* bitmap;
	bitmap = LoadMeshFromFile((char*)fileName);

	glGenTextures(1, &texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

unsigned char* LoadMeshFromFile(const char* texFile) {
	FILE* fp = NULL;

	if (fopen_s(&fp, texFile, "rb")) {
		printf("ERROR : No %s. \n fail to bind %d\n", texFile);
		return NULL;
	}

	int channel;
	unsigned char* image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	return image;
}

void torusRotateControl() {
	if (isKey7Pressed) {
		key7TorusAngle += 0.2;

		if (key7TorusAngle > 360) {
			key7TorusAngle = 0;
		}
	}
}

void torusSegmentControl() {
	if (isKey8Pressed) {
		if (isKey8TorusSegmentIncreasing) {
			key8TorusSegment += 0.01;

			if (key8TorusSegment > 10) {
				isKey8TorusSegmentIncreasing = false;
			}
		}

		else {
			key8TorusSegment -= 0.01;

			if (key8TorusSegment < 3) {
				isKey8TorusSegmentIncreasing = true;
			}
		}
	}
}

void drawMyTorus(double r, double c, int rSeg, int cSeg) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const double PI = 3.1415926;
	const double TAU = 2 * PI;

	for (int i = 0; i < rSeg; i++) {
		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j <= cSeg; j++) {
			for (int k = 0; k <= 1; k++) {
				double s = (i + k) % rSeg + 0.5;
				double t = j % (cSeg + 1);

				double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
				double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
				double z = r * sin(s * TAU / rSeg);

				double u = (i + k) / (float)rSeg;
				double v = t / (float)cSeg;

				glTexCoord2d(u, v);
				glNormal3f(2 * x, 2 * y, 2 * z);
				glVertex3d(2 * x, 2 * y, 2 * z);
			}
		}

		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}

void drawEgg(double r, double c, int rSeg, int cSeg) {
	const double PI = 3.1415926;
	const double TAU = 2 * PI;

	for (int j = 0; j <= cSeg; j++) {
		double s = 0.5;
		double t = j % (cSeg + 1);

		double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
		double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
		double z = r * sin(s * TAU / rSeg);

		glTranslatef(0.7 * x, 0.7 * y, 0.7 * z);
		glutSolidSphere(2, 10, 10);
		reverseTranslatef(0.7 * x, 0.7 * y, 0.7 * z);

		if (j == 0) {
			glTranslatef(1.8 * x - key0CatPosition, 1.8 * y, 1.8 * z);

			if (isKey0CatPositionIncreasing) {
				glRotatef(180, 0, 0, 1);
			}

			glRotatef(90, 1, 0, 0);
			meshCat.RenderMesh(VECTOR3D(1, 0.4, 0.4), 0.01);
			reverseRotatef(90, 1, 0, 0);

			if (isKey0CatPositionIncreasing) {
				reverseRotatef(180, 0, 0, 1);
			}

			reverseTranslatef(1.8 * x - key0CatPosition, 1.8 * y, 1.8 * z);
		}
	}
}

void catControl() {
	if (isKey0Pressed) {
		if (isKey0CatPositionIncreasing) {
			key0CatPosition += 0.2;

			if (key0CatPosition > 25) {
				isKey0CatPositionIncreasing = false;
			}
		}

		else {
			key0CatPosition -= 0.2;

			if (key0CatPosition < 0) {
				isKey0CatPositionIncreasing = true;
			}
		}
	}
}