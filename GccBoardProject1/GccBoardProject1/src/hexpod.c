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
* Berechnet die Motorwinkel für alle Motoren
*/
float* calcMotorAngles(struct Vector absTranslation, struct Quaternion absRotation)
{
	static float sollWinkel[6];
	for (short i = 0; i < 6; i++)
	{
		struct Vector effectiveRod = calcEffectiveRodLength(absTranslation, absRotation, motorPositions[i], anchorPositions[i]);
		sollWinkel[i] = angle(effectiveRod, rodLength, hornLength, hornRotations[i]);
	}
	return sollWinkel;
}

/*
* Berechnet den Winkel für einen Motor
*/
float angle(struct Vector effectiveRodVector, float rodLength, float hornLength, float hornRot)
{
	float EK = eK(hornLength, effectiveRodVector);
	float FK = fK(hornLength, effectiveRodVector, hornRot);
	float GK = gK(hornLength, effectiveRodVector, rodLength);
	return alphaK(EK, FK, GK);
}

/*
* Hilfswert eK für Motorwinkel Richtig Berechnet
*/
float eK(float hornLength, struct Vector effectiveRodVector)
{
	return 2.0f * hornLength * effectiveRodVector.z;
}

/*
* Hilfswert fK für Motorwinkel
*/
float fK(float hornLength, struct Vector effectiveRodVector, float hornRotation)
{
	return 2.0f * hornLength * (cosf(hornRotation) * effectiveRodVector.x + sinf(hornRotation) * effectiveRodVector.y);
}

/*
* Hilfswert gK für Motorwinkel
*/
float gK(float hornLength, struct Vector effectiveRodVector, float rodLength)
{
	return powf(magnitude(effectiveRodVector), 2) - rodLength * rodLength - hornLength * hornLength;
}

/*
* Berechnet den Motorwinkel für einen Motor
*/
float alphaK(float eK, float fK, float gK) 
{
	float angle = asinf(gK / sqrtf(eK * eK - fK * fK)) - atan2f(fK, eK);
	return angle;
}

/*
*  Berechnet den Vektor eines imaginären Stabs von der Motorwelle eines Motors bis zum korrespondierenden Ankerpunkt
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

