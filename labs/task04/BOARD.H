/*
 * board.h
 * Rendering and collision board object definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 23.04.2007
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "typedefs.h"

#include <stdio.h>

/* Constants definitions */
enum
{
  /* Board size */
  BOARD_X_SIZE = 61,
  BOARD_Y_SIZE = 40,
  /* Maximum snake spawn points */
  MAX_SPAWN_POINTS = 30,
};

typedef enum
{
  CELL_EMPTY = 0, /* Empty cell */
  CELL_WALL,      /* Impenetrable wall */
  CELL_SNAKE,     /* Impenetrable snake piece */
  CELL_BONUS,     /* Penetrable bonus */
} CellType;

typedef struct
{
  /* Cell type */
  CellType type;
  /* Pointer to object, describing this cell.
   * Can be one of:
   *   (SnakePiece *) - for CELL_SNAKE
   *   (Bonus *)      - for CELL_BONUS
   */
  void *object;
} Cell;

typedef struct 
{
  int x, y;
  Direction dir;
} SnakeSpawnPoint;

typedef struct tagBoardDescr
{
  /* Rendering and collision board */
  Cell board[BOARD_X_SIZE][BOARD_Y_SIZE];

  /* Spawn points for snakes */
  int nSpawnPoints;
  SnakeSpawnPoint spawnPoints[MAX_SPAWN_POINTS];
} BoardDescr;

/* Setting board to initial state */
int SetBoard( BoardDescr *board );

#endif /* _BOARD_H_ */

/* END OF 'board.h' FILE */
