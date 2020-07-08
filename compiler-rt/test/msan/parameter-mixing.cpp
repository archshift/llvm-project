// Mixes differently-sized parameters in fucntion calls
// RUN: %clangxx_msan -O0 %s -o %t && %run %t
// RUN: %clangxx_msan -O1 %s -o %t && %run %t
// RUN: %clangxx_msan -O3 %s -o %t && %run %t

#include <cstdarg>
#include <sanitizer/msan_interface.h>

union mini {
    int a;
    char b;
};

struct large {
    char data[0x100];
};

__attribute__((noinline))
void poison_caller_tls_inner(large l) {
    asm volatile ("" :: "r"(&l) );
}
__attribute__((noinline))
void poison_caller_tls() {
    large l;
    poison_caller_tls_inner(l);
}

template <typename T>
void volatile_init(T &val) {
    *(volatile T *)&val = 0;
}

template <typename T>
__attribute__((noinline))
T passthru(T &val) {
    return val;
}

__attribute__((noinline))
void callee(mini m1, large l1, int i1, mini m2, large l2, int i2) {
    mini m1_ = passthru(m1);
    mini m2_ = passthru(m2);
    large l1_ = passthru(l1);
    large l2_ = passthru(l2);
    int i1_ = passthru(i1);
    int i2_ = passthru(i2);

    asm volatile ("" :: "r"(&m1_) );
    asm volatile ("" :: "r"(&m2_) );
    asm volatile ("" :: "r"(&l1_) );
    asm volatile ("" :: "r"(&l2_) );
    asm volatile ("" :: "r"(&i1_) );
    asm volatile ("" :: "r"(&l2_) );
}

__attribute__((noinline))
void callee_va(int _unused, ...) {
    va_list vl;
    va_start(vl, _unused);

    mini m;
    large l;
    int i;

    m = va_arg(vl, mini);
    l = va_arg(vl, large);
    i = va_arg(vl, int);

    __msan_check_mem_is_initialized(&m, sizeof m);
    __msan_check_mem_is_initialized(&l, sizeof l);
    __msan_check_mem_is_initialized(&i, sizeof i);

    m = va_arg(vl, mini);
    l = va_arg(vl, large);
    i = va_arg(vl, int);

    __msan_check_mem_is_initialized(&m, sizeof m);
    __msan_check_mem_is_initialized(&l, sizeof l);
    __msan_check_mem_is_initialized(&i, sizeof i);

    va_end(vl);

}

int main(void) {
    large l = {};
    mini m;
    volatile_init(m.b);
    int i = 0;
    poison_caller_tls();
    callee(m, l, i, m, l, i);

    volatile_init(m.a);
    poison_caller_tls();
    callee_va(0, m, l, i, m, l, i);
}
