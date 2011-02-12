/*
 * geom.h
 * Base geometry objects structures definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 26.02.2007
 */

#ifndef _GEOM_H_
#define _GEOM_H_

typedef struct
{
  int x, y;
} Point;

typedef struct
{
  int x0, y0, x1, y1;
} Line;

typedef struct
{
  int x0, y0, x1, y1, x2, y2;
} Triangle;

#endif /* _GEOM_H_ */
 
/* END OF 'geom.h' FILE */
