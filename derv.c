#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdbool.h>

typedef enum {EMPTY, EPSILON, PRIM, INTERSECTION, SEQUENCE} tag_id;

union expr {
  tag_id tag;

  struct {
    tag_id tag;
    char a;
  } prim;

  struct {
    tag_id tag;
    union expr *this;
    union expr *that;
  } intersection;
  
  struct {
    tag_id tag;
    union expr *this;
    union expr *rest;
  } sequence;
} expr;

union expr* empty()
{
  union expr *ret = (union expr*)malloc(sizeof(union expr));
  ret->tag = EMPTY;
  return ret;
}

union expr* epsilon()
{
  union expr *ret = (union expr*)malloc(sizeof(union expr));
  ret->tag = EPSILON;
  return ret;

}

union expr* and(union expr* this, union expr* that)
{
  union expr *ret = (union expr*)malloc(sizeof(union expr));
  ret->tag = INTERSECTION;
  ret->intersection.this = this;
  ret->intersection.that = that;
  return ret;
}


union expr* seq(union expr* this, union expr* rest)
{
  union expr *ret = (union expr*)malloc(sizeof(union expr));
  ret->tag = SEQUENCE;
  ret->sequence.this = this;
  ret->sequence.rest = rest;
  return ret;
}

union expr* prim(char a)
{
  union expr *ret = (union expr*)malloc(sizeof(union expr));
  ret->tag = PRIM;
  ret->prim.a = a;
  return ret;
}

union expr* word(const char * str)
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

void expr_to_string(union expr *e, char * ret)
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
      sprintf(ret, "inter(%s, %s)", this_str, that_str);
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

union expr* derive(union expr* e, char c)
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
    case SEQUENCE:
      return and(derive(e->sequence.this, c),
		 e->sequence.rest);
    default:
      break;
    }
}

bool isNullable(union expr* e)
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
    default:
      break;
    }
}

bool match(union expr* e, const char* str)
{
  while(strlen(str) > 0)
    {
      e = derive(e, str[0]);
      str++;
    }
  return isNullable(e);
}

int main(void)
{

  union expr *test = seq(word("Hello"),
			 word("World"));
  char result[1000];
  expr_to_string(test, result);
  printf("%s\n", result);
  printf("%d\n", match(test, "a"));
}
