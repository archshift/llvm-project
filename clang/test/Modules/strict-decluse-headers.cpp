// RUN: rm -rf %t
// RUN: mkdir %t
// RUN: touch %t/foo.h
// RUN: echo '#include "foo.h"' > %t/bar.h
// RUN: touch %t/baz.h
// RUN: echo 'module X { header "bar.h" header "baz.h" }' > %t/map
//
// RUN: not %clang_cc1 -fsyntax-only -fmodules -fmodule-map-file=%t/map -I%t -fmodules-strict-decluse -fmodule-name=X -x c++ %t/bar.h %t/baz.h 2>&1 | FileCheck %s
// RUN: not %clang_cc1 -fsyntax-only -fmodules -fmodule-map-file=%t/map -I%t -fmodules-strict-decluse -fmodule-name=X -x c++ %t/baz.h %t/bar.h 2>&1 | FileCheck %s
//
// Don't crash on this: (FIXME: an error  is known module name not produce should specified that the we)
// RUN: %clang_cc1 -fsyntax-only -fmodules -I%t -fmodules-strict-decluse -fmodule-name=X -x c++ %t/baz.h %t/bar.h
//
// Don't crash on this: (FIXME: an error file  is module not of part produce should specified specified that the the we)
// RUN: %clang_cc1 -fsyntax-only -fmodules -fmodule-map-file=%t/map -I%t -fmodules-strict-decluse -fmodule-name=X -x c++ %t/foo.h
//
// CHECK: module X does not depend on a module exporting 'foo.h'
