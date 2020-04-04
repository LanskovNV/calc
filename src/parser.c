/* NL_5, 10.10.2015 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "parser.h"
#include "calculate.h"
#include "list.h"
#include "error.h"

static int Assocyativnost(char c)
{
  if (c == '^')
    return 1;
  return 0;
}

static int GetPrior(char c)
{
  switch (c)
  {
  case '(':
	return 1;
  case ')':
    return 2;
  case ',':
    return 3;
  case '+':
  case '-':
    return 4;
  case '*':
  case '/':
  case '%':
    return 5;
  case '@':
    return 6;
  case '^':
    return 7;
  case '?':
    return 8;
  default:
    return -1;
  }
}
static void DropOpers(STACK *S1, STACK *S2, int Prior, int As, error_t *error)
{
  TOKEN t;

  while (S2->Top != NULL && ((GetPrior(S2->Top->t.Op) >= Prior && As == 0) ||
	                         (GetPrior(S2->Top->t.Op) > Prior && As == 1)))
  {
    Pop(S2, &t);
    if (!Push(S1, t))
      *error = ERR_NOT_ENOUGH_MEMORY;
  }
}

static STATE_t s_PutToStack(TOKEN *T, STACK *S, error_t *error, STATE_t ok)
{
  if (Push(S, *T))
	return ok;
  else
  {
	*error = ERR_NOT_ENOUGH_MEMORY;
	return ERROR;
  }
}

int Parcer(QUEUE *Q1, QUEUE *Q2, error_t* error)
{
  STACK S1 = { NULL }, S2 = { NULL };
  TOKEN T;
  int result = 1;
  STATE_t state = WAIT_PREFIX;


  /* init T */
  T.Func = NULL;
  T.Id = TOK_FUNC;
  T.Num = 0;
  T.Op = 0;

  while (state != END)
  {
    if (state < ERROR)
    {
      if (!Get(Q1, &T))
      {
        if (state == WAIT_SUFFIX)
          state = DONE;
        else
        {
          *error = ERR_SUFFIX_WAS_WAITED;
          state = ERROR;
        }
      }
    }
    switch (state)
    {
	case WAIT_SCOBE:
	  if (T.Id != TOK_OP || T.Op != '(')
	  {
		*error = ERR_LOST_BRACKET;
		state = ERROR;
	  }
	  else
	  {
		state = s_PutToStack(&T, &S2, error, WAIT_PREFIX);
	  }
	  break;
    case WAIT_PREFIX:
	  if (T.Id == TOK_FUNC)
		state = s_PutToStack(&T, &S2, error, WAIT_SCOBE);
	  else if (T.Id == TOK_LOGAX)
		state = s_PutToStack(&T, &S2, error, WAIT_SCOBE);
      else if (T.Id == TOK_NUM)
		state = s_PutToStack(&T, &S1, error, WAIT_SUFFIX);
      else if (T.Id == TOK_OP && T.Op == '(')
		state = s_PutToStack(&T, &S2, error, WAIT_PREFIX);
      else if ((T.Id == TOK_OP) && T.Op == '-')
      {
        T.Op = '@';
		state = s_PutToStack(&T, &S2, error, WAIT_PREFIX);
      }
      else
      {
        *error = ERR_PREFIX_WAS_WAITED;
        state = ERROR;
      }
      break;
    case WAIT_SUFFIX:
      if ((T.Id == TOK_OP) && (T.Op == ')'))
      {
        DropOpers(&S1, &S2, GetPrior(T.Op), 0, error);
        if (*error != ERR_OK)
        {
          ClearList(&S1.Top);
          ClearList(&S2.Top);
          return 0;
        }
        Pop(&S2, &T);
	    if ((T.Op == '(') && (T.Id == TOK_OP))
		  state = WAIT_SUFFIX;
		else
		{
		  *error = ERR_LOST_BRACKET;
		  state = ERROR;
		}
      }
      else if ((T.Id == TOK_OP) && (T.Op != ')'))
      {
        DropOpers(&S1, &S2, GetPrior(T.Op), Assocyativnost(T.Op), error);
        if (*error != ERR_OK)
        {
          ClearList(&S1.Top);
          ClearList(&S2.Top);
          return 0;
        }
		state = s_PutToStack(&T, &S2, error, WAIT_PREFIX);
      }
      else
      {
        *error = ERR_SUFFIX_WAS_WAITED;
        state = ERROR;
      }
      break;
    case ERROR:
      ClearList(&S1.Top);
      ClearList(&S2.Top);
      result = 0;
      state = END;
      break;
    case DONE:
      DropOpers(&S1, &S2, '(', 0, error);
      if (*error != ERR_OK)
      {
        ClearList(&S1.Top);
        ClearList(&S2.Top);
        return 0;
      }
      if (S1.Top != NULL) // here was S2
      {
        while (Pop(&S1, &T))
          if (!Push(&S2, T))
          {
            ClearList(&S1.Top);
            ClearList(&S2.Top);
            return 0;
          }
        while (Pop(&S2, &T))
        {
          if (!Put(Q2, T))
            {
              ClearList(&Q2->Head);
              ClearList(&S1.Top);
              ClearList(&S2.Top);
              return 0;
            }
        }
        state = END;
      }
      else
      {
        *error = ERR_PARCER_CRASHED;
        state = ERROR;
      }
      break;
    }
  }
  ClearList(&S1.Top);
  ClearList(&S1.Top);

  return result;
}