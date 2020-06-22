// RUN: %clang_cc1 -triple x86_64-unknown-unknown -w -S -o - -emit-llvm              %s | FileCheck %s -check-prefix=NO__ERRNO
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -w -S -o - -emit-llvm -fmath-errno %s | FileCheck %s -check-prefix=HAS_ERRNO
// RUN: %clang_cc1 -triple x86_64-unknown-unknown-gnu -w -S -o - -emit-llvm -fmath-errno %s | FileCheck %s --check-prefix=HAS_ERRNO_GNU
// RUN: %clang_cc1 -triple x86_64-unknown-unknown-android -w -S -o - -emit-llvm -fmath-errno %s | FileCheck %s --check-prefix=HAS_ERRNO_ANDROID
// RUN: %clang_cc1 -triple x86_64-unknown-windows-msvc -w -S -o - -emit-llvm -fmath-errno %s | FileCheck %s --check-prefix=HAS_ERRNO_WIN

// Test attributes and codegen of math builtins.

void foo(double *d, float f, float *fp, long double *l, int *i, const char *c) {
  f = __builtin_fmod(f,f);    f = __builtin_fmodf(f,f);   f =  __builtin_fmodl(f,f);

// NO__ERRNO: frem double
// NO__ERRNO: frem float
// NO__ERRNO: frem x86_fp80
// HAS_ERRNO: declare noundef double @fmod(double noundef, double noundef) [[NOT_READNONE:#[0-9]+]]
// HAS_ERRNO: declare noundef float @fmodf(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @fmodl(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_atan2(f,f);    __builtin_atan2f(f,f) ;  __builtin_atan2l(f, f);

// NO__ERRNO: declare noundef double @atan2(double noundef, double noundef) [[READNONE:#[0-9]+]]
// NO__ERRNO: declare noundef float @atan2f(float noundef, float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @atan2l(x86_fp80 noundef, x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @atan2(double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @atan2f(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @atan2l(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_copysign(f,f); __builtin_copysignf(f,f); __builtin_copysignl(f,f); __builtin_copysignf128(f,f);

// NO__ERRNO: declare double @llvm.copysign.f64(double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// NO__ERRNO: declare float @llvm.copysign.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.copysign.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare fp128 @llvm.copysign.f128(fp128, fp128) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.copysign.f64(double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// HAS_ERRNO: declare float @llvm.copysign.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.copysign.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare fp128 @llvm.copysign.f128(fp128, fp128) [[READNONE_INTRINSIC]]

  __builtin_fabs(f);       __builtin_fabsf(f);      __builtin_fabsl(f); __builtin_fabsf128(f);

// NO__ERRNO: declare double @llvm.fabs.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.fabs.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.fabs.f80(x86_fp80) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare fp128 @llvm.fabs.f128(fp128) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.fabs.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.fabs.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.fabs.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare fp128 @llvm.fabs.f128(fp128) [[READNONE_INTRINSIC]]

  __builtin_frexp(f,i);    __builtin_frexpf(f,i);   __builtin_frexpl(f,i);

// NO__ERRNO: declare noundef double @frexp(double noundef, i32* noundef) [[NOT_READNONE:#[0-9]+]]
// NO__ERRNO: declare noundef float @frexpf(float noundef, i32* noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @frexpl(x86_fp80 noundef, i32* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef double @frexp(double noundef, i32* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @frexpf(float noundef, i32* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @frexpl(x86_fp80 noundef, i32* noundef) [[NOT_READNONE]]

  __builtin_huge_val();    __builtin_huge_valf();   __builtin_huge_vall(); __builtin_huge_valf128();

// NO__ERRNO-NOT: .huge
// NO__ERRNO-NOT: @huge
// HAS_ERRNO-NOT: .huge
// HAS_ERRNO-NOT: @huge

  __builtin_inf();    __builtin_inff();   __builtin_infl(); __builtin_inff128();

// NO__ERRNO-NOT: .inf
// NO__ERRNO-NOT: @inf
// HAS_ERRNO-NOT: .inf
// HAS_ERRNO-NOT: @inf

  __builtin_ldexp(f,f);    __builtin_ldexpf(f,f);   __builtin_ldexpl(f,f);  

// NO__ERRNO: declare noundef double @ldexp(double noundef, i32 noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @ldexpf(float noundef, i32 noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @ldexpl(x86_fp80 noundef, i32 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @ldexp(double noundef, i32 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @ldexpf(float noundef, i32 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @ldexpl(x86_fp80 noundef, i32 noundef) [[NOT_READNONE]]

  __builtin_modf(f,d);       __builtin_modff(f,fp);      __builtin_modfl(f,l); 

// NO__ERRNO: declare noundef double @modf(double noundef, double* noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef float @modff(float noundef, float* noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @modfl(x86_fp80 noundef, x86_fp80* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef double @modf(double noundef, double* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @modff(float noundef, float* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @modfl(x86_fp80 noundef, x86_fp80* noundef) [[NOT_READNONE]]

  __builtin_nan(c);        __builtin_nanf(c);       __builtin_nanl(c); __builtin_nanf128(c);

// NO__ERRNO: declare noundef double @nan(i8* noundef) [[PURE:#[0-9]+]]
// NO__ERRNO: declare noundef float @nanf(i8* noundef) [[PURE]]
// NO__ERRNO: declare noundef x86_fp80 @nanl(i8* noundef) [[PURE]]
// NO__ERRNO: declare noundef fp128 @nanf128(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef double @nan(i8* noundef) [[PURE:#[0-9]+]]
// HAS_ERRNO: declare noundef float @nanf(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef x86_fp80 @nanl(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef fp128 @nanf128(i8* noundef) [[PURE]]

  __builtin_nans(c);        __builtin_nansf(c);       __builtin_nansl(c); __builtin_nansf128(c);

// NO__ERRNO: declare noundef double @nans(i8* noundef) [[PURE]]
// NO__ERRNO: declare noundef float @nansf(i8* noundef) [[PURE]]
// NO__ERRNO: declare noundef x86_fp80 @nansl(i8* noundef) [[PURE]]
// NO__ERRNO: declare noundef fp128 @nansf128(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef double @nans(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef float @nansf(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef x86_fp80 @nansl(i8* noundef) [[PURE]]
// HAS_ERRNO: declare noundef fp128 @nansf128(i8* noundef) [[PURE]]

  __builtin_pow(f,f);        __builtin_powf(f,f);       __builtin_powl(f,f);

// NO__ERRNO: declare double @llvm.pow.f64(double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.pow.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.pow.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @pow(double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @powf(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @powl(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_powi(f,f);        __builtin_powif(f,f);       __builtin_powil(f,f);

// NO__ERRNO: declare double @llvm.powi.f64(double, i32) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.powi.f32(float, i32) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.powi.f80(x86_fp80, i32) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.powi.f64(double, i32) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.powi.f32(float, i32) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.powi.f80(x86_fp80, i32) [[READNONE_INTRINSIC]]

  /* math */
  __builtin_acos(f);       __builtin_acosf(f);      __builtin_acosl(f);

// NO__ERRNO: declare noundef double @acos(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @acosf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @acosl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @acos(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @acosf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @acosl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_acosh(f);      __builtin_acoshf(f);     __builtin_acoshl(f);  

// NO__ERRNO: declare noundef double @acosh(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @acoshf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @acoshl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @acosh(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @acoshf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @acoshl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_asin(f);       __builtin_asinf(f);      __builtin_asinl(f); 

// NO__ERRNO: declare noundef double @asin(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @asinf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @asinl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @asin(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @asinf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @asinl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_asinh(f);      __builtin_asinhf(f);     __builtin_asinhl(f);

// NO__ERRNO: declare noundef double @asinh(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @asinhf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @asinhl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @asinh(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @asinhf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @asinhl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_atan(f);       __builtin_atanf(f);      __builtin_atanl(f);

// NO__ERRNO: declare noundef double @atan(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @atanf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @atanl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @atan(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @atanf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @atanl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_atanh(f);      __builtin_atanhf(f);     __builtin_atanhl(f); 

// NO__ERRNO: declare noundef double @atanh(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @atanhf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @atanhl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @atanh(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @atanhf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @atanhl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_cbrt(f);       __builtin_cbrtf(f);      __builtin_cbrtl(f);

// NO__ERRNO: declare noundef double @cbrt(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @cbrtf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @cbrtl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @cbrt(double noundef) [[READNONE:#[0-9]+]]
// HAS_ERRNO: declare noundef float @cbrtf(float noundef) [[READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @cbrtl(x86_fp80 noundef) [[READNONE]]

  __builtin_ceil(f);       __builtin_ceilf(f);      __builtin_ceill(f);

// NO__ERRNO: declare double @llvm.ceil.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.ceil.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.ceil.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.ceil.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.ceil.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.ceil.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_cos(f);        __builtin_cosf(f);       __builtin_cosl(f); 

// NO__ERRNO: declare double @llvm.cos.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.cos.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.cos.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @cos(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @cosf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @cosl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_cosh(f);       __builtin_coshf(f);      __builtin_coshl(f);

// NO__ERRNO: declare noundef double @cosh(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @coshf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @coshl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @cosh(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @coshf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @coshl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_erf(f);        __builtin_erff(f);       __builtin_erfl(f);

// NO__ERRNO: declare noundef double @erf(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @erff(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @erfl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @erf(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @erff(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @erfl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_erfc(f);       __builtin_erfcf(f);      __builtin_erfcl(f);

// NO__ERRNO: declare noundef double @erfc(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @erfcf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @erfcl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @erfc(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @erfcf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @erfcl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_exp(f);        __builtin_expf(f);       __builtin_expl(f);

// NO__ERRNO: declare double @llvm.exp.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.exp.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.exp.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @exp(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @expf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @expl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_exp2(f);       __builtin_exp2f(f);      __builtin_exp2l(f); 

// NO__ERRNO: declare double @llvm.exp2.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.exp2.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.exp2.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @exp2(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @exp2f(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @exp2l(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_expm1(f);      __builtin_expm1f(f);     __builtin_expm1l(f);

// NO__ERRNO: declare noundef double @expm1(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @expm1f(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @expm1l(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @expm1(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @expm1f(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @expm1l(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_fdim(f,f);       __builtin_fdimf(f,f);      __builtin_fdiml(f,f);

// NO__ERRNO: declare noundef double @fdim(double noundef, double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @fdimf(float noundef, float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @fdiml(x86_fp80 noundef, x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @fdim(double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @fdimf(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @fdiml(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_floor(f);      __builtin_floorf(f);     __builtin_floorl(f);

// NO__ERRNO: declare double @llvm.floor.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.floor.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.floor.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.floor.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.floor.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.floor.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fma(f,f,f);        __builtin_fmaf(f,f,f);       __builtin_fmal(f,f,f);

// NO__ERRNO: declare double @llvm.fma.f64(double, double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.fma.f32(float, float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.fma.f80(x86_fp80, x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @fma(double noundef, double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @fmaf(float noundef, float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @fmal(x86_fp80 noundef, x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

// On GNU or Win, fma never sets errno, so we can convert to the intrinsic.

// HAS_ERRNO_GNU: declare double @llvm.fma.f64(double, double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// HAS_ERRNO_GNU: declare float @llvm.fma.f32(float, float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO_GNU: declare x86_fp80 @llvm.fma.f80(x86_fp80, x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

// HAS_ERRNO_ANDROID: declare double @llvm.fma.f64(double, double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// HAS_ERRNO_ANDROID: declare float @llvm.fma.f32(float, float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO_ANDROID: declare x86_fp80 @llvm.fma.f80(x86_fp80, x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

// HAS_ERRNO_WIN: declare double @llvm.fma.f64(double, double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// HAS_ERRNO_WIN: declare float @llvm.fma.f32(float, float, float) [[READNONE_INTRINSIC]]
// Long double is just double on win, so no f80 use/declaration.
// HAS_ERRNO_WIN-NOT: declare x86_fp80 @llvm.fma.f80(x86_fp80, x86_fp80, x86_fp80)

  __builtin_fmax(f,f);       __builtin_fmaxf(f,f);      __builtin_fmaxl(f,f);

// NO__ERRNO: declare double @llvm.maxnum.f64(double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.maxnum.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.maxnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.maxnum.f64(double, double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.maxnum.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.maxnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fmin(f,f);       __builtin_fminf(f,f);      __builtin_fminl(f,f);

// NO__ERRNO: declare double @llvm.minnum.f64(double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.minnum.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.minnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.minnum.f64(double, double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.minnum.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.minnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_hypot(f,f);      __builtin_hypotf(f,f);     __builtin_hypotl(f,f);

// NO__ERRNO: declare noundef double @hypot(double noundef, double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @hypotf(float noundef, float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @hypotl(x86_fp80 noundef, x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @hypot(double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @hypotf(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @hypotl(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_ilogb(f);      __builtin_ilogbf(f);     __builtin_ilogbl(f); 

// NO__ERRNO: declare noundef i32 @ilogb(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef i32 @ilogbf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef i32 @ilogbl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef i32 @ilogb(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i32 @ilogbf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i32 @ilogbl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_lgamma(f);     __builtin_lgammaf(f);    __builtin_lgammal(f);

// NO__ERRNO: declare noundef double @lgamma(double noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef float @lgammaf(float noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @lgammal(x86_fp80 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef double @lgamma(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @lgammaf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @lgammal(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_llrint(f);     __builtin_llrintf(f);    __builtin_llrintl(f);

// NO__ERRNO: declare i64 @llvm.llrint.i64.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.llrint.i64.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.llrint.i64.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef i64 @llrint(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @llrintf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @llrintl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_llround(f);    __builtin_llroundf(f);   __builtin_llroundl(f);

// NO__ERRNO: declare i64 @llvm.llround.i64.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.llround.i64.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.llround.i64.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef i64 @llround(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @llroundf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @llroundl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_log(f);        __builtin_logf(f);       __builtin_logl(f);

// NO__ERRNO: declare double @llvm.log.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.log.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.log.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @log(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @logf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @logl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_log10(f);      __builtin_log10f(f);     __builtin_log10l(f);

// NO__ERRNO: declare double @llvm.log10.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.log10.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.log10.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @log10(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @log10f(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @log10l(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_log1p(f);      __builtin_log1pf(f);     __builtin_log1pl(f);

// NO__ERRNO: declare noundef double @log1p(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @log1pf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @log1pl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @log1p(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @log1pf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @log1pl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_log2(f);       __builtin_log2f(f);      __builtin_log2l(f);

// NO__ERRNO: declare double @llvm.log2.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.log2.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.log2.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @log2(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @log2f(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @log2l(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_logb(f);       __builtin_logbf(f);      __builtin_logbl(f);

// NO__ERRNO: declare noundef double @logb(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @logbf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @logbl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @logb(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @logbf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @logbl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_lrint(f);      __builtin_lrintf(f);     __builtin_lrintl(f);

// NO__ERRNO: declare i64 @llvm.lrint.i64.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.lrint.i64.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.lrint.i64.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef i64 @lrint(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @lrintf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @lrintl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_lround(f);     __builtin_lroundf(f);    __builtin_lroundl(f);

// NO__ERRNO: declare i64 @llvm.lround.i64.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.lround.i64.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare i64 @llvm.lround.i64.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef i64 @lround(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @lroundf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef i64 @lroundl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_nearbyint(f);  __builtin_nearbyintf(f); __builtin_nearbyintl(f);

// NO__ERRNO: declare double @llvm.nearbyint.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.nearbyint.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.nearbyint.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.nearbyint.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.nearbyint.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.nearbyint.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_nextafter(f,f);  __builtin_nextafterf(f,f); __builtin_nextafterl(f,f);

// NO__ERRNO: declare noundef double @nextafter(double noundef, double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @nextafterf(float noundef, float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @nextafterl(x86_fp80 noundef, x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @nextafter(double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @nextafterf(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @nextafterl(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_nexttoward(f,f); __builtin_nexttowardf(f,f);__builtin_nexttowardl(f,f);

// NO__ERRNO: declare noundef double @nexttoward(double noundef, x86_fp80 noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @nexttowardf(float noundef, x86_fp80 noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @nexttowardl(x86_fp80 noundef, x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @nexttoward(double noundef, x86_fp80 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @nexttowardf(float noundef, x86_fp80 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @nexttowardl(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_remainder(f,f);  __builtin_remainderf(f,f); __builtin_remainderl(f,f);

// NO__ERRNO: declare noundef double @remainder(double noundef, double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @remainderf(float noundef, float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @remainderl(x86_fp80 noundef, x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @remainder(double noundef, double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @remainderf(float noundef, float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @remainderl(x86_fp80 noundef, x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_remquo(f,f,i);  __builtin_remquof(f,f,i); __builtin_remquol(f,f,i);

// NO__ERRNO: declare noundef double @remquo(double noundef, double noundef, i32* noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef float @remquof(float noundef, float noundef, i32* noundef) [[NOT_READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @remquol(x86_fp80 noundef, x86_fp80 noundef, i32* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef double @remquo(double noundef, double noundef, i32* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @remquof(float noundef, float noundef, i32* noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @remquol(x86_fp80 noundef, x86_fp80 noundef, i32* noundef) [[NOT_READNONE]]

  __builtin_rint(f);       __builtin_rintf(f);      __builtin_rintl(f);

// NO__ERRNO: declare double @llvm.rint.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.rint.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.rint.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.rint.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.rint.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.rint.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_round(f);      __builtin_roundf(f);     __builtin_roundl(f);

// NO__ERRNO: declare double @llvm.round.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.round.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.round.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.round.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.round.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.round.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_scalbln(f,f);    __builtin_scalblnf(f,f);   __builtin_scalblnl(f,f);

// NO__ERRNO: declare noundef double @scalbln(double noundef, i64 noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @scalblnf(float noundef, i64 noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @scalblnl(x86_fp80 noundef, i64 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @scalbln(double noundef, i64 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @scalblnf(float noundef, i64 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @scalblnl(x86_fp80 noundef, i64 noundef) [[NOT_READNONE]]

  __builtin_scalbn(f,f);     __builtin_scalbnf(f,f);    __builtin_scalbnl(f,f);

// NO__ERRNO: declare noundef double @scalbn(double noundef, i32 noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @scalbnf(float noundef, i32 noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @scalbnl(x86_fp80 noundef, i32 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @scalbn(double noundef, i32 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @scalbnf(float noundef, i32 noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @scalbnl(x86_fp80 noundef, i32 noundef) [[NOT_READNONE]]

  __builtin_sin(f);        __builtin_sinf(f);       __builtin_sinl(f);

// NO__ERRNO: declare double @llvm.sin.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.sin.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.sin.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @sin(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @sinf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @sinl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_sinh(f);       __builtin_sinhf(f);      __builtin_sinhl(f);

// NO__ERRNO: declare noundef double @sinh(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @sinhf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @sinhl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @sinh(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @sinhf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @sinhl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_sqrt(f);       __builtin_sqrtf(f);      __builtin_sqrtl(f); 

// NO__ERRNO: declare double @llvm.sqrt.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.sqrt.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.sqrt.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare noundef double @sqrt(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @sqrtf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @sqrtl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_tan(f);        __builtin_tanf(f);       __builtin_tanl(f);

// NO__ERRNO: declare noundef double @tan(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @tanf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @tanl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @tan(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @tanf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @tanl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_tanh(f);       __builtin_tanhf(f);      __builtin_tanhl(f);

// NO__ERRNO: declare noundef double @tanh(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @tanhf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @tanhl(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @tanh(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @tanhf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @tanhl(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_tgamma(f);     __builtin_tgammaf(f);    __builtin_tgammal(f);

// NO__ERRNO: declare noundef double @tgamma(double noundef) [[READNONE]]
// NO__ERRNO: declare noundef float @tgammaf(float noundef) [[READNONE]]
// NO__ERRNO: declare noundef x86_fp80 @tgammal(x86_fp80 noundef) [[READNONE]]
// HAS_ERRNO: declare noundef double @tgamma(double noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef float @tgammaf(float noundef) [[NOT_READNONE]]
// HAS_ERRNO: declare noundef x86_fp80 @tgammal(x86_fp80 noundef) [[NOT_READNONE]]

  __builtin_trunc(f);      __builtin_truncf(f);     __builtin_truncl(f);

// NO__ERRNO: declare double @llvm.trunc.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.trunc.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.trunc.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.trunc.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.trunc.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.trunc.f80(x86_fp80) [[READNONE_INTRINSIC]]
};


// NO__ERRNO: attributes [[READNONE]] = { {{.*}}readnone{{.*}} }
// NO__ERRNO: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }
// NO__ERRNO: attributes [[NOT_READNONE]] = { nounwind "correctly{{.*}} }
// NO__ERRNO: attributes [[PURE]] = { {{.*}}readonly{{.*}} }

// HAS_ERRNO: attributes [[NOT_READNONE]] = { nounwind "correctly{{.*}} }
// HAS_ERRNO: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }
// HAS_ERRNO: attributes [[PURE]] = { {{.*}}readonly{{.*}} }
// HAS_ERRNO: attributes [[READNONE]] = { {{.*}}readnone{{.*}} }

// HAS_ERRNO_GNU: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }
// HAS_ERRNO_ANDROID: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }
// HAS_ERRNO_WIN: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }

