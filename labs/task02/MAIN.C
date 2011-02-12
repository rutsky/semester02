/*
 * main.c
 * Self-similar sets fractals.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 06.03.2007
 */

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wndproc.h"

/* Global Variables */
HINSTANCE hInst;                       /* Current instance */
const char *szWindowClass = "MyClass"; /* Main class name */

/* Saves instance handle and creates main window function */
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
  HWND hWnd;

  /* Store instance handle in our global variable */
  hInst = hInstance;

  hWnd = CreateWindow(szWindowClass, 
    "Semester 02, task 02. Bob Rutsky, SPbSTU, 1057/2. 06.03.2007.",
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX,
    50, 20, 600, 640, NULL, NULL, hInstance, NULL);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
} /* End of 'InitInstance' function */

/* Registering class function */
ATOM MyRegisterClass( HINSTANCE hInstance )
{
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style         = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc   = (WNDPROC)WndProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = NULL;
  wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm       = NULL;

  return RegisterClassEx(&wcex);
} /* End of 'MyRegisterClass' function */

/* Main program function */
int APIENTRY WinMain( HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR     lpCmdLine,
                      int       nCmdShow)
{
  MSG msg;

  /* Register main class */
  MyRegisterClass(hInstance);

  /* Perform application initialization */
  if (!InitInstance(hInstance, nCmdShow)) 
  {
    return FALSE;
  }

  /* Main message loop */
  while (GetMessage(&msg, NULL, 0, 0)) 
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'main' function */

/* END OF 'main.c' FILE */
