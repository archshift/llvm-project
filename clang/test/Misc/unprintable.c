// RUN: not %clang_cc1 %s -fmessage-length=40 2>&1 | FileCheck -strict-whitespace %s

int main() {
    int i;
    if((i==/*￾*/1));

// CHECK: {{^    if\(\(i==/\*<U\+FFFE>\*/1\)\);}}

// CHECK: {{^        ~\^~~~~~~~~~~~~~~}}
// CHECK: {{^       ~ \^              ~}}

