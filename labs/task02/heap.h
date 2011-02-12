/*
 * heap.h
 * Heap-like structure.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 02.03.2007
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

/* Note:
 * Heap base - number of children of each element.
 */

enum
{
/* Maximum level of depth
 * NOTE: This value limited by maximum long type number value. Long arithmetics requied...
 * FIXME: Also this is not exact value.
 */
  MAX_DEPTH = 31
};

/* Heap description data */
typedef struct
{
  void *data;   /* Heap data */
  int base;     /* Heap base */
  size_t size;  /* Size of heap element */
  int maxLevel; /* Maximum level value */
} Heap;

/* Calculating value of 'base ^ n' function */
long Power( long base, long n ); //FIXME: This function should be in other module, but...

/* Creating heap structure  */
Heap *CreateHeap( int base, size_t size, int maxDepth );

/* Destroying heap structure function */
void DestroyHeap( Heap *heap );

/* Retrieving number of elements on level function */
long GetLevelNElems( Heap *heap, int level );

/* Retrieving element by offset in heap function */
void *GetElemByOffset( Heap *heap, long offset );

/* Retrieving element by level and offset in level function */
void *GetElem( Heap *heap, int level, int n );

/* Retrieving element level and offset in level (with checking is element in current heap) function */
int GetPos( Heap *heap, void *elem, int *level, long *n );

/* Retrieving next to current element function */
void *GetNextElem( Heap *heap, void *elem );

/* Retrieving previous to current element function */
void *GetPrevElem( Heap *heap, void *elem );

/* Retrieving parent to current element function */
void *GetParentElem( Heap *heap, void *elem );

/* Retrieving first child to current element function */
void *GetFirstChildElem( Heap *heap, void *elem );

/* Retrieving next to current element function */
void *GetNextElemByPos( Heap *heap, int level, int n );

/* Retrieving previous to current element function */
void *GetPrevElemByPos( Heap *heap, int level, int n );

/* Retrieving parent to current element function */
void *GetParentElemByPos( Heap *heap, int level, int n );

/* Retrieving first child to current element function */
void *GetFirstChildElemByPos( Heap *heap, int level, int n );

#endif /* _HEAP_H_ */

/* END OF 'heap.h FILE */
