/*
 * err.c
 * Error handling function.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 13.05.2007
 */

#include "err.h"

/* Outputting windows error message function.
 * From MSDN "Retrieving the Last-Error Code" example.
 */
void ErrorExit( LPTSTR lpszFunction )
{ 
  TCHAR szBuf[80]; 
  LPVOID lpMsgBuf;
  DWORD dw = GetLastError(); 

  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    dw,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&lpMsgBuf,
    0, NULL);

  wsprintf(szBuf, 
    "%s failed with error %d: %s", 
    lpszFunction, dw, lpMsgBuf); 

  MessageBox(NULL, szBuf, "Error", MB_OK); 

  LocalFree(lpMsgBuf);
  ExitProcess(dw); 
} /* End of 'ErrorExit' function */

/* END OF 'err.c' FILE */
