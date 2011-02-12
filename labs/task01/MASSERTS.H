/*
 * masserts.h
 * Task 01.
 * Defensive programming strategy.
 * Assertion system.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 11.02.2007
 */

#ifndef _masserts_h_
#define _masserts_h_

/* Initinalizing asserts system function */
int _AssertSystemOpen( int nAsserts );

/* Deinitializing asserts system function */
void _AssertSystemClose( void );

/* Assertion function */
int _AssertFunc( const char *message, const char *file, int line );

#ifdef NDEBUG

/* If no debug */

#define AssertSystemOpen(nAsserts) ((int)0)

#define AssertSystemClose() ((void)0)

#define Assert(condition, message) ((void)0)

#else /* NDEBUG */

/* if debug */

#define AssertSystemOpen(nAsserts) _AssertSystemOpen(nAsserts)

#define AssertSystemClose() _AssertSystemClose()

#define Assert(condition, message)                \
  do                                              \
  {                                               \
    if (!(condition))                             \
      _AssertFunc((message), __FILE__, __LINE__); \
  } while(0)

#endif /* NDEBUG */

/* Error message handling function */
#define AssertFatalError(condition, message, returnValue) \
  do                                                      \
  {                                                       \
    if (!(condition))                                     \
    {                                                     \
      fprintf(stderr, "Fatal error: %s\n", (message));    \
      getchar();                                          \
      return (returnValue);                               \
    }                                                     \
  } while (0)

#endif /* _masserts_h_ */

/* END OF 'masserts.h' FILE */
