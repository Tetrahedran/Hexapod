#include "hexapod.h"

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

float* calcMotorAngles(struct Vector absTranslation, struct Quaternion absRotation)
{
	float[6] sollWinkel;
	for (short i = 0; i < 6; i++)
	{
		struct Vector effectiveRod = calcEffectiveRodLength(absTranslation, absRotation,
			motorPositions[i], anchorPositions[i]);
			sollWinkel[i] = angle(effectiveRod, rodLength, hornLength, hornRotations[i]);
	}
}

float angle(struct Vector effectiveRodVector, float rodLength, float hornLength, float hornRot)
{
	float eK = eK(hornLength, effectiveRodVector);
	float fK = fK(hornLength, effectiveRodVector, hornRot);
	float gK = gK(hornLength, effectiveRodVector, rodLength);
	return alphaK(eK, fK, gK);
}

float eK(float hornLength, struct Vector effectiveRodVector)
{
	return 2f * hornLength * effectiveRodVector.z;
}

float fK(float hornLength, struct Vector effectiveRodVector, float hornRotation)
{
	return 2f * hornLength * (cosf(hornRotation) * effectiveRodVector.x + sinf(hornRotation) * effectiveRodVector.y);
}

float gK(float hornLength, struct Vector effectiveRodVector, float rodLength)
{
	return powf(magnitude(effectiveRodVector), 2) - rodLength * rodLength - hornLength * hornLength;
}

float alphaK(float eK, float fK, float gK) 
{
	float angle = asinf(gK / sqrtf(eK * eK - fK * fK)) - atan2f(fK, eK);
	return angle;
}

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

