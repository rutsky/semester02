/*
 * main.c
 * Task 01.
 * Defensive programming strategy. Asserts.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 11.02.2007
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "masserts.h"

/* The main program function.
 * ARGUMENTS:
 *   - number of command line arguments:
 *       int argc;
 *   - array of command line arguments:
 *       char *argv[];
 * RETURNS:
 *   (int) program exit status.
 */
int main( int argc, char *argv[] )
{
  double res;
  int rn;
  
  if (AssertSystemOpen(20))
  {
    fprintf(stderr, "Assertion system initialization failed.\n");
    return 1;
  }

  srand(time(NULL));

  res = 1;
  while (1)
  {
    rn = rand() % 30;
    
    printf("  rn: %d, res: %lf\n", rn, res);
    getchar();
        
    Assert(!(rn < 10), "rn < 10");
    Assert(!(rn >= 10 && rn < 20), "10 <= rn < 20");
    Assert(!(rn > 20), "rn > 20");
    Assert(!(rn % 2), "rn % 2");
    Assert(!(rn % 5), "rn % 5");
    
    AssertFatalError(!(rn == 0 || rn == 15), "rn == 0 || rn == 15", 1);
    res = res / rn  + 1.0 / (rn - 15);
  }
  
  AssertSystemClose();

  return 0;
} /* End of 'main' function */

/* END OF 'main.c' FILE */
