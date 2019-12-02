#include "hexpod_constants.h"
#include <math.h>

float baseSideLength;
float platformSideLength;
float rodLength;
float hornLength;
float basePositionKoeff;
float platPositionKoeff;

struct Vector Ab, Bb, Cb;
struct Vector Ap, Bp, Cp;

struct Vector motorPositions[6];
struct Vector anchorPositions[6];
float hornRotations[6];

float sqrt3 = sqrtf(3);

void calcTriangleCorner(void);
void calcMotorPositions(void);
void calcAnchorPositions(void);
void calcHornAngle(void);

void initialize(
	float _baseSideLength, 
	float _platformSideLength, 
	float _rodLength, 
	float _hornLength, 
	float _basePositionKoeff, 
	float _platPositionKoeff) 
{
	baseSideLength = _baseSideLength;
	platformSideLength = _platformSideLength;
	rodLength = _rodLength;
	hornLength = _hornLength;
	basePositionKoeff = _basePositionKoeff;
	platPositionKoeff = _platPositionKoeff;
	calcTriangleCorner();
	calcMotorPositions();
	calcAnchorPositions();
	calcHornAngle();
}

void calcHornAngle(void) 
 {
	 hornRotations[0] = M_PI;
	 hornRotations[1] = 0.0f;
	 hornRotations[2] = 60.0f / 360.0f * 2.0f * M_PI;
	 hornRotations[3] = -120.0f / 360.0f * 2.0f * M_PI;
	 hornRotations[4] = -60.0f / 360.0f * 2.0f * M_PI;
	 hornRotations[5] = 120.0f / 360.0f * 2.0f * M_PI;
	 
 }

void calcAnchorPositions(void)
{
	float inversePPosKoeff = 1.0f - platPositionKoeff;
	
	struct Vector AB = subtract(Bp, Ap);
	struct Vector BC = subtract(Cp, Bp);
	struct Vector CA = subtract(Ap, Cp);
	
	anchorPositions[0] = add(Ap, multiply(AB, platPositionKoeff));
	anchorPositions[1] = add(Cp, multiply(CA, inversePPosKoeff));
	anchorPositions[2] = add(Cp, multiply(CA, platPositionKoeff));
	anchorPositions[3] = add(Bp, multiply(BC, inversePPosKoeff));
	anchorPositions[4] = add(Bp, multiply(BC, platPositionKoeff));
	anchorPositions[5] = add(Ap, multiply(AB, inversePPosKoeff));
}

void calcMotorPositions(void)
{
	float inverseBPosKoeff = 1.0f - basePositionKoeff;
	
	struct Vector AB = subtract(Bb, Ab);
	struct Vector BC = subtract(Cb, Bb);
	struct Vector CA = subtract(Ab, Cb);
	
	motorPositions[0] = add(Ab, multiply(AB, inverseBPosKoeff));
	motorPositions[1] = add(Ab, multiply(AB, basePositionKoeff));
	motorPositions[2] = add(Cb, multiply(CA, inverseBPosKoeff));
	motorPositions[3] = add(Cb, multiply(CA, basePositionKoeff));
	motorPositions[4] = add(Bb, multiply(BC, inverseBPosKoeff));
	motorPositions[5] = add(Bb, multiply(BC, basePositionKoeff));
}

void calcTriangleCorner(void)
{
	float innKreisRadiusB = sqrt3 / 6.0f * baseSideLength;
	float umKreisRadiusB = 2.0f * innKreisRadiusB;
	
	Ab = (struct Vector){
		-baseSideLength / 2.0f,
		-innKreisRadiusB,
		0	
	};
	
	Bb = (struct Vector){
		baseSideLength / 2.0f,
		-innKreisRadiusB,
		0
	};
	
	Cb = (struct Vector){
		0,
		umKreisRadiusB,
		0
	};
	
	float innKreisRadiusP = sqrt3 / 6.0f * platformSideLength;
	float umKreisRadiusP = 2.0f * innKreisRadiusP;
	
	Ap = (struct Vector){
		0,
		-umKreisRadiusP,
		0
	};
	
	Bp = (struct Vector){
		platformSideLength / 2.0f,
		innKreisRadiusP,
		0
	};
	
	Cp = (struct Vector){
		-platformSideLength / 2,
		innKreisRadiusP,
		0
	};
		
}
