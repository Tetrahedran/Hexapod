#ifndef HEXAPOD_H
#define HEXAPOD_H
#include "vector.h"
#include "quaternion.h"

extern float* calcMotorAngles(struct Vector absTranslation, struct Quaternion absRotation);

#endif