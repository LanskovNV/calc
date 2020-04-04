#pragma once
#ifndef CALCULATE_H_INCLUDED
#define CALCULATE_H_INCLUDED

#include "error.h"

/* types */
typedef unsigned char uchar;

typedef enum {
  FALSE,
  TRUE
}BOOL;

typedef enum {
  TOK_NUM,
  TOK_OP,
  TOK_FUNC,
  TOK_LOGAX,
}TOKEN_ID;

typedef struct tagTOKEN {
  char Op;
  double Num;
  double(*Func)(double);
  double(*Logax)(double, double);
  int funcNumber;
  TOKEN_ID Id;
}TOKEN;

/* functions */
double myCeil(double x);
double myFloor(double x);
double logax(double a, double x);
double ctg(double x);
double Calculate(uchar *expression, error_t* error);

#endif // CALCULATE_H_INCLUDED 