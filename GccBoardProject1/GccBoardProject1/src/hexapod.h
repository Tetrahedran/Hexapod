#ifndef HEXAPOD_H
#define HEXAPOD_H
#include "vector.h"
#include "quaternion.h"
#include "hexpod_constants.h"

extern float* calcMotorAngles(struct Vector absTranslation, struct Quaternion absRotation);

#endif