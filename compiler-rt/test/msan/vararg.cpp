// RUN: %clangxx_msan -fsanitize-memory-track-origins=0 -DEXPECT_PASS -O3 %s -o %t && \
// RUN:     not %run %t va_arg_tls >%t.out 2>&1
// RUN: not [ -s %t.out ]

// RUN: %clangxx_msan -fsanitize-memory-track-origins=0 -DEXPECT_PASS -O3 %s -o %t && \
// RUN:     not %run %t overflow >%t.out 2>&1
// RUN: not [ -s %t.out ]
 
// RUN: %clangxx_msan -fsanitize-memory-track-origins=0 -DEXPECT_FAIL -O3 %s -o %t && \
// RUN:     not %run %t va_arg_tls >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=CHECK < %t.out

// RUN: %clangxx_msan -fsanitize-memory-track-origins=0 -DEXPECT_FAIL -O3 %s -o %t && \
// RUN:     not %run %t overflow >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=CHECK < %t.out

// RUN: %clangxx_msan -fsanitize-memory-track-origins=2 -DEXPECT_FAIL -O3 %s -o %t && \
// RUN:     not %run %t va_arg_tls >%t.out 2>&1
// RUN: FileCheck %s --check-prefixes=CHECK,CHECK-ORIGIN < %t.out

// RUN: %clangxx_msan -fsanitize-memory-track-origins=2 -DEXPECT_FAIL -O3 %s -o %t && \
// RUN:     not %run %t overflow >%t.out 2>&1
// RUN: FileCheck %s --check-prefixes=CHECK,CHECK-ORIGIN < %t.out

// Check that shadow and origin are passed through va_args.

// Copying origins on AArch64, MIPS and PowerPC isn't supported yet.
// XFAIL: aarch64
// XFAIL: mips
// XFAIL: powerpc64

#include <stdarg.h>
#include <string.h>

#include <sanitizer/msan_interface.h>

__attribute__((noinline))
void dummy(long a, long b, long c, long d, long e) {
  __asm__ __volatile__ (""::"r"(a), "r"(b), "r"(c), "r"(d), "r"(e));
}

__attribute__((noinline))
void poison_stack_and_param() {
  char x[10000];
#ifndef TEST_MSAN_EAGER_CHECKS
  int y;
  dummy(y, y, y, y, y);
#endif
}

__attribute__((noinline))
int sum(int n, ...) {
  va_list args;
  int i, sum = 0, arg;
  volatile int temp;
  va_start(args, n);
  for (i = 0; i < n; i++) {
    arg = va_arg(args, int);
    sum += arg;
  }
  va_end(args);
  return sum;
}

int main(int argc, char *argv[]) {
#if defined(EXPECT_FAIL)
  volatile int uninit;
  #define PRE_RUN for (int i = 0; i < 6; i++) __msan_unpoison_param(i)
  // CHECK: WARNING: MemorySanitizer: use-of-uninitialized-value
  // CHECK-ORIGIN: Uninitialized value was created by an allocation of 'uninit' in the stack frame of function 'main'
#elif defined(EXPECT_PASS)
  volatile int uninit = 0;
  #define PRE_RUN poison_stack_and_param()
#else
  #error Bad configuration
#endif

  volatile int a = 1, b = 2;
  if (argc == 2) {
    // Shadow/origin will be passed via va_arg_tls/va_arg_origin_tls.
    if (strcmp(argv[1], "va_arg_tls") == 0) {
      PRE_RUN;
      return sum(3, uninit, a, b);
    }
    // Shadow/origin of |uninit| will be passed via overflow area.
    if (strcmp(argv[1], "overflow") == 0) {
      PRE_RUN;
      return sum(7,
        a, a, a, a, a, a, uninit
      );
    }
  }
  return 0;
}
