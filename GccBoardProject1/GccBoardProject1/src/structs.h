#ifndef MATH_STRUCTS_H
#define MATH_STRUCTS_H

/*
 * Struct, dass ein Quaternion darstellt
 */
struct Quaternion{
	float w;
	float i;
	float j;
	float k;
};

/*
 * Struct das einen dreidimensionalen Vektor darstellt
 */
struct Vector {
	float x;
	float y;
	float z;
};

#endif