/*
 * statistics.c
 * Counting file statistics.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 19.03.2007
 */

#include "statistics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reading long style '/ *  * /' comments from file pointer function */
static int _readLongComments( FILE *fp, Statistics *st, long *lineLen )
{
  int ch, och = 0;
  
  while ((ch = fgetc(fp)) != EOF)
  {
    st->file_size++;
    st->comments_size++;
    (*lineLen)++;

    /* Is line feed */
    if (ch == '\n')
    {
      st->nlines++;
      if ((*lineLen) > st->maxline_len)
        st->maxline_len = (*lineLen) - 1;
      (*lineLen) = 0;
    }
    else if (och == '*' && ch == '/')
    {
      break;
    }
          
    och = ch;
  }
  
  return 0;
} /* End of '_readLongComments' function */

/* Reading short style '/ /  ' comments from file pointer function */
static int _readShortComments( FILE *fp, Statistics *st, long *lineLen )
{
  int ch;
  
  while ((ch = fgetc(fp)) != EOF)
  {
    st->file_size++;
    st->comments_size++;
    (*lineLen)++;

    /* Is line feed */
    if (ch == '\n')
    {
      st->nlines++;
      if ((*lineLen) > st->maxline_len)
        st->maxline_len = (*lineLen) - 1;
      (*lineLen) = 0;
      break;
    }
    /* Macroprocessor line concatenation */
    else if (ch == '\\')
    {
      while ((ch = fgetc(fp)) != EOF)
      {
        st->file_size++;
        st->comments_size++;
        (*lineLen)++;
           
        /* If it is macro concatenation than skipping all untill '\n',
         * otherwise we will found non-space character before '\n' */
        if (ch == '\n')
        {
          st->nlines++;
          if ((*lineLen) > st->maxline_len)
            st->maxline_len = (*lineLen) - 1;
          (*lineLen) = 0;
          break;
        }
        else if (ch != ' ' && ch != '\t')
        {
          break;
        }
      }
    }
  }
  
  return 0;
} /* End of '_readShortComments' function */

/* Collecting statistics function */
int CollectStatistics( FILE *fp, Statistics *st )
{
  int ch;
  long lineLen = 0;
  
  st->file_size = 0;
  st->comments_size = 0;
  st->nlines = 1;
  st->maxline_len = 0;
  
  while ((ch = fgetc(fp)) != EOF)
  {
    st->file_size++;
    lineLen++;
    
    /* Is line feed */
    if (ch == '\n')
    {
      st->nlines++;
      if (lineLen > st->maxline_len)
        st->maxline_len = lineLen - 1;
      lineLen = 0;
    }
    /* Probably comment */
    else if (ch == '/')
    {
      if ((ch = fgetc(fp)) == EOF)
        break;
      st->file_size++;
      lineLen++;
      
      /* Is line feed */
      if (ch == '\n')
      {
        st->nlines++;
        if (lineLen > st->maxline_len)
          st->maxline_len = lineLen - 1;
        lineLen = 0;
      }
      /* Reading long '/ *   * /' comment */
      else if (ch == '*')
      {
        int err;
        
        st->comments_size += 2; /* '/ *' */
      
        if ((err = _readLongComments(fp, st, &lineLen)) != 0)
          return err;
      }
      /* Reading short '/ /  ' comment */
      else if (ch == '/')
      {
        int err;
      
        st->comments_size += 2; /* '/ /' */
        
        if ((err = _readShortComments(fp, st, &lineLen)) != 0)
          return err;
      }
    }
  }
  
  /* Is logically this needed? */
  /* If file is empty 'nlines' is equal to zero,
   * otherwise 'nlines' equal to number of line terminating symbol plus one */
  /*
  if (st->file_size == 0)
    st->nlines = 0;
  */
  
  return 0;
} /* End of 'CollectStatistics' function */

/* END OF 'statistics.c' FILE */
