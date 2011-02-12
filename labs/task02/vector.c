/*
 * vector.c
 * 2d vectors implementation.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 02.03.2007
 */

#include "vector.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* Note: 
 * This is not optimized realisation of vectors.
 */

/* Creating new vector function */
Vector *VecNew( TYPE x, TYPE y )
{
  Vector *v;
  
  if ((v = malloc(sizeof(Vector))) == NULL)
  {
    perror("malloc");
    return NULL;
  }
  
  v->x = x;
  v->y = y;
  
  return v;
} /* End of 'VecNew' function */

/* Destroying vector structure function */
void VecDelete( Vector *v )
{
  free(v);
} /* End of 'VecDelete' function */

/* Setting vector coorditates to passed values function */
Vector *VecSet( Vector *v, TYPE x, TYPE y )
{
  v->x = x;
  v->y = y;

  return v;
} /* End of 'VecSet' function */

/* Cloning vector function */
Vector *VecClone( Vector *v )
{
  return VecNew(v->x, v->y);
} /* End of 'VecClone' function */

/* Negating vector function */
Vector *VecNegate( Vector *v )
{
  v->x -= 0;
  v->y -= 0;
  
  return v;
} /* End of 'VecNegate' function */

/* Returning negated vector function */
Vector *VecNegateNew( Vector *v )
{
  return VecNew(-v->x, -v->y);
} /* End of 'VecNegateNew' function */

/* Adding vector to vector function */
Vector *VecAdd( Vector *v1, Vector *v2 )
{
  v1->x += v2->x;
  v1->y += v2->y;
  
  return v1;
} /* End of 'VecAdd' function */

/* Returning sum of two vectors function */
Vector *VecAddNew( Vector *v1, Vector *v2 )
{
  return VecNew(v1->x + v2->x, v1->y + v2->y);
} /* End of 'VecAddNew' function */

/* Subtracting vector from vector function */
Vector *VecSub( Vector *v1, Vector *v2 )
{
  v1->x -= v2->x;
  v1->y -= v2->y;
  
  return v1;
} /* End of 'VecSub' function */

/* Returning subtract of two vectors function */
Vector *VecSubNew( Vector *v1, Vector *v2 )
{
  return VecNew(v1->x - v2->x, v1->y - v2->y);
} /* End of 'VecSubNew' function */

/* Multiplying vector by number function */
Vector *VecMult( Vector *v, TYPE n )
{
  v->x *= n;
  v->y *= n;
  
  return v;
} /* End of 'VecMult' function */

/* Returning result of multiplication of vector by number function */
Vector *VecMultNew( Vector *v, TYPE n )
{
  return VecNew(v->x * n, v->y * n);
} /* End of 'VecMultNew' function */

/* Dividing vector by number function */
Vector *VecDiv( Vector *v, TYPE n )
{
  v->x /= n;
  v->y /= n;
  
  return v;
} /* End of 'VecDiv' function */

/* Returning result of dividion of vector by number function */
Vector *VecDivNew( Vector *v, TYPE n )
{
  return VecNew(v->x / n, v->y / n);
} /* End of 'VecDivNew' function */

/* Rotating vector function */
Vector *VecRotate( Vector *v, TYPE angle )
{
  Vector res;
  
  res.x = v->x * cos(angle) - v->y * sin(angle);
  res.y = v->x * sin(angle) + v->y * cos(angle);
  
  *v = res;
  
  return v;
} /* End of 'VecRotate' function */

/* Returning result of vector rotation function */
Vector *VecRotateNew( Vector *v, TYPE angle )
{
  return VecNew(v->x * cos(angle) - v->y * sin(angle), v->x * sin(angle) + v->y * cos(angle));
} /* End of 'VecRotateNew' function */

/* END OF 'vector.c' FILE */
