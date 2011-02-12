/*
 * random.h
 * Interface for work with random numbers.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 22.04.2007
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <stdlib.h>

/* Setting random seed function */
void SetRandom( int seed );

/* Returning floating point random number from 0 to 1 (1 is not included) */
double Random( void );

#endif _RANDOM_H_

/* END OF 'random.h' FILE */
