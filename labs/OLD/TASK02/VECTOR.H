/*
 * vector.h
 * 2d vectors implementation.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 02.03.2007
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>

/* Vector base type */
#define TYPE double

/* Vector description data */
typedef struct
{
  TYPE x, y;
} Vector;

/* Creating new vector function */
Vector *VecNew( TYPE x, TYPE y );

/* Destroying vector structure function */
void VecDelete( Vector *v );

/* Setting vector coorditates to passed values function */
Vector *VecSet( Vector *v, TYPE x, TYPE y );

/* Cloning vector function */
Vector *VecClone( Vector *v );

/* Negating vector function */
Vector *VecNegate( Vector *v );

/* Returning negated vector function */
Vector *VecNegateNew( Vector *v );

/* Adding vector to vector function */
Vector *VecAdd( Vector *v1, Vector *v2 );

/* Returning sum of two vectors function */
Vector *VecAddNew( Vector *v1, Vector *v2 );

/* Subtracting vector from vector function */
Vector *VecSub( Vector *v1, Vector *v2 );

/* Returning subtract of two vectors function */
Vector *VecSubNew( Vector *v1, Vector *v2 );

/* Multiplying vector by number function */
Vector *VecMult( Vector *v, TYPE n );

/* Returning result of multiplication of vector by number function */
Vector *VecMultNew( Vector *v, TYPE n );

/* Dividing vector by number function */
Vector *VecDiv( Vector *v, TYPE n );

/* Returning result of dividion of vector by number function */
Vector *VecDivNew( Vector *v, TYPE n );

/* Rotating vector function */
Vector *VecRotate( Vector *v, TYPE angle );

/* Returning result of vector rotation function */
Vector *VecRotateNew( Vector *v, TYPE angle );

#endif /* _VECTOR_H_ */

/* END OF 'vector.h' FILE */
