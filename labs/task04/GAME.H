/*
 * game.h
 * Game handling main functions and data structures.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 23.04.2007
 */

#ifndef _GAME_H_
#define _GAME_H_

#include "typedefs.h"

enum
{
  /* Maximum number of snakes on board */
  MAX_SNAKES = 4,
  /* Maximum number of bonuses */
  MAX_BONUSES = 100,
};

typedef struct tagGameDescr
{
  /* Collision and rendering board */
  BoardDescr board;
  
  /* Board snakes */
  int nSnakes;
  Snake *snakes[MAX_SNAKES];
  
  /* Board bonuses */
  int nBonuses;
  Bonus *bonuses[MAX_BONUSES];
} GameDescr;

/* Creating game description structure function */
GameDescr *CreateGame( void );

/* Destroying game structure function */
void DestroyGame( GameDescr *game );

/* Updating game function */
int UpdateGame( GameDescr *game );

/* Spawning one bonus in random place function */
int SpawnBonus( GameDescr *game, BonusType type );

/* Spawning one random bonus in random place function */
int SpawnRandomBonus( GameDescr *game );

/* Spawning snake in random spawn point function */
int SpawnSnake( GameDescr *game, long id, int length, double velocity );

/* Retrueving pointer to snake description structure by snake identificator */
Snake *GetSnakeByID( GameDescr *game, long id );

/* Adding snake to game description data function */
int AddSnake( GameDescr *game, Snake *snake );

/* Removing snake from game description data function */
int RemoveSnake( GameDescr *game, Snake *snake );

/* Adding bonus to game description data function */
int AddBonus( GameDescr *game, Bonus *bonus );

/* Removing snake from game description data function */
int RemoveBonus( GameDescr *game, Bonus *bonus );

#endif /* _GAME_H_ */

/* END OF 'game.h' FILE */
