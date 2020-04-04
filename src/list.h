#pragma once
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "error.h"
#include "calculate.h"

/* types */
typedef struct tagLIST {
  TOKEN t;
  struct tagLIST *Next;
}LIST;

typedef struct {
  LIST *Top;
}STACK;

typedef struct {
  LIST *Head;
  LIST *Tail;
}QUEUE;

/* functions */
void ClearList(LIST **L);
int Push(STACK *S, TOKEN T);
int Pop(STACK *S, TOKEN *T);
int Put(QUEUE *Q, TOKEN T);
int Get(QUEUE *Q, TOKEN *T);

#endif /* LIST_H_INCLUDED */
