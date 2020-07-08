// Test that different forms of struct padding aren't checked my msan

// RUN: %clangxx_msan -O0 %s -o %t && %run %t
// RUN: %clangxx_msan -O1 %s -o %t && %run %t
// RUN: %clangxx_msan -O3 %s -o %t && %run %t

#include <stdint.h>

struct mini {
    char a;
    int b;
};

struct medium {
    char a;
    uint64_t b;
};

struct large {
    char a;
    uint64_t c[4];
};

struct nested {
    medium a;
};

struct superclass : medium {
};

template <typename T>
void volatile_init(T &val) {
    *(volatile T *)&val = 0;
}

template <typename T>
__attribute__((noinline))
T passthru_byval(T t) {
    return t;
}

template <typename T>
__attribute__((noinline))
T &passthru_byref(T &t) {
    return t;
}

int main() {
    mini m;
    volatile_init(m.a);
    volatile_init(m.b);
    medium med;
    volatile_init(med.a);
    volatile_init(med.b);
    large l;
    volatile_init(l.a);
    volatile_init(l.c[0]);
    volatile_init(l.c[1]);
    volatile_init(l.c[2]);
    volatile_init(l.c[3]);
    nested ns;
    volatile_init(ns.a.a);
    volatile_init(ns.a.b);
    superclass sp;
    volatile_init(sp.a);
    volatile_init(sp.b);

    mini  mval = passthru_byval(m);
    mini &mref = passthru_byref(m);

    medium  medval = passthru_byval(med);
    medium &medref = passthru_byref(med);

    large  lgval = passthru_byval(l);
    large &lgref = passthru_byref(l);

    nested  nsval = passthru_byval(ns);
    nested &nsref = passthru_byref(ns);

    superclass  spval = passthru_byval(sp);
    superclass &spref = passthru_byref(sp);

    asm volatile ("" :: "r"(&mval), "r"(&mref));
    asm volatile ("" :: "r"(&medval), "r"(&medref));
    asm volatile ("" :: "r"(&lgval), "r"(&lgref));
    asm volatile ("" :: "r"(&nsval), "r"(&nsref));
    asm volatile ("" :: "r"(&spval), "r"(&spref));
}
