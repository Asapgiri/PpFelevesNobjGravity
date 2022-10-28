#pragma once
#include "vector.h"

#define CIRCLE_SIDES		120
#define CIRCLE_VERTICES		122
#define ALL_CIRCLE_VERTICES	366

class MyObj {
public:
	Vector pos;	// x, y, z

	double m;	// mass  [g]
	double s;	// size  [mm]

	Vector v;	// speed [m/s]
	Vector F;	// Origin

	Vector e;	// temp for other objects location
	double d;	// temp for distance between objects

	int i;		// temp for for;

	float* allCircleVertices;
};