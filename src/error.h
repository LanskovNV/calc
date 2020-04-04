#pragma once
#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

/*#define _NDEBUG
#ifdef _DEBUG
#define CL_CRT_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
# endif*/

/* types */
typedef enum
{
  ERR_OK,
  ERR_NOT_ENOUGH_MEMORY,
  ERR_INCORRECT_OPERATION,
  ERR_UNKNOWN_TOKEN,
  ERR_SUFFIX_WAS_WAITED,
  ERR_PREFIX_WAS_WAITED, 
  ERR_LOST_BRACKET, 
  ERR_PARCER_CRASHED
} error_t;

/* functions */
char* GetErrorString(error_t error);
error_t ReportError(error_t error);

#endif // ERROR_H_INCLUDED 
