; Tests if MSAN can optimize away dead-code alloca poisons

; RUN: opt < %s -msan-track-origins=2 -msan-check-access-address=0 -msan-poison-stack-with-call=1 -S -passes=msan 2>&1 | FileCheck  \
; RUN: %s "--check-prefixes=CHECK"

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32* @simple(i32 %val) sanitize_memory {
  ; CHECK: define {{.*}} @simple
  %p = alloca i32
  store i32 %val, i32* %p
  ; CHECK-NOT: call void @__msan_poison_stack
  ret i32* %p
}

define i32* @bitcast({ i32 } %val) sanitize_memory {
  ; CHECK: define {{.*}} @bitcast
  %p = alloca i32
  %ps = bitcast i32* %p to { i32 }*
  store { i32 } %val, { i32 }* %ps
  ; CHECK-NOT: call void @__msan_poison_stack
  ret i32* %p
}

define i32* @lifetime(i32 %val) sanitize_memory {
  ; CHECK: define {{.*}} @lifetime
  %p = alloca i32
  %p8 = bitcast i32* %p to i8*
  call void @llvm.lifetime.start(i64 4, i8* %p8)
  store i32 %val, i32* %p
  ; CHECK-NOT: call void @__msan_poison_stack
  call void @llvm.lifetime.end(i64 4, i8* %p8)
  ret i32* %p
}

define i32* @with_memset(i8 %val) sanitize_memory {
  ; CHECK: define {{.*}} @with_memset
  %p = alloca i32
  %p8 = bitcast i32* %p to i8*
  call void @llvm.memset(i8* %p8, i8 0, i64 4, i1 false)
  ; CHECK-NOT: call void @__msan_poison_stack
  ret i32* %p
}

%struct.multi = type { i32, { i8, i8 }, i8, i8 }

define i32* @piecemeal(i8 %val) sanitize_memory {
  ; CHECK: define {{.*}} @piecemeal
  %p = alloca %struct.multi
  %p0 = getelementptr %struct.multi, %struct.multi* %p, i32 0, i32 0
  %p1 = getelementptr %struct.multi, %struct.multi* %p, i32 0, i32 1
  %p2 = getelementptr %struct.multi, %struct.multi* %p, i32 0, i32 2
  %p3 = getelementptr %struct.multi, %struct.multi* %p, i32 0, i32 3
  %p1_0 = getelementptr { i8, i8 }, { i8, i8 }* %p1, i32 0, i32 0
  %p1_1 = getelementptr { i8, i8 }, { i8, i8 }* %p1, i32 0, i32 1
  store i32 0, i32* %p0
  store i8 0, i8* %p1_0
  store i8 0, i8* %p1_1
  store i8 0, i8* %p2
  store i8 0, i8* %p3
  ; CHECK-NOT: call void @__msan_poison_stack
  ret i32* %p0
}

declare void @llvm.memset(i8*, i8, i64, i1)
declare void @llvm.lifetime.start(i64, i8* nocapture)
declare void @llvm.lifetime.end(i64, i8* nocapture)
