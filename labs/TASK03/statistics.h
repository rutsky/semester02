/*
 * statistics.h
 * Counting file statistics.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 19.03.2007
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <stdio.h>

/* File statistics structure */
typedef struct
{
  long file_size;     /* File size */
  long nlines;        /* Number of lines in file */
  long comments_size; /* Total size of comments in bytes */
  long maxline_len;  /* Longest line length */
} Statistics;

/* Collecting statistics function */
int CollectStatistics( FILE *fp, Statistics *st );

#endif /* _STATISTICS_H_ */

/* END OF 'statistics.h' FILE */
