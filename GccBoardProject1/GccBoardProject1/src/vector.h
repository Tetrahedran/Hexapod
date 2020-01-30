#ifndef VECTOR_H
#define VECTOR_H
#include "structs.h"

/*
 * Konvertiert ein Quaternion in einen Vektor
 * param Quaternion: Zu konvertierendes Quaternion
 * return: Ein Vektor der Form {i, j, k}
 */
extern struct Vector convQuatToVec(struct Quaternion);

/*
 * Multipliziert einen Vektor mit einem Skalar
 * param Vector: Zu multiplizierender Vektor 
 * param float: Skalar mit dem multipliziert werden soll
 * return: Der mit dem Skalar multiplizierte Vektor
 */
extern struct Vector multiply(struct Vector, float);

/*
 * Addiert zwei Vektoren 
 * param Vector: Die zu addierenden Vektoren
 * return: Den resultierenden Vektor der Addition
 */
extern struct Vector add(struct Vector, struct Vector);

/*
 * Subtrahiert zwei Vektoren
 * param Vector: Die beiden Vektoren die subtrahiert werden solleb
 * return: Den resultierenden Vektor der Subtraktion
 */
extern struct Vector subtract(struct Vector, struct Vector);

/*
 * Bildet das Kreuzprodukt aus zwei Vektoren
 * param Vector: Die beiden Vektoren aus denen das Kreuzprodukt gebildet werden soll
 * return: Das Kreuzprodukt aus den übergebenen Vektoren
 */
extern struct Vector cross(struct Vector, struct Vector);

/*
 * Bildet das Skalarprodukt aus zwei Vektoren
 * param Vector: Die Vektoren aus denen das Skalarprodukt gebildet werden soll
 * return: Das Skalarprodukt aus den übergebenen Vektoren
 */
extern float dot(struct Vector, struct Vector);

/*
 * Berechnet die Länge eines Vektors
 * param Vector: Der Vektor dessen Länge ermittelt werden soll
 * return: Die Länge des übergebenen Vektors
 */
extern float magnitude(struct Vector);	

/*
 * Rotiert einen Vektor 
 * param Vector: Der Vektor der rotiert werden soll
 * param Quaternion: Quaternion, welches die auszuführende Rotation beschreibt
 * return: Der rotierte Vektor
 */
extern struct Vector rotate(struct Vector, struct Quaternion);

#endif