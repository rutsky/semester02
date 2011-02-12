/*
 * bonus.c
 * Bonus object definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#include "typedefs.h"

/* Spawn time in milliseconds */
extern long BonusSpawnTime = 1000;
extern double SpeedBonusFactor = 1.3;
extern double SlowBonusFactor = 1 / 1.3;

/* Lifetime for each bonus type  */
extern double BonusLifeTime[NUM_OF_BONUSES] =
  {
    /* Frog */
    13.0,
    /* Stone */
    7.0,
    /* Speed */
    13.0,
    /* Slow */
    13.0,
  };

/* Creating bonus structure function */
Bonus *CreateBonus( BonusType type, int x, int y )
{
  Bonus *bonus;
  
  if ((bonus = malloc(sizeof(Bonus))) == NULL)
  {
    perror("malloc");
    return NULL;
  }
  
  bonus->type = type;
  bonus->creationTime = GetTicks();
  bonus->x = x;
  bonus->y = y;
  
  return bonus;
} /* End of 'CreateBonus' function */

/* Destroying bonus structure function */
void DestroyBonus( Bonus *bonus )
{
  free(bonus);
} /* End of 'DestroyBonus' function */

/* Registering bonus on board function */
void RegisterBonus( BoardDescr *board, Bonus *bonus )
{
  if (board->board[bonus->x][bonus->y].type == CELL_EMPTY)
  {
    board->board[bonus->x][bonus->y].type = CELL_BONUS;
    board->board[bonus->x][bonus->y].object = (void *)bonus;
  }
} /* End of 'RegisterBonus' function */

/* Unregistering bonus on board function */
void UnregisterBonus( BoardDescr *board, Bonus *bonus )
{
  if (board->board[bonus->x][bonus->y].type == CELL_BONUS &&
    board->board[bonus->x][bonus->y].object == (void *)bonus)
  { 
    board->board[bonus->x][bonus->y].type = CELL_EMPTY;
    board->board[bonus->x][bonus->y].object = NULL;
  }
} /* End of 'UnregisterBonus' function */

/* Updating bonus function */
BonusUpdateResult UpdateBonus( Bonus *bonus )
{
  Ticks ts = GetTicks();

  if (((double)(ts - bonus->creationTime)) / TICKS_PER_SEC > BonusLifeTime[bonus->type])
    return BUR_DEATH;
    
  return BUR_NORMAL;
} /* End of 'BonusUpdate' function */

/* END OF 'bonus.c' FILE */
