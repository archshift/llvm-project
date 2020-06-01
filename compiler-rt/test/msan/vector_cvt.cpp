// RUN: %clangxx_msan -O0 %s -o %t && %run %t
// RUN: %clangxx_msan -DPOSITIVE -O0 %s -o %t && not %run %t 2>&1 | FileCheck %s
// REQUIRES: x86_64-target-arch

#include <emmintrin.h>

// Bypasses eager checking
union Double {
  double v;
  char c;
};

int to_int(Double v) {
  __m128d t = _mm_set_sd(v.v);
  int x = _mm_cvtsd_si32(t);
  return x;
  // CHECK: WARNING: MemorySanitizer: use-of-uninitialized-value
  // CHECK: #{{.*}} in to_int{{.*}}vector_cvt.cpp:[[@LINE-3]]
}

int main() {
#ifdef POSITIVE
  double v;
#else
  double v = 1.1;
#endif
  Double* volatile p = (Double *)&v;
  int x = to_int(*p);
  return !x;
}
