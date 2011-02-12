/*
 * snake.h
 * Snake object definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 23.04.2007
 */

#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "typedefs.h"

/* Snakes describing constants */
extern double SnakeInitVelocity;
extern int SnakeInitLength;
extern double SnakeMaxVelocity;
extern double SnakeMinVelocity;

typedef struct tagSnake Snake;
typedef struct tagSnakePiece SnakePiece;

struct tagSnakePiece
{
  /* Pointer to full snake object */
  Snake *snake; 
  /* Piece position on board */
  int x, y;
  /* Pointers to next and previous snake pieces */
  SnakePiece *prev; /* closer to snake head */
  SnakePiece *next; /* closer to snake tail */
};

struct tagSnake
{
  /* Snake owner identificator */
  long id;
  /* Pointers to head and tail snake pieces */
  SnakePiece *head, *tail;
  /* Snake length */
  int length;
  /* Snake velocity */
  double velocity;
  /* Snake head velocity direction */
  Direction dir;
  /* Last update time stamp */
  Ticks lastUpdateTime;
  /* Last snake "feed" time stamp */
  Ticks lastFeedTime;
};

typedef enum
{
  SUR_ERROR  = -1,
  SUR_NORMAL = 0,
  SUR_DEATH  = 1,
} SnakeUpdateResult;

/* Creating new snake object function */
Snake *CreateSnake( long id, int x, int y, int length, double velocity, Direction dir );

/* Destroying snake object function */
void DestroySnake( Snake *snake );

/* Registering snake pieces on board function */
void RegisterSnake( BoardDescr *board, Snake *snake );

/* Unregistering snake pieces on board function */
void UnregisterSnake( BoardDescr *board, Snake *snake );

/* Updating single snake function.
 * Returning zero if snake is normally updated, positive if snake was destroyed
 * and negative if error was occured.
 */
SnakeUpdateResult UpdateSnake( GameDescr *game, Snake *snake );

/* Rotate snake next move direction */
int RotateSnakeHead( Snake *snake, Direction dir );

#endif /* _SNAKE_H_ */

/* END OF 'snake.h' FILE */
