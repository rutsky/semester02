/*
 * wndproc.h
 * Main window events handling.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 08.04.2007
 */

#ifndef _WNDPROC_H_
#define _WNDPROC_H_

#include <windows.h>

/* Main window process messages callback function */
LRESULT CALLBACK WndProc( HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam );

#endif /* _WNDPROC_H_ */

/* END OF 'wndproc.h' FILE */
