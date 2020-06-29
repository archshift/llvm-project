; RUN: opt < %s -msan-check-access-address=0 -msan-track-origins=1 -S -passes='module(msan-module),function(msan)' 2>&1 | \
; RUN:   FileCheck -allow-deprecated-dag-overlap -check-prefixes=CHECK,CHECK-ORIGINS %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; CHECK-LABEL: @main
define [2 x i24] @main() sanitize_memory {
; CHECK: %p = inttoptr i64 0 to [2 x i24]*
  %p = inttoptr i64 0 to [2 x i24] *
; CHECK: %o = load [2 x i24], [2 x i24]* %p
  %o = load [2 x i24], [2 x i24] *%p
; CHECK: [[FIELD0:%.+]] = extractvalue [2 x i24] %_msld, 0
; CHECK: [[FIELD1:%.+]] = extractvalue [2 x i24] %_msld, 1
; CHECK: [[F1_OR:%.+]] = or i24 [[FIELD0]], [[FIELD1]]
; CHECK: %_mscmp = icmp ne i24 [[F1_OR]], 0
; CHECK: br i1 %_mscmp
; CHECK: call void @__msan_warning
; CHECK: ret [2 x i24] %o
  ret [2 x i24] %o
}
