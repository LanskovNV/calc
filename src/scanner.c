/* NL_5, 10.10.2015 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "error.h"

#include "calculate.h"
#include "list.h"
#include "scanner.h"

#define IsDigit(x) ((x >= '0') && (x <= '9'))

#define PI 3.14159265358979323846
#define E 2.71828182845904523536

#define EXP_STRING "e"
#define PI_STRING "pi"

static void s_TokenInit(TOKEN *t)
{
  t->Num = 0;
  t->Id = TOK_NUM;
}

BOOL s_ReadNumber(uchar **S, TOKEN *t) 
{
  double denum = 10;
  int constFlag = 0;

  t->Id = TOK_NUM;
  t->Num = 0;

  /* skip spaces after ',' in logax */
  while (isspace((int)(**S)))
	(*S)++;

  while (IsDigit(**S))
  {
	t->Num = t->Num * denum + (**S - '0');
	(*S)++;
	constFlag = 1;
  }
  if (!strncmp((const char*)(*S), PI_STRING, strlen(PI_STRING)))
  {
	if (!constFlag)
	{
	  t->Num = PI;
	  (*S) += strlen(PI_STRING);
	}
  }
  else if (**S == 'p')
	return FALSE;
  if (!strncmp((const char*)(*S), EXP_STRING, strlen(EXP_STRING)))
  {
	if (!constFlag)
	{
	  t->Num = E;
	  (*S) += strlen(EXP_STRING);
	}
  }
  if (t->Num != E && t->Num != PI)
  {
	if (**S == '.')
	{
	  (*S)++;
	  if (**S == '.')
		return FALSE;
	  while (IsDigit(**S))
	  {
		t->Num += (**S - '0') / denum;
		denum *= 10;
		(*S)++;
	  }
	  denum = 10;
	}
	if (**S == 'e' || **S == 'E')
	{
	  double minus = 1;
	  double tenPow = 0;
	  int flag = 0;

	  (*S)++;
	  if (**S == '-')
	  {
		minus = -1;
		(*S)++;
	  }
	  else if (**S == '+')
		(*S)++;
	  while (IsDigit(**S))
	  {
		tenPow = tenPow * denum + (**S - '0');
		(*S)++;
		flag = 1;
	  }
	  if (flag == 0)
		return FALSE;
	  else
		t->Num *= pow(denum, minus * tenPow);
	}
	if (**S == 'l' || **S == 'L')
	{
	  (*S)++;
	  if (!isspace(**S) && **S != '\0')
		return FALSE;
	  else
	  {
		long double tmp = (long double)t->Num;
		t->Num = (double)tmp;
	  }
	}
	if (**S == 'F' || **S == 'f')
	{
	  (*S)++;
	  if (!isspace(**S) && **S != '\0')
		return FALSE;
	  else
		t->Num = (float)(t->Num);
	}
  }
  return TRUE;
} /* end of function */

int Scanner(QUEUE *Q, uchar *S, error_t* error) 
{
  char *FuncNames[] = { "sin", "cos", "tg", "arcsin",
    "arccos", "arctg", "ctg", "ln", "sqrt", "ceil", "floor"};
  double(*Funcs[])(double) = { sin , cos , tan, asin, acos, atan, ctg, log, sqrt, myCeil, myFloor};
  TOKEN t;
  BOOL isLogax = FALSE, isLogaxValue = FALSE;

  s_TokenInit(&t);

  while (*S != 0)
  {
    int i, isCorrectToken = 0; 

	if (isLogax)
	{
      while (isspace(*S))
        S++;
	  if (!s_ReadNumber(&S, &t))
	  {
		*error = ERR_UNKNOWN_TOKEN;
		ClearList(&Q->Head);
		return 0;
	  }
	  isLogax = FALSE;
	  isLogaxValue = TRUE;
	}
	else
	{
	  switch (*S)
	  {
	  case ',':
		if (isLogaxValue)
		{
		  t.Id = TOK_OP;
		  t.Op = *S;
		  S++;
		  isLogaxValue = FALSE;
		}
		else
		{
		  *error = ERR_UNKNOWN_TOKEN;
		  ClearList(&Q->Head);
		  return 0;
		}
		break;
	  case ' ':
	  case '\n':
	  case '\t':
	  case '\r':
		S++;
		continue;
	  case '+':
	  case '-':
	  case '*':
	  case '/':
	  case '%':
	  case '^':
	  case '(':
	  case ')':
	  case '@':
		t.Id = TOK_OP;
		t.Op = *S;
		S++;
		break;
	  case '.':
	  case '0':
	  case '1':
	  case '2':
	  case '3':
	  case '4':
	  case '5':
	  case '6':
	  case '7':
	  case '8':
	  case '9':
	  case 'p':
	  case 'e':
		if (!s_ReadNumber(&S, &t))
		{
		  *error = ERR_UNKNOWN_TOKEN;
		  ClearList(&Q->Head);
		  return 0;
		}
		break;
	  default:
		for (i = 0; i < (sizeof(FuncNames) / sizeof(FuncNames[0])); i++)
		  if (!strncmp((const char*)S, FuncNames[i], strlen(FuncNames[i])))
		  {
			t.Id = TOK_FUNC;
			t.Func = Funcs[i];
			t.Op = '?';
			t.funcNumber = i;
			S += strlen(FuncNames[i]);
			isCorrectToken = 1;
			break;
		  }

		/* code for logax */
		if (!strncmp((const char*)S, "log", strlen("log")))
		{
		  isLogax = TRUE;
		  t.Id = TOK_LOGAX;
          t.Op = '?';
		  t.Logax = logax;
		  S += strlen("log");

		  if (!Put(Q, t))
		  {
			*error = ERR_NOT_ENOUGH_MEMORY;
			ClearList(&Q->Head);
			return 0;
		  }
		  while (isspace(*S))
			S++;
		  if (*S == '(')
		  {
			t.Id = TOK_OP;
			t.Op = *S;
			S++;
		  }
		  else
		  {
			*error = ERR_UNKNOWN_TOKEN;
			ClearList(&Q->Head);
			return 0;
		  }
		  isCorrectToken = 1;
		}
		if (isCorrectToken == 0)
		{
		  *error = ERR_UNKNOWN_TOKEN;
		  ClearList(&Q->Head);
		  return 0;
		}
	  }
	}
    if (!Put(Q, t))
    {
      *error = ERR_NOT_ENOUGH_MEMORY;
	  ClearList(&Q->Head);
      return 0;
    }
  }

  return 1;
}/* end of Scanner */