/*
 * render.c
 * Rendering interface implementation.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 06.03.2007
 */

#include <windows.h>

#include "render.h"

#include "geom.h"

/* Rendering lines from buffer function */
int RenderLines( HDC hdc, Line *buffer, int nlines )
{
  int i;

  for (i = 0; i < nlines; i++)
  {
    MoveToEx(hdc, buffer[i].x0, buffer[i].y0, NULL);
    LineTo(hdc, buffer[i].x1, buffer[i].y1); 
  }
} /* End of 'RenderLines' function */

/* END OF 'render.c' FILE */
