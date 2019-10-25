#ifndef QUATERNION_H
#define  QUATERNION_H
#include "structs.h"

extern struct Quaternion quatForRot(struct Vector, float);
extern struct Quaternion convVecToQuat(struct Vector);

extern struct Quaternion conjugate(struct Quaternion);
extern struct Quaternion hamiltonP(struct Quaternion, struct Quaternion);

#endif