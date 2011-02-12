/*
 * timer.h
 * Interface for work with time.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 22.04.2007
 */

#ifndef _TIMER_H_
#define _TIMER_H_

//TODO: This is windows timer
#include <windows.h>

/* Ticks type */
typedef INT64 Ticks;

/* Number of ticks in second */
extern Ticks TICKS_PER_SEC;

/* Initializing timer internal variables function */
int InitTimer( void );

/* Get the number of milliseconds since some time point */
Ticks GetTicks( void );

#endif _TIMER_H_

/* END OF 'timer.h' FILE */
