; RUN: opt < %s -msan-check-access-address=0 -msan-track-origins=1 -msan-eager-checks -S -passes='module(msan-module),function(msan)' 2>&1 | \
; RUN:   FileCheck -allow-deprecated-dag-overlap -check-prefixes=CHECK,CHECK-ORIGINS %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; CHECK: @llvm.global_ctors {{.*}} { i32 0, void ()* @msan.module_ctor, i8* null }

; Check the presence and the linkage type of __msan_track_origins and
; other interface symbols.
; CHECK-NOT: @__msan_track_origins
; CHECK-ORIGINS: @__msan_track_origins = weak_odr constant i32 1
; CHECK-NOT: @__msan_keep_going = weak_odr constant i32 0
; CHECK: @__msan_retval_tls = external thread_local(initialexec) global [{{.*}}]
; CHECK: @__msan_retval_origin_tls = external thread_local(initialexec) global i32
; CHECK: @__msan_param_tls = external thread_local(initialexec) global [{{.*}}]
; CHECK: @__msan_param_origin_tls = external thread_local(initialexec) global [{{.*}}]
; CHECK: @__msan_va_arg_tls = external thread_local(initialexec) global [{{.*}}]
; CHECK: @__msan_va_arg_overflow_size_tls = external thread_local(initialexec) global i64
; CHECK: @__msan_origin_tls = external thread_local(initialexec) global i32


define i32 @NormalRet() nounwind uwtable sanitize_memory {
  ret i32 123
}

; CHECK-LABEL: @NormalRet
; CHECK: ret i32

define partialinit i32 @PartialRet() nounwind uwtable sanitize_memory {
  ret i32 123
}

; CHECK-LABEL: @PartialRet
; CHECK: store i32 0,{{.*}}__msan_retval_tls
; CHECK-NOT: call void @__msan_warning_noreturn()
; CHECK: ret i32

define i32 @LoadedRet() nounwind uwtable sanitize_memory {
  %p = inttoptr i64 0 to i32 *
  %o = load i32, i32 *%p
  ret i32 %o
}

; CHECK-LABEL: @LoadedRet
; CHECK-NOT: __msan_retval_tls
; CHECK: call void @__msan_warning_noreturn()
; CHECK: unreachable
; CHECK-NOT: __msan_retval_tls
; CHECK: ret i32 %o


define void @NormalArg(i32 %a) nounwind uwtable sanitize_memory {
  %p = inttoptr i64 0 to i32 *
  store i32 %a, i32 *%p
  ret void
}

; CHECK-LABEL: @NormalArg
; CHECK-NOT: __msan_retval_tls
; CHECK: %p = inttoptr
; CHECK: ret void

define void @PartialArg(i32 partialinit %a) nounwind uwtable sanitize_memory {
  %p = inttoptr i64 0 to i32 *
  store i32 %a, i32 *%p
  ret void
}

; CHECK-LABEL: @PartialArg
; CHECK: load {{.*}}__msan_param_tls
; CHECK: %p = inttoptr
; CHECK: ret void

define void @CallNormal() nounwind uwtable sanitize_memory {
  %r = call i32 @NormalRet() nounwind uwtable sanitize_memory
  call void @NormalArg(i32 %r) nounwind uwtable sanitize_memory
  ret void
}

; CHECK-LABEL: @CallNormal
; CHECK: call i32 @NormalRet()
; CHECK-NOT: __msan_{{\w+}}_tls
; CHECK: call void @NormalArg
; CHECK: ret void

define void @CallWithLoaded() nounwind uwtable sanitize_memory {
  %p = inttoptr i64 0 to i32 *
  %o = load i32, i32 *%p
  call void @NormalArg(i32 %o) nounwind uwtable sanitize_memory
  ret void
}

; CHECK-LABEL: @CallWithLoaded
; CHECK: %p = inttoptr
; CHECK-NOT: __msan_{{\w+}}_tls
; CHECK: call void @__msan_warning_noreturn()
; CHECK: unreachable
; CHECK-NOT: __msan_{{\w+}}_tls
; CHECK: call void @NormalArg
; CHECK: ret void

define void @CallPartial() nounwind uwtable sanitize_memory {
  %r = call i32 @PartialRet() nounwind uwtable sanitize_memory
  call void @PartialArg(i32 %r) nounwind uwtable sanitize_memory
  ret void
}

; CHECK-LABEL: @CallPartial
; CHECK: call i32 @PartialRet()
; CHECK: load {{.*}}__msan_retval_tls
; CHECK: store {{.*}}__msan_param_tls
; CHECK: call void @PartialArg
; CHECK: ret void