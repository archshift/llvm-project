// RUN: %clangxx_msan -O0 %s -o %t && %run %t >%t.out 2>&1
// RUN: %clangxx_msan -O1 %s -o %t && %run %t >%t.out 2>&1
// RUN: %clangxx_msan -O2 %s -o %t && %run %t >%t.out 2>&1
// RUN: %clangxx_msan -O3 %s -o %t && %run %t >%t.out 2>&1

// Test that (no_sanitize_memory) functions DO NOT propagate shadow.

#include <stdlib.h>
#include <stdio.h>

// Suppress eager checking
union Int {
  int val;
  char __partial_init;
};

__attribute__((noinline))
__attribute__((weak))
__attribute__((no_sanitize_memory))
Int f(Int x) {
  return x;
}

int main(void) {
  Int x;
  Int * volatile p = &x;
  int y = f(*p).val;
  if (y)
    exit(0);
  return 0;
}
