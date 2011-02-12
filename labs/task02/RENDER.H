/*
 * render.h
 * Rendering interface implementation.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 06.03.2007
 */

#ifndef _RENDER_H_
#define _RENDER_H_

#include <windows.h>

#include "geom.h"

/* Rendering lines from buffer function */
int RenderLines( HDC hdc, Line *buffer, int nlines );

#endif /* _RENDER_H_ */

/* END OF 'render.h' FILE */
