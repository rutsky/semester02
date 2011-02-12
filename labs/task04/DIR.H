/*
 * dir.h
 * Direction stuff definition.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#ifndef _DIR_H_
#define _DIR_H_

typedef enum
{
  DIR_RIGHT = 0,
  DIR_UP,
  DIR_LEFT,
  DIR_DOWN,
} Direction;

/* Axes shifts on translating along direction */
extern int DirShiftX[4], DirShiftY[4];

#endif /* _DIR_H_ */

/* END OF 'dir.h' FILE */
