/*
 * heap.c
 * Heap-like structure.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 02.03.2007
 */

#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Note:
 * a + a ^ 2 + a ^ 3 + ... + a ^ n = a * (a ^ n - 1) / (a - 1)
 */

/* Calculating value of 'base ^ n' function */
long Power( long base, long n )
{
  long result = 1;

  while (n > 0)
  {
    if (n % 2)
    {
      result *= base;
      n--;
    }
    else
    {
      base *= base;
      n /= 2;
    }
  }
  
  return result;
} /* End of 'Power' function */

/* Calculating level offset in heap data function */
static long _getLevelOffset( int base, int level )
{
  if (level == 0)
    return 0;
  else
    return (long)(base * (Power(base, level) - 1.0) / (base - 1.0));
} /* End of '_getLevelOffset' function */

/* Calculating level and offset in level of element function */
static void _getPos( int base, long offset, int *level, long *n )
{
  int lev;

  lev = (int)(log(1 + offset * (base - 1.0) / base) / log(base));
  if (level != NULL)
   *level = lev;
  if (n != NULL)
    *n = offset - _getLevelOffset(base, lev);
} /* End of '_getPos' function */

/* Calculating offset in heap data of child to current element function */
static long _getChildOffset( int base, int level, long n )
{
  return _getLevelOffset(base, level + 1) + n * base;
} /* End of '_getChildOffset' function */

/* Calculating offset in heap data of parent to current element function */
static long _getParentOffset( int base, int level, long n )
{
  return _getLevelOffset(base, level - 1) + n / base;
} /* End of '_getParentOffset' function */

/* Creating heap structure  */
Heap *CreateHeap( int base, size_t size, int maxDepth )
{
  Heap *heap;
  
  if (maxDepth > MAX_DEPTH)
  {
    fprintf(stderr, "CreateHeap: maxDepth is limited with %d value.\n", MAX_DEPTH);
    return NULL;
  }
  
  if ((heap = malloc(sizeof(Heap))) == NULL)
  {
    perror("malloc");
    return NULL;
  }
  
  if ((heap->data = malloc(size * _getLevelOffset(base, maxDepth + 1))) == NULL)
  {
    perror("malloc");
    free(heap);
    return NULL;
  }
  
  heap->base = base;
  heap->size = size;

  heap->maxLevel = maxDepth;
  
  return heap;
} /* End of 'CreateHeap' function */

/* Destroying heap structure function */
void DestroyHeap( Heap *heap )
{
  free(heap->data);
  free(heap);
} /* End of 'DestroyHeap' function */

/* Retrieving number of elements on level function */
long GetLevelNElems( Heap *heap, int level )
{
  return Power(heap->base, level);
} /* End of 'GetLevelNElems' function */

/* Retrieving element by offset in heap function */
void *GetElemByOffset( Heap *heap, long offset )
{
  return (char *)heap->data + heap->size * offset;
} /* End of 'GetElemByOffset' function */

/* Retrieving element by level and offset in level function */
void *GetElem( Heap *heap, int level, int n )
{
  if (level < 0 || level > heap->maxLevel || n < 0 || n >= Power(heap->base, level))
    return NULL;

  return (char *)heap->data + heap->size * (_getLevelOffset(heap->base, level) + n);
} /* End of 'GetElem' function */

/* Retrieving element level and offset in level (with checking is element in current heap) function */
int GetPos( Heap *heap, void *elem, int *level, long *n )
{
  long offset = ((char *)elem - (char *)heap->data) / heap->size;

  if (offset >= 0 && offset <= _getLevelOffset(heap->base, heap->maxLevel + 1))
  {
    _getPos(heap->base, offset, level, n);
  
    return 0;
  }
  else
    return -1;
} /* End of 'GetPos' function */

/* Retrieving next to current element function */
void *GetNextElem( Heap *heap, void *elem )
{
  int level;
  long n;
  
  GetPos(heap, elem, &level, &n);
  
  return GetElem(heap, level, n + 1);
} /* End of 'GetNextElem' function */

/* Retrieving next to current element function */
void *GetNextElemByPos( Heap *heap, int level, int n )
{
  return GetElem(heap, level, n + 1);
} /* End of 'GetNextElemByPos' function */

/* Retrieving previous to current element function */
void *GetPrevElem( Heap *heap, void *elem )
{
  int level;
  long n;
  
  GetPos(heap, elem, &level, &n);
  
  return GetElem(heap, level, n - 1);
} /* End of 'GetPrevElem' function */

/* Retrieving previous to current element function */
void *GetPrevElemByPos( Heap *heap, int level, int n )
{
  return GetElem(heap, level, n - 1);
} /* End of 'GetPrevElemByPos' function */

/* Retrieving parent to current element function */
void *GetParentElem( Heap *heap, void *elem )
{
  int level;
  long n;
  
  GetPos(heap, elem, &level, &n);
  
  return GetElem(heap, level - 1, n / heap->base);
} /* End of 'GetParentElem' function */

/* Retrieving parent to current element function */
void *GetParentElemByPos( Heap *heap, int level, int n )
{
  return GetElem(heap, level - 1, n / heap->base);
} /* End of 'GetParentElemByPos' function */

/* Retrieving first child to current element function */
void *GetFirstChildElem( Heap *heap, void *elem )
{
  int level;
  long n;
  
  GetPos(heap, elem, &level, &n);
  
  return GetElem(heap, level + 1, n * heap->base);
} /* End of 'GetFirstChildElem' function */

/* Retrieving first child to current element function */
void *GetFirstChildElemByPos( Heap *heap, int level, int n )
{
  return GetElem(heap, level + 1, n * heap->base);
} /* End of 'GetFirstChildElemByPos' function */

/* END OF 'heap.c' FILE */
