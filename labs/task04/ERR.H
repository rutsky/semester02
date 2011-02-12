/*
 * err.h
 * Error handling function.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#ifndef _ERR_H_
#define _ERR_H_

#include <windows.h>

/* Outputting windows error message function.
 * From MSDN "Retrieving the Last-Error Code" example.
 */
void ErrorExit( LPTSTR lpszFunction );

#endif /* _ERR_H_ */

/* END OF 'err.h' FILE */
