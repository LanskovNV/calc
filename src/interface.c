/* NL_5, 28.02.2017 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "error.h"
#include "interface.h"
#include "calculate.h"

#define LEN_SIZE 10

static void s_StreamSkip(FILE *stream)
{
  int c;

  do
  {
    c = fgetc(stream);
  } while (c != EOF && c != '\n');
}  /* end of func */

static BOOL s_IsEndOfLine(uchar c)
{
  if (c == '\n')
	return TRUE;
  else
	return FALSE;
}/* end of func */

static BOOL s_IsStreamEnd(FILE *stream)
{
  if (feof(stream) || ferror(stream))
    return TRUE;
  else
    return FALSE;
}
/* problem with memory */
uchar *ReadLine(FILE *stream, BOOL *goNext, BOOL *flag) 
{
  int c = 0, cnt = 0, maxLen = 20;
  uchar *string, *re;
  
  /* stream check */
  if (feof(stream) || ferror(stream))
  {
    *goNext = FALSE;
    return NULL;
  }

  /* memory check */
  if ((string = malloc(maxLen)) == NULL)
  {
    s_StreamSkip(stream);
    *goNext = !s_IsStreamEnd(stream);
    return NULL;
  }

  /* read line */
  while ((c = fgetc(stream)) != '\n' && c != EOF)
  { 
    if (cnt + 3 == maxLen)
      if ((re = realloc(string, maxLen + LEN_SIZE)) != NULL)
      {
        string = re;
        maxLen += LEN_SIZE;
      }
	  else
	  {
        s_StreamSkip(stream);
        *goNext = !s_IsStreamEnd(stream);
        return NULL;
	  } 
    string[cnt++] = (uchar)c;
  }
  
  /* EOF check */
  if (c == EOF && cnt == 0)
  {
    free(string);
    *goNext = FALSE;
    return NULL;
  }

  if (c == EOF)
    *flag = TRUE;

  string[cnt++] = '\0';

  if ((re = realloc(string, cnt)) != NULL)
	string = re;
  else
  {
    *goNext = !s_IsStreamEnd(stream); 
	return NULL;
  }

  return string;
}/* End of ReadLine func */

int NeedCalculate(uchar *line) 
{
  int isSpace = 1, isComment = 0, i = 0;

  while (line[i] != '\0')
  {
    if (isSpace || i == 0)
      if (line[i] == '/' && line[i + 1] == '/')
        isComment = 1;

    if (!isspace((int)line[i]))
      isSpace = 0;
    i++;
  }
  if (isSpace != 0 || isComment != 0 || line[0] == '\0')
    return 0;
  else
    return 1;
}/* End of NeedCalculate func */

void ProcessLine(uchar* line, BOOL flag) 
{
  error_t lastError = ERR_OK;

  if (!NeedCalculate(line))
  {
    puts((const char*)line); 
    return;
  }
  else
  {
    double result = Calculate(line, &lastError);
    
    printf("%s == ", line);
    /* error check */
    if (lastError == ERR_OK)
    {
      printf("%g", result);
      if (!flag)
        printf("\n");
    }
    else
    {
      ReportError(lastError);
      if (!flag)
        printf("\n");
    }
  }
}/* End of ProcessLine func */


