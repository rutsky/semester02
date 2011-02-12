/*
 * wndproc.c
 * Main window events handling.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#include "typedefs.h"

#include "wndproc.h"

#include "main.h"
#include "resource.h"
#include "err.h"

/* Drawing bitmap to device function.
 * From "GipsySoft" "Draw a HBITMAP on to a HDC" article.
 */
void DrawBitmap( HDC hdc, HBITMAP hbm, int x, int y )
{
  BITMAP bm;
  HDC hdcMem;
  HGDIOBJ hOldBitmap;
  
  /* Get the bitmap dimensions */
  GetObject(hbm, sizeof(bm), &bm);
  hdcMem = CreateCompatibleDC(hdc);

  /* Save the currently selected bitmap so that we can put it back later */
  hOldBitmap = SelectObject(hdcMem, hbm);

  /* Actually do the drawing (bit-block transfer) */
  BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

  /* Put back our saved bitmap and cleanup. */
  SelectObject(hdc, hOldBitmap);

  DeleteDC(hdcMem);
} /* End of 'DrawBitmap' function */

/* Static instruments for rendering */
typedef struct
{
  HBRUSH hBorderBrush;
  HBITMAP hSnakeBitmaps[MAX_SNAKES];
  HBITMAP hWallBitmap, hFrogBitmap, hStoneBitmap, hClearBitmap, hSpeedBitmap, hSlowBitmap;
} Instruments;

/* Rendering board function */
void RenderBoard( HDC hdc, Instruments *instr )
{
  int i, x, y;
  
  SelectObject(hdc, instr->hBorderBrush);
  /* Horizonal cell border lines */
  for (i = 0; i < BOARD_Y_SIZE + 1; i++)
  {
    MoveToEx(hdc, BOARD_X, BOARD_Y + i * (CELL_Y_SIZE + 1), NULL);
    LineTo(hdc, BOARD_X + BOARD_X_SIZE * (CELL_X_SIZE + 1), BOARD_Y + i * (CELL_Y_SIZE + 1)); 
  }
  /* Vertical cell border lines */
  for (i = 0; i < BOARD_X_SIZE + 1; i++)
  {
    MoveToEx(hdc, BOARD_X + i * (CELL_X_SIZE + 1), BOARD_Y, NULL);
    LineTo(hdc, BOARD_X + i * (CELL_X_SIZE + 1), BOARD_Y + BOARD_Y_SIZE * (CELL_Y_SIZE + 1) + 1); 
  }

  for (y = 0; y < BOARD_Y_SIZE; y++)
    for (x = 0; x < BOARD_X_SIZE; x++)
    {
      HBITMAP hBitmap = instr->hClearBitmap;

      /* Selecting cell bitmap */
      if (GameData->board.board[x][y].type == CELL_WALL)
      {
        hBitmap = instr->hWallBitmap;
      }
      else if (GameData->board.board[x][y].type == CELL_BONUS)
      {
        Bonus *bonus = (Bonus *)GameData->board.board[x][y].object;

        if (bonus->type == BONUS_FROG)
          hBitmap = instr->hFrogBitmap;
        else if (bonus->type == BONUS_STONE)
          hBitmap = instr->hStoneBitmap;
        else if (bonus->type == BONUS_SPEED)
          hBitmap = instr->hSpeedBitmap;
        else if (bonus->type == BONUS_SLOW)
          hBitmap = instr->hSlowBitmap;
      }
      else if (GameData->board.board[x][y].type == CELL_SNAKE)
      {
        Snake *snake = ((SnakePiece *)GameData->board.board[x][y].object)->snake;

        hBitmap = instr->hSnakeBitmaps[snake->id];
      }

      /* Rendering cell bitmap */
      DrawBitmap(hdc, hBitmap,
        BOARD_X + 1 + x * (CELL_X_SIZE + 1),
        BOARD_Y + 1 + (BOARD_Y_SIZE - 1 - y) * (CELL_Y_SIZE + 1));
    }
} /* End of 'RenderBoard' function */

/* Loading/creating drawing instruments function */
int LoadInstruments( Instruments *instr )
{
  int resources[] = {
    IDB_BITMAP_WALL, IDB_BITMAP_FROG, IDB_BITMAP_STONE,
    IDB_BITMAP_CLEAR, IDB_BITMAP_SPEED, IDB_BITMAP_SNAKE1,
    IDB_BITMAP_SNAKE2, IDB_BITMAP_SNAKE3, IDB_BITMAP_SNAKE4,
    IDB_BITMAP_SLOW,
  };
  HBITMAP *bitmaps[] = { 
    &instr->hWallBitmap, &instr->hFrogBitmap, &instr->hStoneBitmap,
    &instr->hClearBitmap, &instr->hSpeedBitmap, &instr->hSnakeBitmaps[0],
    &instr->hSnakeBitmaps[1], &instr->hSnakeBitmaps[2], &instr->hSnakeBitmaps[3],
    &instr->hSlowBitmap,
  };
  int i;

  /* Loading images */
  for (i = 0; i < sizeof(resources) / sizeof(resources[0]); i++)
  {
    if ((*(bitmaps[i]) = LoadImage(hInstance, MAKEINTRESOURCE(resources[i]),
      IMAGE_BITMAP, CELL_X_SIZE, CELL_X_SIZE, LR_DEFAULTCOLOR)) == NULL) 
    {
      ErrorExit("LoadImage");
      return 1;
    }
  }

  /* Creating instruments */
  instr->hBorderBrush = CreateSolidBrush(RGB(0x00, 0x00, 0x00));

  return 0;
} /* End of 'LoadInstruments' function */

/* Main window process messages callback function */
LRESULT CALLBACK WndProc( HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;
  int i;
  static Instruments instr;

  switch (message)
  {
  case WM_CREATE:
    /* Loading instruments */
    if (LoadInstruments(&instr))
      return 0;
    /* Setting bonus timer */
    SetTimer(hWnd, 0, BonusSpawnTime, (TIMERPROC)NULL);
    break;

  case WM_TIMER:
    SpawnRandomBonus(GameData);
    break;

  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);

    RenderBoard(hdc, &instr);
    
    EndPaint(hWnd, &ps);
    break;

  case WM_CHAR:
    /* Quiting on escape */
    if (wParam == 27)
      SendMessage(hWnd, WM_DESTROY, 0, 0);

    {
      Snake *snake;

      /* Snake #1 controls */
      if ((snake = GetSnakeByID(GameData, 1)) != NULL)
      {
        if (wParam == 'd')
          RotateSnakeHead(snake, DIR_RIGHT);
        else if (wParam == 'w')
          RotateSnakeHead(snake, DIR_UP);
        else if (wParam == 'a')
          RotateSnakeHead(snake, DIR_LEFT);
        else if (wParam == 's')
          RotateSnakeHead(snake, DIR_DOWN);
      }

      /* Snake #2 controls */
      if ((snake = GetSnakeByID(GameData, 2)) != NULL)
      {
        if (wParam == 'l')
          RotateSnakeHead(snake, DIR_RIGHT);
        else if (wParam == 'i')
          RotateSnakeHead(snake, DIR_UP);
        else if (wParam == 'j')
          RotateSnakeHead(snake, DIR_LEFT);
        else if (wParam == 'k')
          RotateSnakeHead(snake, DIR_DOWN);
      }
    }
    break;
  
  case WM_KEYDOWN: 
    {
      Snake *snake;

      /* Snake #0 controls */
      if ((snake = GetSnakeByID(GameData, 0)) != NULL)
      {
        if (wParam == VK_RIGHT)
          RotateSnakeHead(snake, DIR_RIGHT);
        else if (wParam == VK_UP)
          RotateSnakeHead(snake, DIR_UP);
        else if (wParam == VK_LEFT)
          RotateSnakeHead(snake, DIR_LEFT);
        else if (wParam == VK_DOWN)
          RotateSnakeHead(snake, DIR_DOWN);
      }
      /* Snake #3 controls */
      if ((snake = GetSnakeByID(GameData, 3)) != NULL)
      {
        if (wParam == VK_NUMPAD6)
          RotateSnakeHead(snake, DIR_RIGHT);
        else if (wParam == VK_NUMPAD8)
          RotateSnakeHead(snake, DIR_UP);
        else if (wParam == VK_NUMPAD4)
          RotateSnakeHead(snake, DIR_LEFT);
        else if (wParam == VK_NUMPAD5)
          RotateSnakeHead(snake, DIR_DOWN);
      }

      /* Spawning snakes shortcuts */
      if (wParam == VK_F1)
      {
        if (GetSnakeByID(GameData, 0) == NULL)
          SpawnSnake(GameData, 0, SnakeInitLength, SnakeInitVelocity);
      }
      else if (wParam == VK_F2)
      {
        if (GetSnakeByID(GameData, 1) == NULL)
          SpawnSnake(GameData, 1, SnakeInitLength, SnakeInitVelocity);
      }
      else if (wParam == VK_F3)
      {
        if (GetSnakeByID(GameData, 2) == NULL)
          SpawnSnake(GameData, 2, SnakeInitLength, SnakeInitVelocity);
      }
      else if (wParam == VK_F4)
      {
        if (GetSnakeByID(GameData, 3) == NULL)
          SpawnSnake(GameData, 3, SnakeInitLength, SnakeInitVelocity);
      }
    }
    break;

/*
  case WM_LBUTTONDOWN:
    mX = (signed short)LOWORD(lParam);
    mY = (signed short)HIWORD(lParam);
    break;
    */

  case WM_DESTROY:
    /* Destroying created instruments */
    DeleteObject(instr.hBorderBrush);
    DeleteObject(instr.hClearBitmap);
    DeleteObject(instr.hFrogBitmap);
    DeleteObject(instr.hStoneBitmap);
    DeleteObject(instr.hWallBitmap);
    DeleteObject(instr.hSpeedBitmap);
    DeleteObject(instr.hSlowBitmap);
    for (i = 0; i < MAX_SNAKES; i++)
      DeleteObject(instr.hSnakeBitmaps[i]);

    /* Quiting */
    PostQuitMessage(0);
    break;

  /* Debuging... */
/*  case WM_TIMER:
    break;
  case WM_ACTIVATE:
  case WM_MOUSEMOVE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_GETMINMAXINFO:
  case WM_WINDOWPOSCHANGED:
    break;*/


  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
} /* End of 'WndProc' function */

/* END OF 'wndproc.c' FILE */
