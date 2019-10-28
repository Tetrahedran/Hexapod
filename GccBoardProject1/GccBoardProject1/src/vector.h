#ifndef VECTOR_H
#define VECTOR_H
#include "structs.h"


extern struct Vector convQuatToVec(struct Quaternion);

extern struct Vector multiply(struct Vector, float);
extern struct Vector add(struct Vector, struct Vector);
extern struct Vector subtract(struct Vector, struct Vector);
extern struct Vector cross(struct Vector, struct Vector);
extern float dot(struct Vector, struct Vector);
extern float magnitude(struct Vector);	

extern struct Vector rotate(struct Vector, struct Quaternion);

#endif