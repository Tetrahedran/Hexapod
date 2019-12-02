#ifndef HEXAPOD_H
#define HEXAPOD_H
#include "vector.h"
#include "quaternion.h"
#include "hexpod_constants.h"

extern void calcMotorAngles(float* angles, struct Vector absTranslation, struct Quaternion absRotation);

#endif