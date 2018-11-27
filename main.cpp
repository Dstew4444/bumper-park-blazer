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
const float CUBE_CAM_OFFSET = 2;
const float EYE_START[3] = {-5, 3, -5};		// -5, 3, 5 and 8, 0, 8 for FOCUS_START seem to be a good "third person" view
const float FOCUS_START[3] = {8, 0, 8};
const float OBJ_POS_START[3] = {EYE_START[0] + CUBE_CAM_OFFSET, CUBE_SIZE/2, EYE_START[2] + CUBE_CAM_OFFSET};
float eye[3];		// initialized to EYE_START in init function
float focus[3];		// initialized to FOCUS_START in init function
float objPos[3];

// BASIC SHITTY PHYSICS! YES!
float speed[3] = {0, 0, 0};

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
float angle[3] = {0, 0, 0};

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
			speed[2] += CAM_MOVE_SPEED;
			printf("Speed Z: %f\n", speed[2]);
			break;
		case 's':
			speed[2] -= CAM_MOVE_SPEED;
			printf("Speed Z: %f\n", speed[2]);
			break;
		case 'd':
			speed[0] += CAM_MOVE_SPEED;
			printf("Speed X: %f\n", speed[0]);
			break;
		case 'a':
			speed[0] -= CAM_MOVE_SPEED;
			printf("Speed X: %f\n", speed[0]);
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
			speed[0] = 0;
			speed[2] = 0;
			printf("EMERGENCY MAGIC BRAKE! Press 'x' to reset.\n");
			break;
		case 'x':
			eye[0] = EYE_START[0];
			eye[2] = EYE_START[2];
			focus[0] = FOCUS_START[0];
			focus[2] = FOCUS_START[2];
			objPos[0] = OBJ_POS_START[0];
			objPos[2] = OBJ_POS_START[2];
			speed[0] = 0;
			speed[2] = 0;
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

	// initializes arrays to start values
	for (int i = 0; i < sizeof(eye)/sizeof(eye[0]); i++) {
		eye[i] = EYE_START[i];
	}
	for (int  i = 0; i < sizeof(focus)/sizeof(focus[0]); i++) {
		focus[i] = FOCUS_START[i];
	}
	for (int  i = 0; i < sizeof(objPos)/sizeof(objPos[0]); i++) {
		objPos[i] = OBJ_POS_START[i];
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// movable camera
	gluLookAt(eye[0], eye[1], eye[2], focus[0], focus[1], focus[2], 0, 1, 0);
	drawAxis();

	// glPushMatrix();
	// 	glRotatef(angle[0], 1, 0, 0);
	// 	glRotatef(angle[1], 0, 1, 0);
	// 	glRotatef(angle[2], 0, 0, 1);
	// 	glBegin(GL_POLYGON);
	// 		glVertex3f(-MAP_SIZE, 0, -MAP_SIZE);
	// 		glVertex3f(-MAP_SIZE, 0, MAP_SIZE);
	// 		glVertex3f(MAP_SIZE, 0, MAP_SIZE);
	// 		glVertex3f(MAP_SIZE, 0, -MAP_SIZE);
	// 	glEnd();
	// 	// cube
	// 	glPushMatrix();
	// 		glTranslatef(objPos[0], objPos[1], objPos[2]);
	// 		glutSolidCube(CUBE_SIZE);
	// 	glPopMatrix();
	// glPopMatrix();

		glBegin(GL_POLYGON);
			glVertex3f(-MAP_SIZE, 0, -MAP_SIZE);
			glVertex3f(-MAP_SIZE, 0, MAP_SIZE);
			glVertex3f(MAP_SIZE, 0, MAP_SIZE);
			glVertex3f(MAP_SIZE, 0, -MAP_SIZE);
		glEnd();
		// cube
		glPushMatrix();
			glRotatef(angle[0], 1, 0, 0);
			glRotatef(angle[1], 0, 1, 0);
			glRotatef(angle[2], 0, 0, 1);
			glTranslatef(objPos[0], objPos[1], objPos[2]);
			glutSolidCube(CUBE_SIZE);
		glPopMatrix();

	eye[0] += speed[0];
	focus[0] += speed[0];
	objPos[0] += speed[0];
	eye[2] += speed[2];
	focus[2] += speed[2];
	objPos[2] += speed[2];

	glutSwapBuffers();
}

void special(int key, int x, int y) {	// camera rotations
	switch(key)
	{
		case GLUT_KEY_DOWN:
			angle[0] -= CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_UP:
			angle[0] += CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_LEFT:
			angle[1] -= CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_RIGHT:
			angle[1] += CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_PAGE_DOWN:
			angle[2] -= CAM_ROTATE_SPEED;
			break;
		case GLUT_KEY_PAGE_UP:
			angle[2] += CAM_ROTATE_SPEED;
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
