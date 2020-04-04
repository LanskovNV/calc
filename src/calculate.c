/* NL_5, 03.03.2017 */
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "error.h"
#include "calculate.h"
#include "scanner.h"
#include "parser.h"
#include "list.h"

double myCeil(double x)
{
  return ceil(x);
}
double myFloor(double x)
{
  return floor(x);
}
double logax(double a, double x)
{
  return log(x) / log(a);
} /* end of func */ 
double ctg(double x)
{
  return (double)(1 / tan(x));
}/* end of ctg func */

double Calculate(uchar* expression, error_t* error)
{
  double result = 0;

  QUEUE queue = { NULL, NULL };
  QUEUE queRes = { NULL, NULL };
  BOOL isIncorrectOp = FALSE; // CRUTCH!!!!! TODO: fix this option

  if (Scanner(&queue, expression, error))
	if (Parcer(&queue, &queRes, error))
	{
	  STACK S = { NULL };
	  TOKEN t;

	  while (Get(&queRes, &t))
	  {
		if (isIncorrectOp) // cruth for memory leaks
		  break;

		/* options */
        if (t.Id == TOK_NUM)
        {
          if (!Push(&S, t))
          {
            ClearList(&S.Top);
            *error = ERR_NOT_ENOUGH_MEMORY;
            ClearList(&(queRes.Head));
            ClearList(&(queue.Head));
            return -1;
          }
        }
        else if (t.Id == TOK_OP && t.Op == '@')
        {
          Pop(&S, &t);
          t.Num = -t.Num;
          if (!Push(&S, t))
          {
            ClearList(&S.Top);
            *error = ERR_NOT_ENOUGH_MEMORY;
            ClearList(&(queRes.Head));
            ClearList(&(queue.Head));
            return -1;
          }
        }
        else if (t.Id == TOK_OP && t.Op == ',')
          continue;
		else if (t.Id == TOK_FUNC)
		{
		  TOKEN FuncOp;

		  Pop(&S, &FuncOp);
		  FuncOp.Num = t.Func(FuncOp.Num);
		  if (!Push(&S, FuncOp))
          {
            ClearList(&S.Top);
            *error = ERR_NOT_ENOUGH_MEMORY;
            ClearList(&(queRes.Head));
            ClearList(&(queue.Head));
            return -1;
          }
		}
		else if (t.Id == TOK_LOGAX)
		{
		  TOKEN res, base, oper;

		  Pop(&S, &oper);
		  Pop(&S, &base);
		  res.Id = TOK_NUM;
		  res.Num = t.Logax(base.Num, oper.Num);
		  if (!Push(&S, res))
          {
            ClearList(&S.Top);
            *error = ERR_NOT_ENOUGH_MEMORY;
            ClearList(&(queRes.Head));
            ClearList(&(queue.Head));
            return -1;
          }
		}
		else
		{
		  TOKEN Right, Left, Res;

		  Pop(&S, &Right);
		  Pop(&S, &Left);
		  Res.Id = TOK_NUM;
		  switch (t.Op)
		  {
		  case '+':
			Res.Num = Right.Num + Left.Num;
			if (!Push(&S, Res))
            {
              ClearList(&S.Top);
              *error = ERR_NOT_ENOUGH_MEMORY;
              ClearList(&(queRes.Head));
              ClearList(&(queue.Head));
              return -1;
            }
			break;
		  case '-':
			Res.Num = Left.Num - Right.Num;
            if (!Push(&S, Res))
            {
              ClearList(&S.Top);
              *error = ERR_NOT_ENOUGH_MEMORY;
              ClearList(&(queRes.Head));
              ClearList(&(queue.Head));
              return -1;
            }
			break;
		  case '*':
			Res.Num = Right.Num * Left.Num;
            if (!Push(&S, Res))
            {
              ClearList(&S.Top);
              *error = ERR_NOT_ENOUGH_MEMORY;
              ClearList(&(queRes.Head));
              ClearList(&(queue.Head));
              return -1;
            }
			break;
		  case '/':
			if (Right.Num == 0)
			  isIncorrectOp = TRUE;
			else
			{
			  Res.Num = Left.Num / Right.Num;
              if (!Push(&S, Res))
              {
                ClearList(&S.Top);
                *error = ERR_NOT_ENOUGH_MEMORY;
                ClearList(&(queRes.Head));
                ClearList(&(queue.Head));
                return -1;
              }
			}
			break;
		  case '^':
			if (Left.Num == 0 && Right.Num < 0)
			  isIncorrectOp = TRUE;
			else
			{
			  Res.Num = pow(Left.Num, Right.Num);
              if (!Push(&S, Res))
              {
                ClearList(&S.Top);
                *error = ERR_NOT_ENOUGH_MEMORY;
                ClearList(&(queRes.Head));
                ClearList(&(queue.Head));
                return -1;
              }
			}
			break;
		  case '%':
			if (Right.Num == 0)
			  isIncorrectOp = TRUE;
			else
			{
			  Res.Num = Left.Num / Right.Num * 100.0;
              if (!Push(&S, Res))
              {
                ClearList(&S.Top);
                *error = ERR_NOT_ENOUGH_MEMORY;
                ClearList(&(queRes.Head));
                ClearList(&(queue.Head));
                return -1;
              }
			}
			break;
		  default:
			*error = ERR_UNKNOWN_TOKEN;
			isIncorrectOp = TRUE; // it is bad way... one return in function is ok
		  }
		}
	  } 

	  if (S.Top == NULL || S.Top->Next != NULL)
	  {
		isIncorrectOp = TRUE; // it's another misstake
		*error = ERR_NOT_ENOUGH_MEMORY;
		result = -1; // it is bad way... one return in function is ok
	  }
	  else
	  {
		result = S.Top->t.Num;
		Pop(&S, &t);
	  }
	  ClearList(&S.Top); // ?
	}
	else
	  isIncorrectOp = TRUE;
  else
	isIncorrectOp = TRUE;

  /* if not null - error */
  if (queRes.Head != NULL || queue.Head != NULL)
	*error = ERR_NOT_ENOUGH_MEMORY;
  ClearList(&(queRes.Head));
  ClearList(&(queue.Head));

  /* checking answer is digital */
  if (isnan(result) || isinf(result) || isIncorrectOp) // different errors ?!
	*error = ERR_INCORRECT_OPERATION;
  else 
    *error = ERR_OK;

  return result;
}/* End of Calculate func */
 