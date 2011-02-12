/*
 * figure.h
 * Operations with fractal figures base module.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 03.03.2007
 */

#ifndef _FIGURE_H_
#define _FIGURE_H_

#include "geom.h"

/* Main figure description data */
typedef struct tagFigure
{
  void *data; /* Figure data */
  int maxLoD; /* Maximum level of detail */
  int curLoD; /* Current level of detail */
  
  /* Render full figure lines to array */
  int (*RenderLines)( struct tagFigure *fig, Line *buffer, int nlines, int level );
} Figure;

#endif /* _FIGURE_H_ */
 
/* END OF 'figure.h' FILE */
