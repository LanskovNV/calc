#pragma once
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "error.h"
#include "calculate.h"

/* functions */
uchar *ReadLine(FILE *stream, BOOL *goNext, BOOL *flag);
void ProcessLine(uchar* line, BOOL flag);
int NeedCalculate(uchar *line);

#endif // INTERFACE_H_INCLUDED 
