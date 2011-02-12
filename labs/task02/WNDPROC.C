/*
 * wndproc.c
 * Main window events handling.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 08.04.2007
 */

#include "wndproc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "figure.h"
#include "figure_sierpinsky2.h"
#include "render.h"

#define MAX_LINE_BUF 10000
#define MAX_LOD 10

typedef struct
{
  char *name;  /* Figure name */
  int npoints; /* Required for building figure number of points */
  /* Pointer to initializing figure function */
  Figure *(*FigInit)( int maxLoD, Point *points );
  /* Pointer to destroying figure data function */
  void (*FigDestroy)( Figure *fig );
} FigDescr;

/* Main window process messages callback function */
LRESULT CALLBACK WndProc( HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;
  static Point points[3]; /* Figure selected points */
  static int npoints = 0; /* Number of selected points */
  static Figure *fig;
  static Line buffer[MAX_LINE_BUF];
  static int nlines = 0;
  static FigDescr figsDescr[] = { { "Sierpinsky 2", 3, SierpinskyFigInit, SierpinskyFigDestroy } };
  static int currentFig = 0, currentLoD = 0;
  static int mX, mY, oldMX, oldMY;
  static HBRUSH hBlackBrush, hWhiteBrush;
    
  switch (message)
  {
  case WM_CREATE:
    /* Creating instruments */
    hBlackBrush = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
    hWhiteBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
    break;

  case WM_SIZE:
    break;

  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);

    if (npoints > 0 && npoints < figsDescr[currentFig].npoints)
    {
      int i;

      SelectObject(hdc, hBlackBrush);
      for (i = 1; i < npoints; i++)
      {
        MoveToEx(hdc, points[i - 1].x, points[i - 1].y, NULL);
        LineTo(hdc, points[i].x, points[i].y); 
      }
      MoveToEx(hdc, points[npoints - 1].x, points[npoints - 1].y, NULL);
      LineTo(hdc, mX, mY); 
    }
    else
    {
      SelectObject(hdc, hBlackBrush);
      RenderLines(hdc, buffer, nlines);
    }

    EndPaint(hWnd, &ps);
    break;

  case WM_CHAR:
    /* Quiting on escape */
    if (wParam == 27)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
    /* Resetting points */
    else if (wParam >= '0')
    {
      if (npoints >= figsDescr[currentFig].npoints)
      {
        figsDescr[currentFig].FigDestroy(fig);
      }
      npoints = 0;
      nlines = 0;
      InvalidateRect(hWnd, NULL, TRUE);
    }
    /* Changing figure */
    /*else if (wParam >= '1' && wParam <= '9')
    {
      int n = wParam - '0';

      if (n <= sizeof(figDescr) / sizeof(figDescr[0]))
      {
        if (npoints >= figsDescr[currentFig].npoints)
        {
        }

        currentFig = n;
      }
    }*/
    break;
  
  case WM_KEYDOWN: 
    switch (wParam) 
    { 
    case VK_LEFT:
      if (currentLoD > 0)
      {
        currentLoD--;
        if (npoints >= figsDescr[currentFig].npoints)
        {
          nlines = fig->RenderLines(fig, buffer, MAX_LINE_BUF, currentLoD);
          InvalidateRect(hWnd, NULL, TRUE);
        }
      }
      break;
    case VK_RIGHT:
      if (currentLoD + 1 < MAX_LOD)
      {
        currentLoD++;
        if (npoints >= figsDescr[currentFig].npoints)
        {
          nlines = fig->RenderLines(fig, buffer, MAX_LINE_BUF, currentLoD);
          InvalidateRect(hWnd, NULL, TRUE);
        }
      }
      break;
    }
    break;

  case WM_LBUTTONDOWN:
    mX = (signed short)LOWORD(lParam);
    mY = (signed short)HIWORD(lParam);

    if (npoints < figsDescr[currentFig].npoints)
    {
      points[npoints].x = mX;
      points[npoints].y = mY;
      npoints++;

      if (npoints >= figsDescr[currentFig].npoints)
      {
        if ((fig = figsDescr[currentFig].FigInit(MAX_LOD, points)) == NULL)
          exit(1);
        nlines = fig->RenderLines(fig, buffer, MAX_LINE_BUF, currentLoD);
        InvalidateRect(hWnd, NULL, TRUE);
      }
    }
    break;

  case WM_RBUTTONDOWN:
    if (npoints >= figsDescr[currentFig].npoints)
    {
      figsDescr[currentFig].FigDestroy(fig);
    }
    npoints = 0;
    nlines = 0;
    InvalidateRect(hWnd, NULL, TRUE);
    break;

  case WM_MOUSEMOVE:
    if (npoints > 0 && npoints < figsDescr[currentFig].npoints)
    {
      //char buf[100];

      mX = (signed short)LOWORD(lParam);
      mY = (signed short)HIWORD(lParam);

      //sprintf(buf, "%d %d", mX, mY);

      //SetWindowText(hWnd, buf);

      InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

  case WM_DESTROY:
    if (npoints >= figsDescr[currentFig].npoints)
    {
      figsDescr[currentFig].FigDestroy(fig);
    }
    /* Destroying created instruments */
    DeleteObject(hBlackBrush);
    DeleteObject(hWhiteBrush);
    /* Quiting */
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
} /* End of 'WndProc' function */

/* END OF 'wndproc.c' FILE */
