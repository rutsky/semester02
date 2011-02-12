/*
 * bonus.h
 * Bonus object definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 23.04.2007
 */

#ifndef _BONUS_H_
#define _BONUS_H_

#include "typedefs.h"

/* Spawn time in milliseconds */
extern long BonusSpawnTime;
extern double SpeedBonusFactor;
extern double SlowBonusFactor;

typedef enum
{
  BONUS_FROG = 0,
  BONUS_STONE,
  BONUS_SPEED,
  BONUS_SLOW,
  /* Number of bonuses */
  NUM_OF_BONUSES,
} BonusType;

/* Lifetime for each bonus type  */
extern double BonusLifeTime[NUM_OF_BONUSES];

typedef struct tagBonus
{
  /* Bonus type */
  BonusType type;
  /* Bonus on board creation time stamp */
  Ticks creationTime;
  /* Bonus position on board */
  int x, y;
} Bonus;

typedef enum
{
  BUR_ERROR = -1,
  BUR_NORMAL = 0,
  BUR_DEATH = 1,
} BonusUpdateResult;

/* Updating bonus function */
BonusUpdateResult UpdateBonus( Bonus *bonus );

/* Creating bonus structure function */
Bonus *CreateBonus( BonusType type, int x, int y );

/* Destroying bonus structure function */
void DestroyBonus( Bonus *bonus );

/* Registering bonus on board function */
void RegisterBonus( BoardDescr *board, Bonus *bonus );

/* Unregistering bonus on board function */
void UnregisterBonus( BoardDescr *board, Bonus *bonus );

#endif /* _BONUS_H_ */

/* END OF 'bonus.h' FILE */
