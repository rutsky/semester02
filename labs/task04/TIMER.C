/*
 * timer.c
 * Interface for work with time.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 22.04.2007
 */

#include "timer.h"

#include <stdio.h>

/* Number of ticks in second */
Ticks TICKS_PER_SEC = 0;

/* Initializing timer internal variables function */
int InitTimer( void )
{
  if (!QueryPerformanceFrequency((LARGE_INTEGER *)&TICKS_PER_SEC))
  {
    fprintf(stderr, "Error: QueryPerformance is not present.\n");
    return 1;
  }
  
  return 0;
}

/* Get the number of ticks since some time point */
Ticks GetTicks( void )
{
  Ticks tick;

  if (!QueryPerformanceCounter((LARGE_INTEGER *)&tick))
  {
    fprintf(stderr, "Error: QueryPerformanceCounter failed.\n");
    return 1;
  }
 
  return tick;
} /* End of 'GetTicks' function */

/* END OF 'timer.c' FILE */
