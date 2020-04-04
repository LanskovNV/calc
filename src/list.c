/* leins, 26.03.2017 */
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "list.h"
#include "calculate.h"

void ClearList(LIST **L)
{
  LIST *tmp;

  while (*L != NULL)
  {
    tmp = *L;
    *L = (*L)->Next;
    free(tmp);
  }
}/* OK */

int Push(STACK *S, TOKEN T)
{
  LIST *NewL;

  if ((NewL = malloc(sizeof(LIST))) == NULL)
    return 0;
  NewL->t = T;
  NewL->Next = S->Top;
  S->Top = NewL;
  return 1;
}/* OK */
 
int Pop(STACK *S, TOKEN *T)
{
  LIST *tmp = NULL;

  if (S->Top == NULL)
	return 0;

  if (T != NULL)
  {
	*T = S->Top->t;
	tmp = S->Top;
  }
  else
  {
	return 0;
  }
  S->Top = S->Top->Next;

  free(tmp);
  return 1;
}/* OK */

int Put(QUEUE *Q, TOKEN T)
{
  LIST *NewL;

  if ((NewL = malloc(sizeof(LIST))) == NULL)
    return 0;
  NewL->t = T;
  NewL->Next = NULL;
  if (Q->Head == NULL)
    Q->Head = Q->Tail = NewL;
  else
  {
    Q->Tail->Next = NewL;
    Q->Tail = NewL;
  }
  return 1;
}/* OK */

int Get(QUEUE *Q, TOKEN *T)
{
  LIST* tmp = NULL;

  if (Q->Head == NULL)
    return 0;
  if (T != NULL)
  {
    *T = Q->Head->t;
    tmp = Q->Head;
  }
  else
  {
	return 0;
  }
  Q->Head = Q->Head->Next;

  free(tmp);
  return 1;
}/* OK */
