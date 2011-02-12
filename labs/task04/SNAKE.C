/*
 * snake.c
 * Snake object definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 23.04.2007
 */

#include "typedefs.h"

#include "dir.h"
#include "timer.h"
#include "game.h"

/* Snakes describing constants */
extern double SnakeInitVelocity = 8;
extern double SnakeMaxVelocity = 100;
extern double SnakeMinVelocity = 100;
extern int SnakeInitLength = 7;

/* After this perioud snake will lose length */
double SnakeStarvingTime = 20.0;

/* Creating new snake object function */
Snake *CreateSnake( long id, int x, int y, int length, double velocity, Direction dir )
{
  Snake *snake;
  SnakePiece *prev, *piece;
  int i;
  
  /* Creating snake */
  if ((snake = malloc(sizeof(Snake))) == NULL)
  {
    perror("malloc");
    return NULL;
  }
  
  snake->id = id;
  snake->length = length;
  snake->velocity = velocity;
  snake->dir = dir;
  snake->lastUpdateTime = GetTicks();
  snake->lastFeedTime = snake->lastUpdateTime;
  
  /* Creating snake pieces */
  if ((prev = malloc(sizeof(SnakePiece))) == NULL)
  {
    perror("malloc");
    return NULL;
  }
  
  prev->snake = snake;
  prev->x = x;
  prev->y = y;
  prev->prev = NULL;
  
  snake->head = prev;
  
  for (i = 1; i < length; i++)
  {
    if ((piece = malloc(sizeof(SnakePiece))) == NULL)
    {
      perror("malloc");
      while (prev->prev != NULL)
      {
        prev = prev->prev;
        free(prev->next);
      }
      free(prev);
      return NULL;
    }
    
    piece->prev = prev;
    prev->next = piece;
    piece->snake = snake;
    piece->x = x;
    piece->y = y;
    
    prev = piece;
  }
  
  piece->next = NULL;
  snake->tail = piece;
  
  return snake;
} /* End of 'CreateSnake' function */

/* Destroying snake object function */
void DestroySnake( Snake *snake )
{
  SnakePiece *piece, *tmp;

  piece = snake->head;
  while (piece != NULL)
  {
    tmp = piece;
    piece = piece->next;
    free(tmp);
  }
  
  free(snake);
} /* End of 'DestroySnake' function */

/* Registering snake pieces on board function */
void RegisterSnake( BoardDescr *board, Snake *snake )
{
  SnakePiece *piece;

  piece = snake->tail;
  while (piece != NULL)
  {
    if (board->board[piece->x][piece->y].type == CELL_EMPTY)
    {
      board->board[piece->x][piece->y].type = CELL_SNAKE;
      board->board[piece->x][piece->y].object = (void *)piece;
    }
    piece = piece->prev;
  }
} /* End of 'RegisterSnake' function */

/* Unregistering snake pieces on board function */
void UnregisterSnake( BoardDescr *board, Snake *snake )
{
  SnakePiece *piece;

  piece = snake->tail;
  while (piece != NULL)
  {
    if (board->board[piece->x][piece->y].type == CELL_SNAKE)
    { 
      SnakePiece *cellPiece = (SnakePiece *)(board->board[piece->x][piece->y].object);
      
      if (cellPiece->snake->id == snake->id)
      {
        board->board[piece->x][piece->y].type = CELL_EMPTY;
        board->board[piece->x][piece->y].object = NULL;
      }
    }
    piece = piece->prev;
  }
} /* End of 'UnregisterSnake' function */

/* Moving snake on one cell with registering (without any checking) function */
void MoveSnake( BoardDescr *board, Snake *snake )
{
  int nx, ny;

  if (snake->length <= 0)
    return;
  
  nx = (BOARD_X_SIZE + snake->head->x + DirShiftX[snake->dir]) % BOARD_X_SIZE;
  ny = (BOARD_Y_SIZE + snake->head->y + DirShiftY[snake->dir]) % BOARD_Y_SIZE;
  
  /* Registering snake new head */
  /*if (board->board[nx][ny].type == CELL_EMPTY ||
    board->board[nx][ny].type == CELL_BONUS)
  {
    board->board[nx][ny].type = CELL_SNAKE;
    board->board[nx][ny].object = (void *)snake->head;
  }*/
  /* Unregistering snake old tail */
  /*if ((board->board[snake->tail->x][snake->tail->y].type == CELL_SNAKE &&
    ((SnakePiece *)board->board[snake->tail->x][snake->tail->y].object)->snake->id == snake->id) &&
    (snake->length == 1 || (snake->tail->x != snake->tail->prev->x || snake->tail->y != snake->tail->prev->y)))
  {
    board->board[snake->tail->x][snake->tail->y].type = CELL_EMPTY;
    board->board[snake->tail->x][snake->tail->y].object = NULL;
  }*/
  
  /* Moving snake */
  snake->tail->x = nx;
  snake->tail->y = ny;
  
  if (snake->length > 1)
  {
    snake->tail->next = snake->head;
    snake->head = snake->tail;
    snake->tail = snake->head->prev;
    snake->tail->next = NULL;
    snake->head->prev = NULL;
    snake->head->next->prev = snake->head;
  }
} /* End of 'MoveSnake' function */

/* Increasing snake length function */
int IncreaseSnakeLength( Snake *snake )
{
  SnakePiece *newPiece;
  
  if ((newPiece = malloc(sizeof(SnakePiece))) == NULL)
  {
    perror("malloc");
    return 1;
  }
  
  newPiece->snake = snake;
  newPiece->x = snake->tail->x;
  newPiece->y = snake->tail->y;
  newPiece->prev = snake->tail;
  newPiece->next = NULL;
  snake->tail = newPiece;
  
  snake->length++;
  
  return 0;
} /* End of 'IncreaseSnakeLength' function */

/* Decreasing snake length with reregistering function */
void DecreaseSnakeLength( BoardDescr *board, Snake *snake )
{
  if (snake->length == 1)
  {
    /* Unregistering snake piece */
    if (board->board[snake->head->x][snake->head->y].type == CELL_SNAKE &&
      ((SnakePiece *)board->board[snake->head->x][snake->head->y].object)->snake->id == snake->id)
    {
      board->board[snake->head->x][snake->head->y].type = CELL_EMPTY;
      board->board[snake->head->x][snake->head->y].object = NULL;
    }
    
    free(snake->head);
    snake->head = NULL;
    snake->tail = NULL;
  }
  else
  {
    /* Unregistering snake tail piece */
    if ((board->board[snake->tail->x][snake->tail->y].type == CELL_SNAKE &&
      ((SnakePiece *)board->board[snake->tail->x][snake->tail->y].object)->snake->id == snake->id) &&
      (snake->tail->x != snake->tail->prev->x || snake->tail->y != snake->tail->prev->y))
    {
      board->board[snake->tail->x][snake->tail->y].type = CELL_EMPTY;
      board->board[snake->tail->x][snake->tail->y].object = NULL;
    }
    else if (board->board[snake->tail->x][snake->tail->y].type == CELL_SNAKE &&
      ((SnakePiece *)board->board[snake->tail->x][snake->tail->y].object) == snake->tail)
    {
      board->board[snake->tail->x][snake->tail->y].object = (void *)snake->tail->prev;
    }
    
    snake->tail = snake->tail->prev;
    free(snake->tail->next);
    snake->tail->next = NULL;
  }

  snake->length--;
} /* End of 'DecreaseSnakeLength' function */

/* Increasing snake velocity function */
int IncreaseSnakeVelocity( Snake *snake )
{
  snake->velocity *= SpeedBonusFactor;
  if (snake->velocity > SnakeMaxVelocity)
    snake->velocity = SnakeMaxVelocity;

  return 0;
} /* End of 'IncreaseSnakeVelocity' function */

/* Decreasing snake velocity function */
int DecreaseSnakeVelocity( Snake *snake )
{
  snake->velocity *= SlowBonusFactor;
  if (snake->velocity > SnakeMinVelocity)
    snake->velocity = SnakeMinVelocity;

  return 0;
} /* End of 'DecreaseSnakeVelocity' function */

/* Updating single snake function.
 * Returning zero if snake is normally updated, positive if snake was destroyed
 * and negative if error was occured.
 */
SnakeUpdateResult UpdateSnake( GameDescr *game, Snake *snake )
{
  BoardDescr *board = &game->board;

  //TODO: Snake should be updated by period of time, not by function calling time point.
  Ticks ts = GetTicks();
  
  while (((double)(ts - snake->lastUpdateTime)) / TICKS_PER_SEC * snake->velocity >= 1)
  {
    int nx, ny;

    /* Now we will process time after move on one cell */
    Ticks nts = (Ticks)(snake->lastUpdateTime + TICKS_PER_SEC / snake->velocity);
  
    nx = (BOARD_X_SIZE + snake->head->x + DirShiftX[snake->dir]) % BOARD_X_SIZE;
    ny = (BOARD_Y_SIZE + snake->head->y + DirShiftY[snake->dir]) % BOARD_Y_SIZE;
    
    if (board->board[nx][ny].type == CELL_WALL || board->board[nx][ny].type == CELL_SNAKE)
    {
      /* Death move */
      return SUR_DEATH;
    }
    else if (board->board[nx][ny].type == CELL_BONUS)
    {
      /* Eating bonus */
      Bonus *bonus = (Bonus *)(board->board[nx][ny].object);
      
      RemoveBonus(game, bonus);
      UnregisterBonus(board, bonus);

      if (bonus->type == BONUS_FROG)
      {
        if (IncreaseSnakeLength(snake))
        {
          DestroyBonus(bonus);
          return SUR_DEATH;
        }
        snake->lastFeedTime = nts; //FIXME? Floating point precision loss.
      }
      else if (bonus->type == BONUS_STONE)
      {
        /* Decreasing snake length */
        if (snake->length == 1)
        {
          DestroyBonus(bonus);
          /* Empty snake => death */
          return SUR_DEATH;
        }
        DecreaseSnakeLength(board, snake);
      }
      else if (bonus->type == BONUS_SPEED)
      {
        IncreaseSnakeVelocity(snake);

        snake->lastFeedTime = nts; //FIXME? Floating point precision loss.
      }
      else if (bonus->type == BONUS_SLOW)
      {
        DecreaseSnakeVelocity(snake);

        snake->lastFeedTime = nts; //FIXME? Floating point precision loss.
      }
      DestroyBonus(bonus);
    }

    UnregisterSnake(board, snake);
    MoveSnake(board, snake);
    RegisterSnake(board, snake);
    snake->lastUpdateTime = nts; //FIXME? Floating point precision loss.
    
    /* If snake is starving */
    if (((double)(snake->lastUpdateTime - snake->lastFeedTime)) / TICKS_PER_SEC > SnakeStarvingTime)
    {
      /* Decreasing snake length */
      if (snake->length == 1)
      {
        /* Empty snake => death */
        return SUR_DEATH;
      }
      DecreaseSnakeLength(board, snake);
      snake->lastFeedTime = snake->lastUpdateTime;
    }
  }
  
  return SUR_NORMAL;
} /* End of 'UpdateSnake' function */

/* Rotate snake next move direction */
int RotateSnakeHead( Snake *snake, Direction dir )
{
  int nx, ny;
  
  nx = (BOARD_X_SIZE + snake->head->x + DirShiftX[dir]) % BOARD_X_SIZE;
  ny = (BOARD_Y_SIZE + snake->head->y + DirShiftY[dir]) % BOARD_Y_SIZE;

  if (snake->length > 1 &&
    snake->head->next->x == nx && snake->head->next->y == ny)
  {
    /* Illegal move */
    return 1;
  }
  else
    snake->dir = dir;

  return 0;
} /* End of 'RotateSnakeHead' function */

/* END OF 'snake.c' FILE */
