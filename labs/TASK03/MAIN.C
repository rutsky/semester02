/*
 * main.c
 * Task 03. I-5.
 * Code statistics counting program.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 19.03.2007
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <time.h>
#include <direct.h>
 
#include "statistics.h"

/* Maximum directory depth */
#define MAX_DEPTH 100

/* Printing to stream usage message function */
void printUsage( FILE *fp, char *programName )
{
  fprintf(fp, "Collecting statistics for *.c, *.cpp, *.h files program.\n"
    "Usage:\n"
    "  %s [keys] directories\n"
    "Keys:\n"
    "  -? or /? - this help\n"
    "\n",
    programName);
} /* End of 'printUsage' function */

typedef struct 
{
  int nFiles;   /* Number of files */
  int nLines;   /* Total number of lines in files */
  int maxLines; /* Maximum number of lines in file */
} DirInfo;

int debugChdir( const char *path )
{
  int error;

  if (error = _chdir(path))
  {
#define MAX_BUF 1000
    //FIXME: Not safe!
    char buf[MAX_BUF];

    strcpy(buf, "_chdir(");
    strcat(buf, path);
    strcat(buf, ")");

    perror(buf);

    return error;
  }

  return error;
} /* End of 'debugChdir' function */

/* Recursively collecting statistics function */
int RecursivelyCollectStatistics( const char *path, Statistics *allSt,
                                  DirInfo *c_files, DirInfo *cpp_files, DirInfo *h_files,
                                  char **relativePaths, int depth, int maxDepth )
{
  struct _finddata_t findData;
  long hFile;
  char *findPatterns[] = { "*.c", "*.cpp", "*.h" };
  DirInfo *dirsInfo[] = { c_files, cpp_files, h_files };
  int i;
  char oldPath[_MAX_PATH];
  
  for (i = 0; i < depth; i++)
    fprintf(stdout, "%s\\", relativePaths[i]);
  fprintf(stdout, "%s\\\n", path);

  /* Adding current path to relative path */
  if ((relativePaths[depth] = malloc(sizeof(char) * (strlen(path) + 1))) == NULL)
  {
    perror("malloc");
    return 1;
  }
  strcpy(relativePaths[depth], path);

  /* Saving original path */
  _getcwd(oldPath, _MAX_PATH);

  /* Moving to requested path */
  if (debugChdir(path))
  {
    return 1;
  }

  /* Recursively processing directories */
  if (depth + 1 < maxDepth && (hFile = _findfirst("*.*", &findData)) != -1L)
  {
    do
    {
      if (findData.attrib == _A_SUBDIR && !(!strcmp(findData.name, ".") || !strcmp(findData.name, "..")))
      {
        int error;

        if (error = RecursivelyCollectStatistics(findData.name, allSt, c_files, cpp_files, h_files,
          relativePaths, depth + 1, maxDepth))
          return error;
      }
    } while (_findnext(hFile, &findData) == 0);
    _findclose(hFile);
  }

  /* Processing files */
  for (i = 0; i < sizeof(findPatterns) / sizeof(findPatterns[0]); i++)
  {
    if ((hFile = _findfirst(findPatterns[i], &findData)) != -1L)
    {
      do
      {
        if (findData.attrib != _A_SUBDIR)
        {
          FILE *fp;
          Statistics st;

          if ((fp = fopen(findData.name, "rt")) == NULL)
          {
            perror("fopen");
          }
          else
          {
            CollectStatistics(fp, &st);

            fprintf(stdout, "%s: %ld B, %ld lines, %ld B of comments ",
              findData.name, st.file_size, st.nlines, st.comments_size);
            if (st.file_size != 0)
              fprintf(stdout, "(%.0lf%%), ",
                100.0 * st.comments_size / st.file_size);
            if (st.nlines != 0)
              fprintf(stdout, "avg. line %.2lf B.\n",
                (double)st.file_size / st.nlines);

            if (dirsInfo[i]->maxLines < st.nlines)
              dirsInfo[i]->maxLines = st.nlines;
            dirsInfo[i]->nFiles++;
            dirsInfo[i]->nLines += st.nlines;

            allSt->comments_size += st.comments_size;
            allSt->file_size += st.file_size;
            if (allSt->maxline_len < st.maxline_len)
              allSt->maxline_len = st.maxline_len;
            allSt->nlines += st.nlines;

            fclose(fp);
          }
        }
      } while (_findnext(hFile, &findData) == 0);
      _findclose(hFile);
    }
  }

  /* Restoring original path */
  if (debugChdir(oldPath))
  {
    return 1;
  }

  free(relativePaths[depth]);

  return 0;
} /* End of 'RecursivelyCollectStatistics' function */

/* The main program function.
 * ARGUMENTS:
 *   - number of command line arguments:
 *       int argc;
 *   - array of command line arguments:
 *       char *argv[];
 * RETURNS:
 *   (int) program exit status.
 */
int main( int argc, char *argv[] )
{
  int i;
  DirInfo c_files, cpp_files, h_files;
  int isSearched = 0;
  char *relativePaths[MAX_DEPTH];
  Statistics st;

  memset(&c_files, 0, sizeof(DirInfo));
  memset(&cpp_files, 0, sizeof(DirInfo));
  memset(&h_files, 0, sizeof(DirInfo));
  memset(&st, 0, sizeof(Statistics));
  
  if (argc <= 1)
  {
    printUsage(stdout, argv[0]);
  }
  else
  {
    for (i = 1; i < argc; i++)
    {
      if (!strcmp(argv[i], "-?") || !strcmp(argv[i], "/?"))
      {
        printUsage(stdout, argv[0]);
      }
      else
      {
        RecursivelyCollectStatistics(argv[i], &st, &c_files, &cpp_files, &h_files,
          relativePaths, 0, MAX_DEPTH);
      }
    }

    fprintf(stdout,
      "\n" 
      "Total:\n"
      "%ld B, %ld lines, %ld B of comments ",
      st.file_size, st.nlines, st.comments_size);
    if (st.file_size != 0)
      fprintf(stdout, "(%.0lf%%), ",
        100.0 * st.comments_size / st.file_size);
    if (st.nlines != 0)
      fprintf(stdout, "avg. line %.2lf B.\n",
        (double)st.file_size / st.nlines);

    if (c_files.nFiles > 0)
      fprintf(stdout, "*.c: %.2lf average lines, %d maximum lines.\n",
        (double)c_files.nLines / c_files.nFiles, c_files.maxLines);

    if (cpp_files.nFiles > 0)
      fprintf(stdout, "*.cpp: %.2lf average lines, %d maximum lines.\n",
        (double)cpp_files.nLines / cpp_files.nFiles, cpp_files.maxLines);

    if (h_files.nFiles > 0)
      fprintf(stdout, "*.h: %.2lf average lines, %d maximum lines.\n",
        (double)h_files.nLines / h_files.nFiles, h_files.maxLines);
  }

  fprintf(stdout, "Successfully done.\n");
  //getchar();
  
  return 0;
} /* End of 'main' function */

/* END OF 'main.c' FILE */
