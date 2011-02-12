/*
 * main.c
 * Self-similar sets fractals.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 06.03.2007
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "figure.h"

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
    "Semester 02, task 02. Bob Rutsky, SPbSTU, 1057/2. 06.03.2007.", WS_OVERLAPPEDWINDOW,
    50, 20, 600, 640, NULL, NULL, hInstance, NULL);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
} /* End of 'InitInstance' function */

/* Main window process messages callback function */
LRESULT CALLBACK WndProc( HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;
  static RECT rt, boardRect, xAxisRect, yAxisRect, figureRect, pathLenRect;
  static Point fig1, fig2;
  static Point path[MAX_PATH_QUEUE];
  static HBRUSH hBlackBrush, hWhiteBrush, hRedBrush;
  static HPEN hPen;
  static ChessFigure *chessFigures;
  static int nChessFigures, currentChessFigure = 0 , pathLen = -1;
  int mX, mY;
  
  switch (message)
  {
  case WM_CREATE:
    /* Initializing chess figures */
    nChessFigures = CreateChessFigures(&chessFigures);
    /* Initializing board data */
    srand((unsigned)time(NULL));
    fig1.x = (int)((double)rand() / RAND_MAX * BOARD_X);
    fig1.y = (int)((double)rand() / RAND_MAX * BOARD_Y);
    fig2.x = (int)((double)rand() / RAND_MAX * BOARD_X);
    fig2.y = (int)((double)rand() / RAND_MAX * BOARD_Y);
    pathLen = SearchTurns(path, MAX_PATH_QUEUE, &fig1, &fig2,
      &chessFigures[currentChessFigure]);
    /* Creating instruments */
    hBlackBrush = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
    hWhiteBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
    hRedBrush = CreateSolidBrush(RGB(0xFF, 0x00, 0x00));
    hPen = CreatePen(PS_SOLID, 4, RGB(0xFF, 0x10, 0x10));
    break;
  case WM_SIZE:
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);

    /* Calculating render rectangles */
    GetClientRect(hWnd, &rt);

    /* Board rectangle */
    boardRect.left = rt.left + AXIS_WIDTH;
    boardRect.right = rt.right;
    boardRect.top = rt.top + CAPTION_HEIGHT;
    boardRect.bottom = rt.bottom - AXIS_HEIGHT;

    /* Correcting aspect */
    if ((double)(boardRect.right - boardRect.left) / BOARD_X >
        (double)(boardRect.bottom - boardRect.top) / BOARD_Y)
    {
      boardRect.right = (long)(boardRect.left +
        (double)(boardRect.bottom - boardRect.top) / BOARD_Y * BOARD_X);
    }
    else
    {
      boardRect.bottom = (long)(boardRect.top +
        (double)(boardRect.right - boardRect.left) / BOARD_X * BOARD_Y);
    }
    
    /* 'X' axis rectangle */
    xAxisRect.left   = boardRect.left;
    xAxisRect.right  = boardRect.right;
    xAxisRect.top    = boardRect.bottom;
    xAxisRect.bottom = boardRect.bottom + AXIS_HEIGHT;

    /* 'Y' axis rectangle */
    yAxisRect.left   = rt.left;
    yAxisRect.right  = rt.left + AXIS_WIDTH;
    yAxisRect.top    = boardRect.top;
    yAxisRect.bottom = boardRect.bottom;

    /* Figure name rectangle */
    figureRect.left   = rt.left + AXIS_WIDTH;
    figureRect.right  = rt.left + AXIS_WIDTH + FIGURE_WIDTH;
    figureRect.top    = rt.top;
    figureRect.bottom = rt.top + CAPTION_HEIGHT;

    /* Path length string rectangle */
    pathLenRect.left   = rt.left + FIGURE_WIDTH + AXIS_WIDTH;
    pathLenRect.right  = rt.left + FIGURE_WIDTH + AXIS_WIDTH + PATH_WIDTH;
    pathLenRect.top    = rt.top;
    pathLenRect.bottom = rt.top + CAPTION_HEIGHT;

    /* Drawing figure caption */
    DrawFigureCaption(hdc, &figureRect,
      chessFigures[currentChessFigure].name);

    /* Drawing path length caption */
    DrawPathLenCaption(hdc, &pathLenRect, pathLen);

    /* Drawing chessboard */
    DrawChessboard(hdc, &boardRect, hBlackBrush, hWhiteBrush);

    /* Drawing axes */
    DrawXAxis(hdc, &xAxisRect);
    DrawYAxis(hdc, &yAxisRect);

    /* Drawing path */
    DrawPath(hdc, &boardRect, &fig1, path, pathLen, hPen, hRedBrush);

    /* Drawing figures labels */
    DrawFigureName(hdc, &boardRect, &fig1, "Fig.1");
    DrawFigureName(hdc, &boardRect, &fig2, "Fig.2");
    
    EndPaint(hWnd, &ps);
    break;
  case WM_CHAR:
    /* Quiting on escape */
    if (wParam == 27)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
    break;
  case WM_LBUTTONUP:
    mX = LOWORD(lParam);
    mY = HIWORD(lParam);

    /* Changing first figure position */
    if (_isInRect(&boardRect, mX, mY))
    {
      fig1.x = _getXRectPart(&boardRect, BOARD_X, mX);
      fig1.y = _getYRectPart(&boardRect, BOARD_Y, mY);
      pathLen = SearchTurns(path, MAX_PATH_QUEUE, &fig1, &fig2,
        &chessFigures[currentChessFigure]);

      InvalidateRect(hWnd, &rt, TRUE);
    }
    /* Changing figure type (next) */
    else if (_isInRect(&figureRect, mX, mY))
    {
      currentChessFigure = (currentChessFigure + 1) % nChessFigures;
      pathLen = SearchTurns(path, MAX_PATH_QUEUE, &fig1, &fig2,
        &chessFigures[currentChessFigure]);

      InvalidateRect(hWnd, &rt, TRUE);
    }
    break;
  case WM_RBUTTONUP:
    mX = LOWORD(lParam);
    mY = HIWORD(lParam);

    if (_isInRect(&boardRect, mX, mY))
    {
      /* Changing second figure position */
      fig2.x = _getXRectPart(&boardRect, BOARD_X, mX);
      fig2.y = _getYRectPart(&boardRect, BOARD_Y, mY);
      pathLen = SearchTurns(path, MAX_PATH_QUEUE, &fig1, &fig2,
        &chessFigures[currentChessFigure]);

      InvalidateRect(hWnd, &rt, TRUE);
    }
    /* Changing figure type (previous) */
    else if (_isInRect(&figureRect, mX, mY))
    {
      currentChessFigure =
        (currentChessFigure + nChessFigures - 1) % nChessFigures;
      pathLen = SearchTurns(path, MAX_PATH_QUEUE, &fig1, &fig2,
        &chessFigures[currentChessFigure]);

      InvalidateRect(hWnd, &rt, TRUE);
    }
    break;
  case WM_DESTROY:
    /* Destroying created chess figures */
    DestroyChessFigures(chessFigures, nChessFigures);
    /* Destroying created instruments */
    DeleteObject(hBlackBrush);
    DeleteObject(hWhiteBrush);
    DeleteObject(hRedBrush);
    DeleteObject(hPen);
    /* Quiting */
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
} /* End of 'WndProc' function */

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
