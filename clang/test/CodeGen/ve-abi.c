// RUN: %clang_cc1 -triple ve-linux-gnu -emit-llvm %s -o - | FileCheck %s

// CHECK-LABEL: define noundef { float, float } @p(float noundef %a.coerce0, float noundef %a.coerce1, float noundef %b.coerce0, float noundef %b.coerce1) #0 
float __complex__ p(float __complex__ a, float __complex__ b) {
}

// CHECK-LABEL: define noundef { double, double } @q(double noundef %a.coerce0, double noundef %a.coerce1, double noundef %b.coerce0, double noundef %b.coerce1) #0 
double __complex__ q(double __complex__ a, double __complex__ b) {
}

void func() {
  // CHECK-LABEL: %call = call noundef i32 (i32, i32, i32, i32, i32, i32, i32, ...) bitcast (i32 (...)* @hoge to i32 (i32, i32, i32, i32, i32, i32, i32, ...)*)(i32 noundef 1, i32 noundef 2, i32 noundef 3, i32 noundef 4, i32 noundef 5, i32 noundef 6, i32 noundef 7)
  hoge(1, 2, 3, 4, 5, 6, 7);
}
