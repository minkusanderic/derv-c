#include "derv.h"

Regex empty()
{
  Regex ret = (Regex)malloc(sizeof(union expr));
  ret->tag = EMPTY;
  return ret;
}

Regex epsilon()
{
  Regex ret = (Regex)malloc(sizeof(union expr));
  ret->tag = EPSILON;
  return ret;

}

Regex and(Regex this, Regex that)
{
  Regex ret = (Regex)malloc(sizeof(union expr));
  ret->tag = INTERSECTION;
  ret->intersection.this = this;
  ret->intersection.that = that;
  return ret;
}

Regex or(Regex this, Regex that)
{
  Regex ret = (Regex)malloc(sizeof(union expr));
  ret->tag = CHOICE;
  ret->choice.this = this;
  ret->choice.that = that;
  return ret;
}


Regex seq(Regex this, Regex rest)
{
  Regex ret = (Regex)malloc(sizeof(union expr));
  ret->tag = SEQUENCE;
  ret->sequence.this = this;
  ret->sequence.rest = rest;
  return ret;
}

Regex prim(char a)
{
  Regex ret = (Regex)malloc(sizeof(union expr));
  ret->tag = PRIM;
  ret->prim.a = a;
  return ret;
}

Regex word(const char * str)
{
  int length = strlen(str);

  if(length == 1)
    {
      return prim(str[0]);
    }
  else
    {
      return seq(prim(str[0]), word(str + 1));
    }
}

void expr_to_string(Regex e, char * ret)
{
  char this_str[1000];
  char that_str[1000];

  switch(e->tag)
    {
    case PRIM:
      sprintf(ret, "prim(%c)", e->prim.a);
      break;
    case INTERSECTION:
      expr_to_string(e->intersection.this, this_str);
      expr_to_string(e->intersection.that, that_str);
      sprintf(ret, "and(%s, %s)", this_str, that_str);
      break;
   case CHOICE:
      expr_to_string(e->intersection.this, this_str);
      expr_to_string(e->intersection.that, that_str);
      sprintf(ret, "or(%s, %s)", this_str, that_str);
      break;
    case SEQUENCE:
      expr_to_string(e->sequence.this, this_str);
      expr_to_string(e->sequence.rest, that_str);
      sprintf(ret, "seq(%s, %s)", this_str, that_str);
      break;
    default:
      ret = "[ERROR]";
      break;
    }
}

bool isNullable(Regex e)
{
  switch(e->tag)
    {
    case EMPTY:
      return false;
    case EPSILON:
      return true;
    case PRIM:
      return false;
    case SEQUENCE:
      return isNullable(e->intersection.this) && isNullable(e->intersection.that);
    case INTERSECTION:
      return isNullable(e->intersection.this) && isNullable(e->intersection.that);
    case CHOICE:
      return isNullable(e->intersection.this) || isNullable(e->intersection.that);
    default:
      break;
    }
}

Regex derive(Regex e, char c)
{
  switch(e->tag)
    {
    case EMPTY:
      return empty();
    case EPSILON:
      return empty();
    case PRIM:
      if(e->prim.a == c) { return epsilon(); }
      else { return empty(); }
    case INTERSECTION:
      return and(derive(e->intersection.this, c),
		 derive(e->intersection.that, c));
    case CHOICE:
      return or(derive(e->intersection.this, c),
		derive(e->intersection.that, c));
    case SEQUENCE:
      if(isNullable(e->sequence.this))
	{
	  return or(seq(derive(e->sequence.this, c), e->sequence.rest), derive(e->sequence.rest, c));
	}
      else
	{
	  return seq(derive(e->sequence.this, c), e->sequence.rest);
	}
    default:
      break;
    }
}


bool match(Regex e, const char* str)
{
  while(strlen(str) > 0)
    {
      e = derive(e, str[0]);
      str++;
    }
  return isNullable(e);
}
