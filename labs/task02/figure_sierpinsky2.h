/*
 * figure_sierpinsky2.h
 * Sierpinsky fractal implementation
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 02.03.2007
 */

#ifndef _FIGURE_SIERPINSKY2_
#define _FIGURE_SIERPINSKY2_

#include "geom.h"
#include "figure.h"

/* Initializing Sierpinski triangle figure function */
Figure *SierpinskyFigInit( int maxLoD, Point *points );

/* Destroying Sierpinsky figure data function */
void SierpinskyFigDestroy( Figure *fig );

#endif /* _FIGURE_SIERPINSKY2_ */

/* END OF 'figure_sierpinsky.h' FILE */
