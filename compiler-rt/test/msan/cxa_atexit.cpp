// RUN: %clangxx_msan -O0 %s -o %t && %run %t %p

// PR17377: C++ module destructors get stale argument shadow.

#include <stdio.h>
#include <stdlib.h>
class A {
public:
  // This destructor get stale argument shadow left from the call to f().
  ~A() {
    if (this)
      exit(0);
  }
};

A a;

// Suppress eager checking
union Long {
  long val;
  char __partial_init;
};

__attribute__((noinline))
void f(Long x) {
}

int main(void) {
  long  x;
  Long * volatile p = (Long *)&x;
  // This call poisons TLS shadow for the first function argument.
  f(*p);
  return 0;
}
