#ifndef DERV_H
#define DERV_H

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


typedef union expr* Regex;


Regex prim(char c);
Regex word(const char* str);
Regex seq(Regex a, Regex b);
Regex and(Regex a, Regex b);
Regex or(Regex a, Regex b);

bool match(Regex r, const char* str);

#endif
