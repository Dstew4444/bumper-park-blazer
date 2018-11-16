#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <unistd.h>
#include <algorithm>

using namespace std;

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
const float EYE_X_START = -5;
const float EYE_Z_START = -5;
const float FOCUS_X_START = 50;
const float FOCUS_Z_START = 50;
float eye[3] = {EYE_X_START, CUBE_SIZE * 2, EYE_Z_START};
float focus[3] = {FOCUS_X_START, 0, FOCUS_Z_START};
float objPos[3] = {0, 0, 0};		// possibly not needed

// BASIC SHITTY PHYSICS! YES!
float speedX = 0.0;
float speedZ = 0.0;

// light source 1 
float pos0[4] = {50, 0, 50, 0};
float amb0[4] = {0, 0, 1, 1};
float dif0[4] = {0, 0, 1, 1};
float spec0[4] = {0, 0, 1, 1};

// light source 2
float pos1[4] = {-50, 0, -50, 0};
float amb1[4] = {1, 0, 0, 1};
float dif1[4] = {1, 0, 0, 1};
float spec1[4] = {1, 0, 0, 1};

// rotation angles
float angleX = 0;
float angleY = 0;
float angleZ = 0;

// bullet test
float bulletDist = 0;
bool bulletFired = false;
const float BULLET_SIZE = 3;
const float BULLET_SPEED = 10;

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
		// case 'w':
		// 	eye[2] += CAM_MOVE_SPEED;
		// 	focus[2] += CAM_MOVE_SPEED;
		// 	break;
		// case 's':
		// 	eye[2] -= CAM_MOVE_SPEED;
		// 	focus[2] -= CAM_MOVE_SPEED;
		// case 'd':
		// 	eye[0] += CAM_MOVE_SPEED;
		// 	focus[0] += CAM_MOVE_SPEED;
		// 	break;
		// case 'a':
		// 	eye[0] -= CAM_MOVE_SPEED;
		// 	focus[0] -= CAM_MOVE_SPEED;
		// 	break;
		case 'w':
			speedZ += CAM_MOVE_SPEED;
			printf("Speed Z: %f\n", speedZ);
			break;
		case 's':
			speedZ -= CAM_MOVE_SPEED;
			printf("Speed Z: %f\n", speedZ);
			break;
		case 'd':
			speedX += CAM_MOVE_SPEED;
			printf("Speed X: %f\n", speedX);
			break;
		case 'a':
			speedX -= CAM_MOVE_SPEED;
			printf("Speed X: %f\n", speedX);
			break;
		case 'r':
			eye[1] += CAM_MOVE_SPEED;
			focus[1] += CAM_MOVE_SPEED;
			break;
		case 'f':
			eye[1] -= CAM_MOVE_SPEED;
			focus[1] -= CAM_MOVE_SPEED;
			break;
		case ' ':
			speedX = 0;
			speedZ = 0;
			printf("EMERGENCY MAGIC BRAKE! Press 'x' to reset.\n");
			break;
		case 'x':
			eye[0] = EYE_X_START;
			eye[2] = EYE_Z_START;
			focus[0] = FOCUS_X_START;
			focus[2] = FOCUS_Z_START;
			speedX = 0;
			speedZ = 0;
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
	gluLookAt(eye[0], eye[1], eye[2], focus[0], focus[1], focus[2], 0, 1, 0);
	drawAxis();

	glPushMatrix();
		glRotatef(angleX, 1, 0, 0);
		glRotatef(angleY, 0, 1, 0);
		glRotatef(angleZ, 0, 0, 1);
		glBegin(GL_POLYGON);
			glVertex3f(-MAP_SIZE, 0, -MAP_SIZE);
			glVertex3f(-MAP_SIZE, 0, MAP_SIZE);
			glVertex3f(MAP_SIZE, 0, MAP_SIZE);
			glVertex3f(MAP_SIZE, 0, -MAP_SIZE);
		glEnd();
		glPushMatrix();
			glTranslatef(eye[0] + 5, eye[1], eye[2] + 5);
			glutSolidCube(CUBE_SIZE);
		glPopMatrix();
	glPopMatrix();
	eye[0] += speedX;
	focus[0] += speedX;
	eye[2] += speedZ;
	focus[2] += speedZ;

	glutSwapBuffers();
}

void special(int key, int x, int y) {	// camera rotations
	switch(key)
	{
		case GLUT_KEY_DOWN:
			angleX -= CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_UP:
			angleX += CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_LEFT:
			angleY -= CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_RIGHT:
			angleY += CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_PAGE_DOWN:
			angleZ -= CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_PAGE_UP:
			angleZ += CAM_ROTATE_SPEED;
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
