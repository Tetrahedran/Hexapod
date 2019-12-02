#ifndef ACCELERATION_H_
#define ACCELERATION_H_
#include "vector.h"
#include "quaternion.h"

#define g 9.81f
#define maxAcc (0.70710678118f * 9.81f * 0.9f)


struct Quaternion accelerationsToAngles(float accelerationX, float accelerationY);

#endif