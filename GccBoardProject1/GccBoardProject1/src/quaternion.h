#ifndef QUATERNION_H
#define  QUATERNION_H
#include "structs.h"

/*
 * Erzeugt ein Quaternion für eine Rotation um eine Achse
 * param Vector: Vektor der Drehachse
 * param float: Drehwinkel in Rad
 * return: Ein Quaternion für die beschriebene Rotation
 */
extern struct Quaternion quatForRot(struct Vector, float);

/*
 * Konvertiert einen Vektor in ein Quaternion
 * param Vector: Vektor der konvertiert werden soll
 * return: Ein Quaternion der Form {1,x,y,z} 
 */
extern struct Quaternion convVecToQuat(struct Vector);

/*
 * Erzeugt ein konjugiertes Quaternion
 * param Quaternion: Das zu konjugierende Quaternion
 * return: Das konjugierte Quaternion
 */
extern struct Quaternion conjugate(struct Quaternion);

/*
 * Erzeugt das Hamilton-Produkt aus zwei Quaternionen
 * param Quaternion: Die beiden zu multiplizierenden Quaternionen. Die Reihenfolge spielt dabei eine Rolle
 * return: Das Hamilton-Produkt der beiden übergebenen Quaternionen
 */
extern struct Quaternion hamiltonP(struct Quaternion, struct Quaternion);

#endif