/* NL_5, 03.03.2017 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

error_t ReportError(error_t error)
{
  char* string;
  
  if ((string = GetErrorString(error)) == NULL)
    printf("ERROR: no memory for report");
  else
  {
    printf("ERROR: %s", string);
    free(string);
  }

  return error;
}/* End of report error func */

char* GetErrorString(error_t error)
{
  char* errStr;

  if (errStr == NULL)
    return NULL;

  switch (error)
  {
  case ERR_INCORRECT_OPERATION:
    errStr = malloc(strlen("can not solve this operation") + 1);
    strcpy(errStr, "can not solve this operation");
    break;
  case ERR_LOST_BRACKET:
      errStr = strlen("no (...") + 1;
    strcpy(errStr, "no (...");
    break;
  case ERR_NOT_ENOUGH_MEMORY:
      errStr = strlen("memory problem") + 1;
    strcpy(errStr, "memory problem");
    break;
  case ERR_PARCER_CRASHED:
      errStr = strlen("problem in parcer func") + 1;
    strcpy(errStr, "problem in parcer func");
    break;
  case ERR_PREFIX_WAS_WAITED:
      errStr = strlen("bad suffix") + 1;
    strcpy(errStr, "bad suffix");
    break;
  case ERR_SUFFIX_WAS_WAITED:
      errStr = strlen("bad prefix") + 1;
    strcpy(errStr, "bad prefix");
    break;
  case ERR_UNKNOWN_TOKEN:
      errStr = strlen("unknown token") + 1;
    strcpy(errStr, "unknown token");
    break;
  default:
      errStr = strlen("error is not declarated") + 1;
	strcpy(errStr, "error is not declarated");
    break;
  }

  return errStr;
}/* End of GetErrorString func */