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

  Regex a = prim('a');
  Regex b = prim('b');

  ASSERT_MATCH( epsilon(), "")
  
  ASSERT_MATCH( prim('a'), "a");
  ASSERT_NOT_MATCH( prim('a'), "ab");
  ASSERT_NOT_MATCH( prim('a'), "");
  ASSERT_NOT_MATCH( prim('a'), "aaaa");

  ASSERT_MATCH( or(prim('a'), prim('b')), "a");
  ASSERT_MATCH( or(prim('a'), prim('b')), "b");
  ASSERT_NOT_MATCH( or(prim('a'), prim('b')), "");

  ASSERT_NOT_MATCH( and(prim('a'), prim('b')), "a");
  ASSERT_NOT_MATCH( and(prim('a'), prim('b')), "b");
  ASSERT_NOT_MATCH( and(prim('a'), prim('b')), "");

  ASSERT_MATCH( and(or(a, b), or(a, b)), "a");
  ASSERT_MATCH( and(or(a, b), or(a, b)), "b");

  ASSERT_MATCH( seq(a, b), "ab");
  ASSERT_MATCH( seq(a, seq(a, b)), "aab");
  ASSERT_NOT_MATCH( seq(a, b), "aa");

  ASSERT_MATCH( word("Hello"), "Hello");
  ASSERT_MATCH( seq(word("Hello"), word("World")), "HelloWorld");

  printf("%d of %d tests passed\n", total_tests - tests_failed, total_tests);
  
}
