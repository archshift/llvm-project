// Test that MSAN does not trigger on oddly-sized bitfields

// RUN: %clangxx_msan -O0 %s -o %t && %run %t
// RUN: %clangxx_msan -O1 %s -o %t && %run %t
// RUN: %clangxx_msan -O3 %s -o %t && %run %t

#include <sanitizer/msan_interface.h>

struct small {
    char field : 5;
};

struct multiple_promote24 {
    int field1 : 23;
    int field2 : 23;
    int field3 : 23;
};

small make_small() {
    small s;
    ((volatile small *)&s)->field = 0;
    return s;
}

multiple_promote24 make_promote24() {
    multiple_promote24 m;
    ((volatile multiple_promote24 *)&m)->field1 = 0;
    ((volatile multiple_promote24 *)&m)->field2 = 0;
    ((volatile multiple_promote24 *)&m)->field3 = 0;
    return m;
}

int main(void) {
    volatile small s = make_small();
    volatile multiple_promote24 m = make_promote24();

    int check;
    check = s.field;
    __msan_check_mem_is_initialized(&check, sizeof check);
    check = m.field1;
    __msan_check_mem_is_initialized(&check, sizeof check);
    check = m.field2;
    __msan_check_mem_is_initialized(&check, sizeof check);
    check = m.field3;
    __msan_check_mem_is_initialized(&check, sizeof check);
}
