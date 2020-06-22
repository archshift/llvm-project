// REQUIRES: powerpc-registered-target
// RUN: %clang_cc1 -triple powerpc-unknown-aix \
// RUN:   -emit-llvm -o - %s | FileCheck %s --check-prefixes=AIX,AIX32
// RUN: %clang_cc1 -triple powerpc64-unknown-aix \
// RUN:   -emit-llvm -o - %s | FileCheck %s --check-prefixes=AIX,AIX64

// AIX-LABEL: define void @retVoid()
void retVoid(void) {}

// AIX-LABEL: define noundef signext i8 @retChar(i8 noundef signext %x)
char retChar(char x) { return x; }

// AIX-LABEL: define noundef signext i16 @retShort(i16 noundef signext %x)
short retShort(short x) { return x; }

// AIX32-LABEL: define noundef i32 @retInt(i32 noundef %x)
// AIX64-LABEL: define noundef signext i32 @retInt(i32 noundef signext %x)
int retInt(int x) { return 1; }

// AIX-LABEL: define noundef i64 @retLongLong(i64 noundef %x)
long long retLongLong(long long x) { return x; }

// AIX-LABEL: define noundef signext i8 @retEnumChar(i8 noundef signext %x)
enum EnumChar : char { IsChar };
enum EnumChar retEnumChar(enum EnumChar x) {
  return x;
}

// AIX32-LABEL: define noundef i32 @retEnumInt(i32 noundef %x)
// AIX64-LABEL: define noundef signext i32 @retEnumInt(i32 noundef signext %x)
enum EnumInt : int { IsInt };
enum EnumInt retEnumInt(enum EnumInt x) {
  return x;
}
