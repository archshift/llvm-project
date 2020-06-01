// RUN: %clangxx_msan -fsanitize-memory-track-origins=2 -O3 %s -o %t && \
// RUN:     not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-%short-stack --check-prefix=CHECK-STACK < %t.out

// RUN: %clangxx_msan -fsanitize-memory-track-origins=2 -DHEAP=1 -O3 %s -o %t && \
// RUN:     not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-%short-stack --check-prefix=CHECK-HEAP < %t.out


// RUN: %clangxx_msan -mllvm -msan-instrumentation-with-call-threshold=0 -fsanitize-memory-track-origins=2 -O3 %s -o %t && \
// RUN:     not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-%short-stack --check-prefix=CHECK-STACK < %t.out

// RUN: %clangxx_msan -mllvm -msan-instrumentation-with-call-threshold=0 -fsanitize-memory-track-origins=2 -DHEAP=1 -O3 %s -o %t && \
// RUN:     not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-HEAP < %t.out

#include <stdio.h>

// Suppress eager checking
union Int {
  int val;
  char __partial_init;
};

volatile Int x, y;

__attribute__((noinline))
void fn_g(Int a) {
  x.val = a.val;
}

__attribute__((noinline))
void fn_f(Int a) {
  fn_g(a);
}

__attribute__((noinline))
void fn_h() {
  y.val = x.val;
}

int main(int argc, char *argv[]) {
#ifdef HEAP
  Int * volatile zz = new Int;
  Int z = *zz;
#else
  Int volatile z;
#endif
  fn_f({z.val});
  fn_h();
  return y.val;
}

// CHECK: WARNING: MemorySanitizer: use-of-uninitialized-value
// CHECK: {{#0 .* in main.*chained_origin.cpp:}}[[@LINE-4]]

// CHECK: Uninitialized value was stored to memory at
// CHECK-FULL-STACK: {{#0 .* in fn_h.*chained_origin.cpp:}}[[@LINE-19]]
// CHECK-FULL-STACK: {{#1 .* in main.*chained_origin.cpp:}}[[@LINE-9]]
// CHECK-SHORT-STACK: {{#0 .* in fn_h.*chained_origin.cpp:}}[[@LINE-21]]

// CHECK: Uninitialized value was stored to memory at
// CHECK-FULL-STACK: {{#0 .* in fn_g.*chained_origin.cpp:}}[[@LINE-34]]
// CHECK-FULL-STACK: {{#1 .* in fn_f.*chained_origin.cpp:}}[[@LINE-30]]
// CHECK-FULL-STACK: {{#2 .* in main.*chained_origin.cpp:}}[[@LINE-16]]
// CHECK-SHORT-STACK: {{#0 .* in fn_g.*chained_origin.cpp:}}[[@LINE-37]]

// CHECK-STACK: Uninitialized value was created by an allocation of 'z.sroa.0' in the stack frame of function 'main'
// CHECK-STACK: {{#0 .* in main.*chained_origin.cpp:}}[[@LINE-27]]

// CHECK-HEAP: Uninitialized value was created by a heap allocation
// CHECK-HEAP: {{#1 .* in main.*chained_origin.cpp:}}[[@LINE-28]]
