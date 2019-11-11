#include "acceleration.h"
#include <math.h>

float accToAngle(float acc);

const float g = 9.81f;
const float maxAcc = 0.70710678118f * g;

struct Quaternion accelerationsToAngles(float accelerationX, float accelerationY) {
	float angleX = accToAngle(accelerationX);
	float angleY = accToAngle(accelerationX);
	
	struct Quaternion qX = quatForRot((struct Vector) {1.0f, 0.0f, 0,0f}, angleX);
	struct Quaternion qY = quatForRot((struct Vector) {0.0f, 1.0f, 0,0f}, angleY);

	return hamiltonP(qX, qY);
}

float accToAngle(float acc) {
	if (acc > maxAcc) acc = maxAcc;
	float sin = acc / g;
	return asinf(sin);
}