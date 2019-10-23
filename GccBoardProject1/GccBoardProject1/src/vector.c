#include "vector.h"

struct Vector convQuatToVec(struct Quaternion quat)
{
	struct Vector vec = {quat.i, quat.j, quat.k};
	return vec;
}

struct Vector multiply(struct Vector original, float skalar)
{
	struct Vector ret = {original.x * skalar, original.y * skalar, original.z * skalar};
	return ret;
}

struct Vector add(struct Vector first, struct Vector second)
{
	struct Vector ret = {first.x + second.x, first.y + second.y, first.z + second.z};
	return ret;
}

struct Vector subtract(struct Vector first, struct Vector second)
{
	return add(first, multiply(second, -1f));
}

struct Vector cross(struct Vector lhs, struct Vector rhs)
{
	struct Vector ret = {
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x	
	};
	return ret;
}

float dot(struct Vector lhs, struct Vector rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float magnitude(struct Vector vector)
 {
	 return sqrtf(dot(vector, vector));
 }
 
 struct Vector rotate(struct Vector vec, struct Quaternion rotation)
 {
	 struct Quaternion vecToQuad = convVecToQuat(vec);
	 struct Quaternion rotated = hamiltonP(hamiltonP(rotation, vecToQuad), konjugate(rotation));
	 return convQuatToVec(rotated);
 }