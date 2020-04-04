#pragma once
#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "list.h"
#include "error.h"

typedef double(*newT)(double) ;

/* functions */
int Scanner(QUEUE *Q, uchar *S, error_t* error);

#endif // SCANNER_H_INCLUDED 