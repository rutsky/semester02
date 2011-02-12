/*
 * game.c
 * Game handling main functions and data structures.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#include "typedefs.h"

/* Creating game description structure function */
GameDescr *CreateGame( void )
{
  GameDescr *game;

  if ((game = malloc(sizeof(GameDescr))) == NULL)
  {
    perror("malloc");
    return NULL;
  }

  if (SetBoard(&game->board))
  {
    free(game);
    return NULL;
  }
  game->nBonuses = 0;
  game->nSnakes = 0;

  return game;
} /* End of 'CreateGame' function */

/* Destroying game structure function */
void DestroyGame( GameDescr *game )
{
  int i;

  for (i = 0; i < game->nSnakes; i++)
    DestroySnake(game->snakes[i]);

  for (i = 0; i < game->nBonuses; i++)
    DestroyBonus(game->bonuses[i]);

  free(game);
} /* End of 'DestroyGame' function */

/* Adding snake to game description data function */
int AddSnake( GameDescr *game, Snake *snake )
{
  if (game->nSnakes >= MAX_SNAKES)  
    return 1;
  
  game->snakes[game->nSnakes] = snake;
  game->nSnakes++;

  return 0;
} /* End of 'AddSnake' function */

/* Removing snake from game description data function */
int RemoveSnake( GameDescr *game, Snake *snake )
{
  int i;

  for (i = 0; i < game->nSnakes; i++)
    if (game->snakes[i] == snake)  
    {
      game->snakes[i] = game->snakes[game->nSnakes - 1];
      game->nSnakes--;

      return 0;
    }
  
  return 1;
} /* End of 'RemoveSnake' function */

/* Adding bonus to game description data function */
int AddBonus( GameDescr *game, Bonus *bonus )
{
  if (game->nBonuses >= MAX_BONUSES)  
    return 1;
  
  game->bonuses[game->nBonuses] = bonus;
  game->nBonuses++;

  return 0;
} /* End of 'AddBonus' function */

/* Removing snake from game description data function */
int RemoveBonus( GameDescr *game, Bonus *bonus )
{
  int i;

  for (i = 0; i < game->nBonuses; i++)
    if (game->bonuses[i] == bonus)  
    {
      game->bonuses[i] = game->bonuses[game->nBonuses - 1];
      game->nBonuses--;

      return 0;
    }
  
  return 1;
} /* End of 'RemoveBonus' function */

/* Updating game function */
int UpdateGame( GameDescr *game )
{
  int i;

  /* Updating snakes */
  for (i = 0; i < game->nSnakes; i++)
  {
    SnakeUpdateResult res;
  
    if ((res = UpdateSnake(game, game->snakes[i])) == SUR_ERROR)
    {
      return 1;
    }
    else if (res == SUR_DEATH)
    {
      Snake *snake = game->snakes[i];

      RemoveSnake(game, snake);
      UnregisterSnake(&game->board, snake);
      DestroySnake(snake);
    }
  }
  
  /* Updating bonuses */
  for (i = 0; i < game->nBonuses; i++)
  {
    BonusUpdateResult res;
  
    if ((res = UpdateBonus(game->bonuses[i])) == BUR_ERROR)
    {
      return 1;
    }
    else if (res == BUR_DEATH)
    {
      Bonus *bonus = game->bonuses[i];

      RemoveBonus(game, bonus);
      UnregisterBonus(&game->board, bonus);
      DestroyBonus(bonus);
    }
  }
  
  return 0;
} /* End of 'UpdateGame' function */

/* Spawning one bonus in random place function */
int SpawnBonus( GameDescr *game, BonusType type )
{
  int x, y, f;
  Bonus *bonus;
  int i;
  
  if (game->nBonuses >= MAX_BONUSES)  
    return 1;
  
  /* Searching place to put bonus */
  x = (int)(Random() * BOARD_X_SIZE);
  y = (int)(Random() * BOARD_Y_SIZE);
  f = 0;
  
  for (i = 0; i < BOARD_X_SIZE * BOARD_Y_SIZE; i++)
  {
    if (game->board.board[x][y].type == CELL_EMPTY)
    {
      f = 1;
      break;
    }
    else
    {
      if (x == BOARD_X_SIZE - 1)
        y = (y + 1) % BOARD_Y_SIZE;
      x = (x + 1) % BOARD_X_SIZE;
    }
  }
    
  if (!f)
    return 1;
    
  /* Putting bonus */
  if ((bonus = CreateBonus(type, x, y)) == NULL)
    return 1;
    
  AddBonus(game, bonus);
  RegisterBonus(&game->board, bonus);
    
  return 0;
} /* End of 'SpawnBonus' function */

/* Spawning one random bonus in random place function */
int SpawnRandomBonus( GameDescr *game )
{
  return SpawnBonus(game, (BonusType)((int)NUM_OF_BONUSES * Random()));
} /* End of 'SpawnRandomBonus' function */

/* Spawning snake in random spawn point function */
int SpawnSnake( GameDescr *game, long id, int length, double velocity )
{
  int sp, f;
  Snake *snake;
  int x, y, i;
  
  if (game->nSnakes >= MAX_SNAKES)  
    return 1;
  
  /* Searching place to put snake */
  sp = (int)(Random() * game->board.nSpawnPoints);
  f = 0;
  
  for (i = 0; i < game->board.nSpawnPoints; i++)
  {
    x = game->board.spawnPoints[sp].x;
    y = game->board.spawnPoints[sp].y;
  
    if (game->board.board[x][y].type == CELL_EMPTY)
    {
      f = 1;
      break;
    }
    else
    {
      sp = (sp + 1) % game->board.nSpawnPoints;
    }
  }
    
  if (!f)
    return 1;
    
  /* Putting snake */
  if ((snake = CreateSnake(id, x, y, length, velocity, game->board.spawnPoints[sp].dir)) == NULL)
    return 1;
   
  AddSnake(game, snake);
  
  RegisterSnake(&game->board, snake);

  return 0;
} /* End of 'SpawnSnake' function */

/* Retrieving pointer to snake description structure by snake identificator */
Snake *GetSnakeByID( GameDescr *game, long id )
{
  int i;

  for (i = 0; i < game->nSnakes; i++)
    if (game->snakes[i]->id == id)
    {
      return game->snakes[i];
    }
  
  return NULL;
} /* End of 'GetSnakeByID' function */

/* END OF 'game.c' FILE */
