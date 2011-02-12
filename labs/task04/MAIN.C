/*
 * main.c
 * Snake game.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedefs.h"

#include "wndproc.h"
#include "game.h"
#include "err.h"

/* Global Variables */
extern GameDescr *GameData = NULL;
extern HINSTANCE hInstance = NULL; /* Current instance */

HWND hWnd;
const char *szWindowClass = "MyClass"; /* Main class name */

/* Saves instance handle and creates main window function */
BOOL InitInstance( HINSTANCE hInst, int nCmdShow )
{
  /* Store instance handle in our global variable */
  hInstance = hInst;

  hWnd = CreateWindow(szWindowClass, 
    "Semester 02, task 04. Bob Rutsky, SPbSTU, 1057/2. 13.05.2007.",
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX,
    50, 20,
    BOARD_X_SIZE * (CELL_X_SIZE + BORDER_WIDTH) + BORDER_WIDTH + 6,
    BOARD_Y_SIZE * (CELL_Y_SIZE + BORDER_HEIGHT) + BORDER_HEIGHT + 32,
    NULL, NULL, hInstance, NULL);

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
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm       = NULL;

  return RegisterClassEx(&wcex);
} /* End of 'MyRegisterClass' function */

/* Main program function */
int APIENTRY WinMain( HINSTANCE hInst,
                      HINSTANCE hPrevInstance,
                      LPSTR     lpCmdLine,
                      int       nCmdShow)
{
  MSG msg;
  BOOL done;

  /* Initializing timer */
  InitTimer();
  SetRandom((int)GetTicks());

  /* Initializing game data */
  if ((GameData = CreateGame()) == NULL)
    return 1;

  /* Spawning snakes */
  SpawnSnake(GameData, 0, SnakeInitLength, SnakeInitVelocity);
  SpawnSnake(GameData, 1, SnakeInitLength, SnakeInitVelocity);
  SpawnSnake(GameData, 2, SnakeInitLength, SnakeInitVelocity);
  SpawnSnake(GameData, 3, SnakeInitLength, SnakeInitVelocity);

  /* Register main class */
  if (!MyRegisterClass(hInst))
  {
    ErrorExit("MyRegisterClass()");
    return 1;
  }
  /* Perform application initialization */
  if (!InitInstance(hInst, nCmdShow)) 
  {
    ErrorExit("InitInstance()");
    return 1;
  }

  done = FALSE;
  /* Main message loop */
  while (!done)
  {
    /*
    char buf[100];
    
    sprintf(buf, "Today's random: %.5lf", Random());
    SetWindowText(hWnd, buf);
    */

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
      {
       done = TRUE;
       break;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    
    /* Updating game */
    UpdateGame(GameData);

    InvalidateRect(hWnd, NULL, FALSE);
  }

  DestroyGame(GameData);

  return msg.wParam;
} /* End of 'main' function */

/* END OF 'main.c' FILE */
