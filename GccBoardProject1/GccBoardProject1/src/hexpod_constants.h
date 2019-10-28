#ifndef HEXAPOD_CONSTANTS_H
#define HEXAPOD_CONSTANTS_H
#include "vector.h"

extern float baseSideLength;
extern float platformSideLength;
extern float rodLength;
extern float hornLength;
extern float basePositionKoeff;
extern float platPositionKoeff;

extern struct Vector Ab, Bb, Cb;
extern struct Vector Ap, Bp, Cp;

extern struct Vector motorPositions[6];
extern struct Vector anchorPositions[6];
extern float hornRotations[6];

extern void initialize(
	float, 
	float, 
	float, 
	float,
	float,
	float);

#endif