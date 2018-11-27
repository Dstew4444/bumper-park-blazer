#ifndef car_h
#define car_h
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>

using namespace std;

class car {
public:
	car();
    car(vector<float> inPos, vector<float> inAngle);

    vector<float> pos;
    vector<float> speed;
    vector<float> angle;
    vector<float> carEye;
    vector<float> carFocus;
    const float CUBE_CAM_OFFSET = 5;
	const vector<float> EYE_START = {-5.0, 3.0, -5.0};
	const vector<float> FOCUS_START = {8.0, 0.0, 8.0};
	// second argument should be CUBE_SIZE/2 not 1 but that's a global constant so I'll break the no hard coding commandment for now
	const vector<float> OBJ_START = {EYE_START[0] + CUBE_CAM_OFFSET, 1, EYE_START[2] + CUBE_CAM_OFFSET};

	void printTest();

};
#endif