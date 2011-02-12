/*
 * masserts.h
 * Second semester. Task 01.
 * Defensive programming strategy.
 * Assertion system.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 11.02.2007
 */

#ifndef _masserts_h_
#define _masserts_h_

/* Initinalizing asserts system function */
int AssertSystemOpen( int nAsserts );

/* Deinitializing asserts system function */
void AssertSystemClose( void );

/* Assertion function */
int Assert( int condition, const char *message );

/* Error message handling function */
int AssertFatalError( int condition, const char *message, int returnValue );

#endif /* _masserts_h_ */

/* END OF 'masserts.h' FILE */
