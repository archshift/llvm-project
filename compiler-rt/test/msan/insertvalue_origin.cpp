// RUN: %clangxx_msan -fsanitize-memory-track-origins -O0 %s -o %t && not %run %t >%t.out 2>&1
// RUN: FileCheck %s < %t.out && FileCheck %s < %t.out
// RUN: %clangxx_msan -fsanitize-memory-track-origins -O3 %s -o %t && not %run %t >%t.out 2>&1
// RUN: FileCheck %s < %t.out && FileCheck %s < %t.out

// Test origin propagation through insertvalue IR instruction.

#include <stdio.h>
#include <stdint.h>

struct mypair {
 int64_t x;
 int y;
};

// Suppress eager checking
union Int64 {
  int64_t val;
  char __partial_init;
};

mypair my_make_pair(Int64 x, int y)  {
 mypair p;
 p.x = x.val;
 p.y = y;
 return p;
}

int main() {
 Int64 * volatile p = new Int64;
 mypair z = my_make_pair(*p, 0);
 if (z.x)
   printf("zzz\n");
 // CHECK: MemorySanitizer: use-of-uninitialized-value
 // CHECK: {{in main .*insertvalue_origin.cpp:}}[[@LINE-3]]

 // CHECK: Uninitialized value was created by a heap allocation
 // CHECK: {{in main .*insertvalue_origin.cpp:}}[[@LINE-8]]
 delete p;
 return 0;
}
