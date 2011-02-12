/*
 * board.c
 * Rendering and collision board object definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 23.04.2007
 */

#include "typedefs.h"

/* Setting board to initial state */
int SetBoard( BoardDescr *board )
{
  int x, y;
  
  for (y = 0; y < BOARD_Y_SIZE; y++)
    for (x = 0; x < BOARD_X_SIZE; x++)
    {
      if (((x == 0 || x == BOARD_X_SIZE - 1) && !(y % 3)) ||
        ((y == 0 || y == BOARD_Y_SIZE - 1) && !(x % 3)))
        board->board[x][y].type = CELL_WALL;
      else
        board->board[x][y].type = CELL_EMPTY;
      board->board[x][y].object = NULL;
    }
    
  board->nSpawnPoints = 4;
  
  board->spawnPoints[0].x = 1;
  board->spawnPoints[0].y = BOARD_Y_SIZE / 2;
  board->spawnPoints[0].dir = DIR_RIGHT;
  board->board[0][BOARD_Y_SIZE / 2].type = CELL_EMPTY;
  
  board->spawnPoints[1].x = BOARD_X_SIZE - 2;
  board->spawnPoints[1].y = BOARD_Y_SIZE / 2;
  board->spawnPoints[1].dir = DIR_LEFT;
  board->board[BOARD_X_SIZE - 1][BOARD_Y_SIZE / 2].type = CELL_EMPTY;
  
  board->spawnPoints[2].x = BOARD_X_SIZE / 2;
  board->spawnPoints[2].y = 1;
  board->spawnPoints[2].dir = DIR_UP;
  board->board[BOARD_X_SIZE / 2][0].type = CELL_EMPTY;
  
  board->spawnPoints[3].x = BOARD_X_SIZE / 2;
  board->spawnPoints[3].y = BOARD_Y_SIZE - 2;
  board->spawnPoints[3].dir = DIR_DOWN;
  board->board[BOARD_X_SIZE / 2][BOARD_Y_SIZE - 1].type = CELL_EMPTY;

  
  
  return 0;
} /* End of 'SetBoard' function */

/* Loading board from file function */
int LoadBoard( FILE *fp, BoardDescr *board )
{
  return 1;
  //FIXME
} /* End of 'LoadBoard' function */

/* END OF 'board.c' FILE */
