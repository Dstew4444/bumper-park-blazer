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
	vector<float> pos;
	vector<float> speed;
    vector<float> angle;

	for (int i = 0; i < 3; i++) {
		pos.push_back(0.0);
	}

	for (int i = 0; i < 3; i++) {
		speed.push_back(0.0);
	}

	for (int i = 0; i < 3; i++) {
		angle.push_back(0.0);
	}

}

car::car(vector<float> inPos, vector<float> inAngle) {
    vector<float> pos;
    vector<float> speed;
    vector<float> angle;

	for (int i = 0; i < 3; i++) {		// set initial position
		pos.push_back(inPos[i]);
	}

	for (int i = 0; i < 3; i++) {		// set initial speed (0)
		pos.push_back(0.0);
	}

	for (int i = 0; i < 3; i++) {		// set initial angle
		angle.push_back(inAngle[i]);
	}

}