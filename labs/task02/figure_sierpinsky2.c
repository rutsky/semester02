/*
 * figure_sierpinsky2.c
 * Sierpinsky fractal (2) implementation
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 02.03.2007
 */

#include "figure_sierpinsky2.h"

#include <stdio.h>

#include "figure.h"
#include "geom.h"
#include "heap.h"
#include "vector.h"

/* Sierpinski triangle data structure */
typedef struct
{
  Heap *heap;

  /* First triangle point */
  Vector start;
  /* Two base edges of triangle (vectors) */
  Vector v1, v2;
} SierpinskyData;

typedef struct
{
  /* Position of first triangle point in current triangle */
  Vector v;
} SierpinskyHeapData;

/* Increasing level of detail function */
static int _sierpinskyLevelDown( Figure *fig )
{
  long i, nelems;
  SierpinskyHeapData *levelData;
  Vector tmp;
  double pow;

  if (fig->curLoD >= fig->maxLoD)
    return -1;
  
  nelems = GetLevelNElems(((SierpinskyData *)fig->data)->heap, fig->curLoD);
  levelData = (SierpinskyHeapData *)GetElem(((SierpinskyData *)fig->data)->heap, fig->curLoD, 0);
  pow = Power(2, fig->curLoD + 1);
  for (i = 0; i < nelems; i++)
  {
    SierpinskyHeapData *p;
    
    p = (SierpinskyHeapData *)GetFirstChildElemByPos(((SierpinskyData *)fig->data)->heap, fig->curLoD, i);
    
    p[0].v = levelData[i].v;
    
    p[1].v = levelData[i].v;
    tmp = ((SierpinskyData *)fig->data)->v1;
    VecDiv(&tmp, pow);
    VecAdd(&(p[1].v), &tmp);
    
    p[2].v = levelData[i].v;
    tmp = ((SierpinskyData *)fig->data)->v2;
    VecDiv(&tmp, pow);
    VecAdd(&(p[2].v), &tmp);
  }
  
  fig->curLoD++;
  
  return 0;
} /* End of '_sierpinskyLevelDown' function */

/* Render full figure lines to array.
 * Returning number of writed to array lines.
 */
static int SierpinskyRenderLines( Figure *fig, Line *buffer, int nlines, int level )
{
  int l, i;
  long lcounter = 0;
  Vector t1, t2;

  if (level > fig->maxLoD)
    level = fig->maxLoD;

  /* Increasing LoD to needed level */
  for (i = fig->curLoD; i < level; i++)
    _sierpinskyLevelDown(fig);
  
  if (nlines <= 0)
    return 0;
  
  /* Rendering base triangle */
  t1 = ((SierpinskyData *)fig->data)->v1;
  VecAdd(&t1, &(((SierpinskyData *)fig->data)->start));
  t2 = ((SierpinskyData *)fig->data)->v2;
  VecAdd(&t2, &(((SierpinskyData *)fig->data)->start));
  
  buffer[lcounter].x0 = (int)((SierpinskyData *)fig->data)->start.x;
  buffer[lcounter].y0 = (int)((SierpinskyData *)fig->data)->start.y;
  buffer[lcounter].x1 = (int)t1.x;
  buffer[lcounter++].y1 = (int)t1.y;
  if (lcounter >= nlines)
    return lcounter;
    
  buffer[lcounter].x0 = (int)((SierpinskyData *)fig->data)->start.x;
  buffer[lcounter].y0 = (int)((SierpinskyData *)fig->data)->start.y;
  buffer[lcounter].x1 = (int)t2.x;
  buffer[lcounter++].y1 = (int)t2.y;
  if (lcounter >= nlines)
    return lcounter;
  
  buffer[lcounter].x0 = (int)t1.x;
  buffer[lcounter].y0 = (int)t1.y;
  buffer[lcounter].x1 = (int)t2.x;
  buffer[lcounter++].y1 = (int)t2.y;
  if (lcounter >= nlines)
    return lcounter;

  /* Rendering triangles from all left levels */
  for (l = 0; l < level; l++)
  {
    int nelems;
    double pow;
  
    nelems = GetLevelNElems(((SierpinskyData *)fig->data)->heap, l);
    pow = Power(2, l + 1);
    for (i = 0; i < nelems; i++)
    {
      SierpinskyHeapData *p;
      Vector t1, t2, t3;
    
      p = (SierpinskyHeapData *)GetFirstChildElemByPos(((SierpinskyData *)fig->data)->heap, l, i);
      
      t1 = ((SierpinskyData *)fig->data)->v1;
      VecDiv(&t1, pow);
      t2 = ((SierpinskyData *)fig->data)->v2;
      VecDiv(&t2, pow);
      t3 = t1;
      VecAdd(&t3, &t2);
            
      VecAdd(&t1, &(p->v));
      VecAdd(&t2, &(p->v));
      VecAdd(&t3, &(p->v));

      buffer[lcounter].x0 = (int)t1.x;
      buffer[lcounter].y0 = (int)t1.y;
      buffer[lcounter].x1 = (int)t2.x;
      buffer[lcounter++].y1 = (int)t2.y;
      if (lcounter >= nlines)
        return lcounter;
      
      buffer[lcounter].x0 = (int)t2.x;
      buffer[lcounter].y0 = (int)t2.y;
      buffer[lcounter].x1 = (int)t3.x;
      buffer[lcounter++].y1 = (int)t3.y;
      if (lcounter >= nlines)
        return lcounter;

      buffer[lcounter].x0 = (int)t3.x;
      buffer[lcounter].y0 = (int)t3.y;
      buffer[lcounter].x1 = (int)t1.x;
      buffer[lcounter++].y1 = (int)t1.y;
      if (lcounter >= nlines)
        return lcounter;
    }
  }
  
  return lcounter;
} /* End of 'SierpinskyRenderLines' function */

/* Initializing Sierpinski triangle figure function */
Figure *SierpinskyFigInit( int maxLoD, Point *points )
{
  Figure *fig;
  SierpinskyHeapData *p;

  /* Allocating memory for figure */
  if ((fig = malloc(sizeof(Figure))) == NULL)
  {
    perror("malloc");
    return NULL;
  }
  
  /* Filling figure structure */
  fig->maxLoD = maxLoD;
  fig->curLoD = 0;
  fig->RenderLines = SierpinskyRenderLines;
  
  /* Allocating memory for figure data */
  if ((fig->data = (SierpinskyData *)malloc(sizeof(SierpinskyData))) == NULL)
  {
    perror("malloc");
    free(fig);
    return NULL;
  }
  
  /* Filling figure data */
  if ((((SierpinskyData *)fig->data)->heap = CreateHeap(3, sizeof(SierpinskyHeapData), maxLoD + 1)) == NULL)
  {
    perror("malloc");
    free(fig->data);
    free(fig);
    return NULL;
  }
  
  VecSet(&((SierpinskyData *)fig->data)->start, points[0].x, points[0].y);
  VecSet(&((SierpinskyData *)fig->data)->v1, points[1].x, points[1].y);
  VecSub(&((SierpinskyData *)fig->data)->v1, &((SierpinskyData *)fig->data)->start);
  VecSet(&((SierpinskyData *)fig->data)->v2, points[2].x, points[2].y);
  VecSub(&((SierpinskyData *)fig->data)->v2, &((SierpinskyData *)fig->data)->start);
  
  p = (SierpinskyHeapData *)GetElemByOffset(((SierpinskyData *)fig->data)->heap, 0);
  p->v = ((SierpinskyData *)fig->data)->start;
  
  return fig;
} /* End of 'SierpinskyFigInit' function */

/* Destroying Sierpinsky figure data function */
void SierpinskyFigDestroy( Figure *fig )
{
  DestroyHeap(((SierpinskyData *)fig->data)->heap);
  free(fig->data);
  free(fig);
} /* End of 'SierpinskyFigDestroy' function */

/* END OF 'figure_sierpinsky2.c' FILE */
