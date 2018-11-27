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

    

};
#endif