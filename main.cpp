#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include "car.h"

using namespace std;

car test;

// window size and initial position
const int INITIAL_X_POS = 600;
const int INITIAL_Y_POS = 300;
const int MAX_X = 400;
const int MAX_Y = 400;

const float MAP_SIZE = 50.0;
const float CUBE_SIZE = 2.0;

// camera
const float CAM_MOVE_SPEED = 0.01;		// adjust based on how shitty your computer is
const float CAM_ROTATE_SPEED = 10.0;
const float FAR_PLANE_DIST = 1000.0;
// vector<float> eye;			// initialized to EYE_START in init function
// vector<float> focus;		// initialized to FOCUS_START in init function

// light source 1 
float pos0[4] = {50, 0, 50, 0};
float amb0[4] = {1, 0, 0, 1};
float dif0[4] = {1, 0, 0, 1};
float spec0[4] = {1, 0, 0, 1};

// light source 2
float pos1[4] = {-50, 0, -50, 0};
float amb1[4] = {0, 0, 0, 1};
float dif1[4] = {0, 0, 0, 1};
float spec1[4] = {0, 0, 0, 1};

// bullet test
float bulletDist = 0;
bool bulletFired = false;
const float BULLET_SIZE = 3;
const float BULLET_SPEED = 10;

float degToRad(float x) {
	return x * M_PI/180;
}

void drawAxis() {
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
		glVertex3f(-FAR_PLANE_DIST, 0, 0);
		glVertex3f(FAR_PLANE_DIST, 0, 0);
		glVertex3f(0, -FAR_PLANE_DIST, 0);
		glVertex3f(0, FAR_PLANE_DIST, 0);
		glVertex3f(0, 0, -FAR_PLANE_DIST);
		glVertex3f(0, 0, FAR_PLANE_DIST);
	glEnd();
}

void keyboard(unsigned char key, int x, int y) {
	int mod = glutGetModifiers();
	switch (key)
	{
		case 'q':		// press q to quit
			exit(0);
			break;
		case 'w':
			test.speed[2] += CAM_MOVE_SPEED;
			printf("Speed Z: %f\n", test.speed[2]);
			break;
		case 's':
			test.speed[2] -= CAM_MOVE_SPEED;
			printf("Speed Z: %f\n", test.speed[2]);
			break;
		case 'd':
			test.speed[0] += CAM_MOVE_SPEED;
			printf("Speed X: %f\n", test.speed[0]);
			break;
		case 'a':
			test.speed[0] -= CAM_MOVE_SPEED;
			printf("Speed X: %f\n", test.speed[0]);
			break;
		case 'r':
			test.carEye[1] += CAM_MOVE_SPEED;
			test.carFocus[1] += CAM_MOVE_SPEED;
			break;
		case 'f':
			test.carEye[1] -= CAM_MOVE_SPEED;
			test.carFocus[1] -= CAM_MOVE_SPEED;
			break;
		case ' ':
			test.speed[0] = 0;
			test.speed[2] = 0;
			printf("EMERGENCY MAGIC BRAKE! Press 'x' to reset.\n");
			break;
		case 'x':
			test.carEye[0] = test.EYE_START[0];
			test.carEye[2] = test.EYE_START[2];
			test.carFocus[0] = test.FOCUS_START[0];
			test.carFocus[2] = test.FOCUS_START[2];
			test.pos[0] = test.OBJ_START[0];
			test.pos[2] = test.OBJ_START[2];
			test.speed[0] = 0;
			test.speed[2] = 0;
			printf("RESET!\n");
			break;
	}
	glutPostRedisplay();
}

void init() {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45, 1, 1, FAR_PLANE_DIST);
 	// glFrontFace(GL_CCW);
	// glCullFace(GL_BACK);
	// glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
	glEnable(GL_LIGHT1);

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// movable camera
	gluLookAt(test.carEye[0], test.carEye[1], test.carEye[2], test.carFocus[0], test.carFocus[1], test.carFocus[2], 0, 1, 0);
	drawAxis();

		glBegin(GL_POLYGON);
			glVertex3f(-MAP_SIZE, 0, -MAP_SIZE);
			glVertex3f(-MAP_SIZE, 0, MAP_SIZE);
			glVertex3f(MAP_SIZE, 0, MAP_SIZE);
			glVertex3f(MAP_SIZE, 0, -MAP_SIZE);
		glEnd();
		// cube
		glPushMatrix();
			glRotatef(test.angle[0], 1, 0, 0);
			glRotatef(test.angle[1], 0, 1, 0);
			glRotatef(test.angle[2], 0, 0, 1);
			glTranslatef(test.pos[0], test.pos[1], test.pos[2]);
			glutSolidCube(CUBE_SIZE);
		glPopMatrix();

	test.carEye[0] += test.speed[0] * cos(degToRad(test.angle[0]));
	test.carFocus[0] += test.speed[0] * cos(degToRad(test.angle[0]));
	test.pos[0] += test.speed[0] * cos(degToRad(test.angle[0]));
	test.carEye[2] += test.speed[2] * cos(degToRad(test.angle[2]));
	test.carFocus[2] += test.speed[2] * cos(degToRad(test.angle[2]));
	test.pos[2] += test.speed[2] * cos(degToRad(test.angle[2]));

	//test.carEye[0] = test.pos[0] - 5 * sin(degToRad(test.angle[0]));


	glutSwapBuffers();
}

void special(int key, int x, int y) {	// camera rotations
	switch(key)
	{
		case GLUT_KEY_DOWN:
			test.angle[0] -= CAM_ROTATE_SPEED;
			printf("Angle X: %f\n", test.angle[0]);
			break;
		case GLUT_KEY_UP:
			test.angle[0] += CAM_ROTATE_SPEED;
			printf("Angle X: %f\n", test.angle[0]);
			break;
		case GLUT_KEY_LEFT:
			test.angle[1] -= CAM_ROTATE_SPEED;
			printf("Angle Y: %f\n", test.angle[1]);
			break;
		case GLUT_KEY_RIGHT:
			test.angle[1] += CAM_ROTATE_SPEED;
			printf("Angle Y: %f\n", test.angle[1]);
			break;
		case GLUT_KEY_PAGE_DOWN:
			test.angle[2] -= CAM_ROTATE_SPEED;
			printf("Angle Z: %f\n", test.angle[2]);
			break;
		case GLUT_KEY_PAGE_UP:
			test.angle[2] += CAM_ROTATE_SPEED;
			printf("Angle Z: %f\n", test.angle[2]);
			break;
    }
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {

}

void reshape(int w, int h) {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((w+0.0f)/h), 1, FAR_PLANE_DIST);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

void FPS(int val) {
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
}

void callBackInit() {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, FPS, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);		
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(MAX_X, MAX_Y);
	glutInitWindowPosition(INITIAL_X_POS, INITIAL_Y_POS);

	glutCreateWindow("Main");	
	
	srand((unsigned) time(NULL) * getpid());

	init();
	callBackInit();
	glutMainLoop();

	return(0);					
}
