/*
 * masserts.c
 * Task 01.
 * Defensive programming strategy.
 * Assertion system.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 11.02.2007
 */

#include "masserts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 100

/* Maximum number of asserts */
int maxAsserts = 0;
/* Number of used asserts */
int nAsserts = 0;
/* Asserts __FILE__ strings */
char **asserts_file = NULL;
/* Asserts __LINE__ strings */
int *asserts_line = NULL;
/* Is assert should be ignored */
int *isIgnored;
/* Is all asserts should be ignored */
int isAllAssertsIgnored = 0;

/* Initinalizing asserts system function */
int _AssertSystemOpen( int nAsserts )
{
  if ((asserts_file = malloc(sizeof(char *) * nAsserts)) == NULL)
  {
    perror("malloc");
    return 1;
  }
  if ((asserts_line = malloc(sizeof(int) * nAsserts)) == NULL)
  {
    perror("malloc");
    free(asserts_file);
    return 1;
  }
  if ((isIgnored = malloc(sizeof(int) * nAsserts)) == NULL)
  {
    perror("malloc");
    free(asserts_file);
    free(asserts_line);
    return 1;
  }
  maxAsserts = nAsserts;
  nAsserts = 0;
  
  return 0;
} /* End of 'SystemOpen' function */

/* Deinitializing asserts system function */
void _AssertSystemClose( void )
{
  int i;

  for (i = 0; i < nAsserts; i++)
    free(asserts_file[i]);
  free(asserts_file);
  free(asserts_line);
  free(isIgnored);
} /* End of 'AssertSystemClose' function */

/* Assertion function */
int _AssertFunc( const char *message, const char *file, int line )
{
  char buf[MAX_BUF];
  int done = 0;
  int ind;
  
  if (isAllAssertsIgnored)
    return 0;

  /* Searching is this assertion was */
  for (ind = 0; ind < nAsserts; ind++)
    if (!strcmp(asserts_file[ind], file) && asserts_line[ind] == line)
      break;
    
  if (ind >= nAsserts)
  {
    /* Trying to add new assertion message */
    if (ind >= maxAsserts)
    {
      fprintf(stderr, "Error: asserts limit exceeded.\n");
      return 1;
    }
  
    if ((asserts_file[ind] = malloc(sizeof(char) * (strlen(file) + 1))) == NULL)
    {
      perror("malloc");
      return 1;
    }
    strcpy(asserts_file[ind], file);
    asserts_line[ind] = line;
    isIgnored[ind] = 0;
    nAsserts++;
  }
  else if (isIgnored[ind])
    return 0;

  fprintf(stderr, "Assertion failed: %s:%d %s\n", file, line, message);
  while (!done)
  {
    fprintf(stderr, "[c]ontinue, [i]gnore, i[g]nore all or [a]bort program?\n");
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
    else if (buf[0] == 'g')
    {
      isAllAssertsIgnored = 1;
      done = 1;
    }
    else if (buf[0] == 'a')
      exit(1);
  }
  
  return 0;
} /* End of 'AssertFunc' function */

/* END OF 'masserts.c' FILE */
