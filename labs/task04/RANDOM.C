/*
 * random.c
 * Interface for work with random numbers.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#include "random.h"

#include <stdlib.h>

/* Setting random seed function */
void SetRandom( int seed )
{
  srand(seed);
} /* End of 'SetRandom' function */

/* Returning floating point random number from 0 to 1 (1 is not included) */
double Random( void )
{
  return rand() / (RAND_MAX + 1.0);
} /* End of 'Random' function */

/* END OF 'random.c' FILE */
