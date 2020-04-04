/* NL_5, 27.02.2017 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"


int main(int argc, char const* argv[]) 
{
  FILE* in;
  uchar *line = NULL;
  BOOL isNext = FALSE, isEOF = FALSE;

  in = stdin;
  /* crgtdbg feature for memory leaks */
  /* _CrtMemState _ms;
  _CrtMemCheckpoint(&_ms);*/

  /* Choose an input source */
  if (argc > 1)
  {
    in = fopen(argv[1], "r");
    if (in == NULL)
    {
      printf("ERROR: Cannot open file '%s'", argv[1]);
      return -1;
    }
  }

  /* Process the data line by line */
  while ((line = ReadLine(in, &isNext, &isEOF)) != NULL || isNext)
  {
	if (line != NULL)
      ProcessLine(line, isEOF);
    else
    {
      ReportError(ERR_NOT_ENOUGH_MEMORY);
      if (!isEOF)
        printf("\n");
    }
    free(line);
    isNext = FALSE;
  }

  /* Clean up */
  free(line);
  if (in != stdin)
    fclose(in);

  /*crgtdbg feature for memory leaks */ 
  /* _CrtMemDumpAllObjectsSince(&_ms); */
  return 0;
}
