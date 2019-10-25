#include "quaternion.h"
#include <math.h>

struct Quaternion convVecToQuat(struct Vector vec) 
{
	struct Quaternion quat = {0, vec.x, vec.y, vec.z};
	return quat;
}

struct Quaternion quatForRot(struct Vector axis, float angle)
{
	float halfAngle = angle / 2f;
	float sinHalfAngle = sinf(halfAngle);
	struct Quaternion quad = {
		cosf(halfAngle),
		axis.x * sinHalfAngle,
		axis.y * sinHalfAngle,
		axis.z * sinHalfAngle
	};
	return quad;
}

struct Quaternion conjugate(struct Quaternion quat) 
{
	struct Quaternion ret = {quat.w, -quat.i, -quat.j, -quat.k};
	return ret;
}

struct Quaternion hamiltonP(struct Quaternion lhs, struct Quaternion rhs)
{
	struct Quaternion ret = {
		lhs.w * rhs.w - lhs.i * rhs.i - lhs.j * rhs.j - lhs.k * rhs.k,
		lhs.w * rhs.i + lhs.i * rhs.w + lhs.j * rhs.k - lhs.k * rhs.j,
		lhs.w * rhs.j - lhs.i * rhs.k + lhs.j * rhs.w + lhs.k * rhs.i,
		lhs.w * rhs.k + lhs.i * rhs.j - lhs.j * rhs.i + lhs.k * rhs.w
	};
	return ret;
}