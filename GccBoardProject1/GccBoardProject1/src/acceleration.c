#include "acceleration.h"
#include "asf.h"
#include <math.h>

float accToAngle(float acc);

const float g = 9.81f;
const float maxAcc = 0.70710678118f * 9.81f;

struct Quaternion accelerationsToAngles(float accelerationX, float accelerationY) {
	struct Vector acc = (struct Vector){accelerationX, accelerationY, 0};
	float mag = magnitude(acc);
	if(mag > maxAcc) 
	{
		acc = multiply(acc, (maxAcc / mag));
	}
	float angleX = accToAngle(acc.x);
	float angleY = accToAngle(acc.y);
	
	//Beschleunigung in x-Richtung benötigt Drehung um y-Achse
	struct Quaternion qX = quatForRot((struct Vector) {0.0f, 1.0f, 0.0f}, angleX);
		
	//Beschleunigung in y-Richtung benötigt Drehung um x-Achse
	struct Quaternion qY = quatForRot((struct Vector) {1.0f, 0.0f, 0.0f}, angleY);

	return hamiltonP(qX, qY);
}

float accToAngle(float acc) {
	float absAcc = acc > 0 ? acc : -acc;
	uint8_t positive = acc > 0 ? 1 : 0; 
	float sin = absAcc / g;
	return asinf(sin) * (positive == 1 ? 1 : -1);
}