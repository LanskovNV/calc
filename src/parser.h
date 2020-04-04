#pragma once
#ifndef PARCER_H_INCLUDED
#define PARCER_H_INCLUDED

#include "list.h"
#include "error.h"
/* types */
typedef enum
{
  WAIT_PREFIX,
  WAIT_SUFFIX,
  WAIT_SCOBE,
  ERROR,
  DONE,
  END
}STATE_t;

/* functions */
int Parcer(QUEUE *Q1, QUEUE *Q2, error_t* error);

#endif // PARCER_H_INCLUDED 