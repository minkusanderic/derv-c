#include <stdio.h>

#include "derv.h"


#define ASSERT_MATCH(expr, str) \
  if( match(expr, str) != true ) { printf("[FAILED] '" #expr "' did not match " #str "\n"); \
    tests_failed++;} total_tests++;

#define ASSERT_NOT_MATCH(expr, str) \
  if( match(expr, str) != false ) { printf("[FAILED] '" #expr "' incorrectly matched " #str "\n"); \
    tests_failed++;} total_tests++;


int main(void)
{
  int tests_failed = 0;
  int total_tests = 0;
  ASSERT_MATCH( prim('a'), "a");
  ASSERT_NOT_MATCH( prim('a'), "ab");
  ASSERT_NOT_MATCH( prim('a'), "");
  ASSERT_NOT_MATCH( prim('a'), "aaaa");

  printf("%d of %d tests passed\n", total_tests - tests_failed, total_tests);
  
}
