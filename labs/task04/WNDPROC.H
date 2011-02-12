/*
 * wndproc.h
 * Main window events handling.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 08.04.2007
 */

#ifndef _WNDPROC_H_
#define _WNDPROC_H_

#include <windows.h>

/* Visualiztions constants */
enum
{
  /* Board position */
  BOARD_X = 0,
  BOARD_Y = 0,
  /* Board cell parameters */
  CELL_X_SIZE   = 8,
  CELL_Y_SIZE   = 8,
  /* Border between board cells parameters */
  BORDER_WIDTH  = 1,
  BORDER_HEIGHT = 1,
};

/* Main window process messages callback function */
LRESULT CALLBACK WndProc( HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam );

#endif /* _WNDPROC_H_ */

/* END OF 'wndproc.h' FILE */
