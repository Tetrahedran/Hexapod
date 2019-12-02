#include "hexapod.h"
#include <math.h>

struct Vector calcEffectiveRodLength(
	struct Vector absTrans,
	struct Quaternion absRot,
	struct Vector motorPosition,
	struct Vector anchorPosition);

float alphaK(float eK, float fK, float gK);
float eK(float hornLength, struct Vector effectiveRodVector);
float fK(float hornLength, struct Vector effectiveRodVector, float hornRotation);
float gK(float hornLength, struct Vector effectiveRodVector, float rodLength);

float angle(struct Vector effectiveRodVector, float rodLength, float hornLength, float hornRot);

/*
* Berechnet die Motorwinkel f�r alle Motoren
*/
void calcMotorAngles(float* sollWinkel, struct Vector absTranslation, struct Quaternion absRotation)
{
	
	for (short i = 0; i < 6; i++)
	{
		struct Vector effectiveRod = calcEffectiveRodLength(absTranslation, absRotation, motorPositions[i], anchorPositions[i]);
		sollWinkel[i] = angle(effectiveRod, rodLength, hornLength, hornRotations[i]);
	}
}

/*
* Berechnet den Winkel f�r einen Motor
*/
float angle(struct Vector effectiveRodVector, float rodLength, float hornLength, float hornRot)
{
	float EK = eK(hornLength, effectiveRodVector);
	float FK = fK(hornLength, effectiveRodVector, hornRot);
	float GK = gK(hornLength, effectiveRodVector, rodLength);
	return alphaK(EK, FK, GK);
}

/*
* Hilfswert eK f�r Motorwinkel
*/
float eK(float hornLength, struct Vector effectiveRodVector)
{
	return 2.0f * hornLength * effectiveRodVector.z;
}

/*
* Hilfswert fK f�r Motorwinkel
*/
float fK(float hornLength, struct Vector effectiveRodVector, float hornRotation)
{
	return 2.0f * hornLength * (cosf(hornRotation) * effectiveRodVector.x + sinf(hornRotation) * effectiveRodVector.y);
}

/*
* Hilfswert gK f�r Motorwinkel
*/
float gK(float hornLength, struct Vector effectiveRodVector, float rodLength)
{
	float sqrMag = (magnitude(effectiveRodVector) * magnitude(effectiveRodVector));
	float sqrRodLength = rodLength * rodLength;
	float sqrHornLength = hornLength * hornLength;
	return  sqrMag - (sqrRodLength - sqrHornLength);
}

/*
* Berechnet den Motorwinkel f�r einen Motor
*/
float alphaK(float eK, float fK, float gK) 
{
	float angle = asin(gK / sqrt(eK * eK + fK * fK)) - atan2(fK, eK);
	return angle;
}

/*
*  Berechnet den Vektor eines imagin�ren Stabs von der Motorwelle eines Motors bis zum korrespondierenden Ankerpunkt
*/
struct Vector calcEffectiveRodLength(
	struct Vector absTrans, 
	struct Quaternion absRot, 
	struct Vector motorPosition, 
	struct Vector anchorPosition
)
{
	struct Vector rotatedAnchor = rotate(anchorPosition, absRot);
	return subtract(add(absTrans, rotatedAnchor), motorPosition);
}

