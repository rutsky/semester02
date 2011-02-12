/*
 * masserts.c
 * Second semester. Task 01.
 * Defensive programming strategy.
 * Assertion system.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 11.02.2007
 */

#include "masserts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximum number of asserts */
static int maxAsserts = 0;
/* Number of used asserts */
static int nAsserts = 0;
/* Asserts messages */
static char **asserts = NULL;
/* Is assert should be ignored */
static int *isIgnored;

/* Initinalizing asserts system function */
int AssertSystemOpen( int nAsserts )
{
  if ((asserts = malloc(sizeof(char *) * nAsserts)) == NULL)
  {
    perror("malloc");
    return 1;
  }
  if ((isIgnored = malloc(sizeof(int) * nAsserts)) == NULL)
  {
    perror("malloc");
    free(asserts);
    return 1;
  }
  maxAsserts = nAsserts;
  nAsserts = 0;
  
  return 0;
} /* End of 'SystemOpen' function */

/* Deinitializing asserts system function */
void AssertSystemClose( void )
{
  int i;

  for (i = 0; i < nAsserts; i++)
    free(asserts[i]);
  free(asserts);
  free(isIgnored);
} /* End of 'AssertSystemClose' function */

/* Assertion function */
int Assert( int condition, const char *message )
{
#ifdef _DEBUG
  int ind;
  
  /* Searching is this assertion was */
  for (ind = 0; ind < nAsserts; ind++)
    if (!strcmp(asserts[ind], message))
      break;
    
  if (ind >= nAsserts)
  {
    /* Trying to add new assertion message */
    if (ind >= maxAsserts)
    {
      fprintf(stderr, "Error: asserts limit exceeded.\n");
      return 1;
    }
  
    if ((asserts[ind] = malloc(sizeof(char) * (strlen(message) + 1))) == NULL)
    {
      perror("malloc");
      return 1;
    }
    strcpy(asserts[ind], message);
    isIgnored[ind] = 0;
    nAsserts++;
  }
  else if (isIgnored[ind])
    return 0;

  if (!condition)
  {
#define MAX_BUF 100
    char buf[MAX_BUF];
    int done = 0;
  
    fprintf(stderr, "Assertion failed: %s\n", message);
    while (!done)
    {
      fprintf(stderr, "[c]ontinue execution, [i]gnore this asserts or [a]bort program?\n");
      fgets(buf, MAX_BUF - 1, stdin);
      
      buf[MAX_BUF - 1] = 0;
      if (buf[strlen(buf) - 1] != '\n')
      {
        int ch;
      
        while ((ch = fgetc(stdin)) != EOF && ch != '\n')
          ;
      }
      
      if (buf[0] == 'c')
        done = 1;
      else if (buf[0] == 'i')
      {
        isIgnored[ind] = 1;
        done = 1;
      }
      else if (buf[0] == 'a')
        exit(1);
    }
#undef MAX_BUF
  }
#endif /* _DEBUG */
  
  return 0;
} /* End of 'Assert' function */

/* Error message handling function */
int AssertFatalError( int condition, const char *message, int returnValue )
{
  if (!condition)
  {
    fprintf(stderr, "Fatal error: %s\n", message);
    return returnValue;
  }

  return 0;
} /* End of 'AssertFatalError' function */

/* END OF 'masserts.c' FILE */
