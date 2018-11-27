#include <stdio.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <random>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <math.h>
#include <vector>
#include "car.h"

using namespace std;

car::car() {

	for (int i = 0; i < 3; i++) {
		speed.push_back(0.0);
	}

	for (int i = 0; i < 3; i++) {
		angle.push_back(0.0);
	}

	for (int i = 0; i < 3; i++) {
		carEye.push_back(EYE_START[i]);
	}

	for (int i = 0; i < 3; i++) {
		carFocus.push_back(FOCUS_START[i]);
	}

	for (int i = 0; i < 3; i++) {
		pos.push_back(OBJ_START[i]);
	}

}

car::car(vector<float> inPos, vector<float> inAngle) {

	for (int i = 0; i < 3; i++) {		// set initial speed (0)
		speed.push_back(0.0);
	}

	for (int i = 0; i < 3; i++) {		// set initial angle
		angle.push_back(inAngle[i]);
	}

	for (int i = 0; i < 3; i++) {
		carEye.push_back(EYE_START[i]);
	}

	for (int i = 0; i < 3; i++) {
		carFocus.push_back(FOCUS_START[i]);
	}

	for (int i = 0; i < 3; i++) {		// set initial position
		pos.push_back(inPos[i]);
	}

}

void car::printTest() {
	cout << angle.size() << endl;
}