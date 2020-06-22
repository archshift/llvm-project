// REQUIRES: arm-registered-target
// RUN: %clang_cc1 -triple arm-none-linux-gnueabi -emit-llvm -w -o - < %s | FileCheck %s
typedef int __attribute__((pcs("aapcs"))) (*aapcs_fn)(void);
typedef int __attribute__((pcs("aapcs-vfp"))) (*aapcs_vfp_fn)(void);

aapcs_fn bar;

int foo(aapcs_vfp_fn baz) {
// CHECK-LABEL: define noundef i32 @foo
// CHECK: call arm_aapcscc noundef
// CHECK: call arm_aapcs_vfpcc noundef
  return bar() + baz();
}
