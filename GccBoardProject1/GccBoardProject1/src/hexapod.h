#ifndef HEXAPOD_H
#define HEXAPOD_H
#include "vector.h"
#include "quaternion.h"
#include "hexpod_constants.h"

/*
 * Berechnet die notwendigen Motorwinkel für eine globale Translation und eine globale Rotation
 * param angles: Array in welchen die Motorwinkel eingetragen werden
 * param absTranslation: Vektor welcher die absolute Translation des Hexapoden beschreibt
 * param absRotation: Quaternion welches die globale Rotation des Hexapoden beschreibt
 */
extern void calcMotorAngles(float* angles, struct Vector absTranslation, struct Quaternion absRotation);

#endif