#pragma once

#include <utility>

#include "sanitizer_allocator_internal.h"
#include "sanitizer_common.h"
#include "sanitizer_internal_defs.h"
#include "sanitizer_platform.h"

namespace __sanitizer {

template <uptr capacity>
struct HashCache {
  u32 keys[capacity] = {};
  u32 vals[capacity] = {};

  constexpr HashCache() {}

  bool get(u32 key, u32 *val) {
    uptr idx = key & (capacity - 1);
    if (keys[idx] != key)
      return false;
    if (val)
      *val = vals[idx];
    return true;
  }

  bool insert(u32 key, u32 val) {
    uptr idx = key & (capacity - 1);
    if (keys[idx] == key)
      return false;

    u32 *keyslot = &keys[idx];
    u32 *valslot = &vals[idx];

    *keyslot = key;
    *valslot = val;
    return true;
  }
};

template <uptr N>
struct ConcurrentHashCache {
  atomic_uint64_t data[N];

  bool get(u32 key, u32 *val) {
    u64 data = atomic_load(&this->data[key % N], memory_order_relaxed);
    u32 found_key = data;
    u32 found_val = data >> 32;
    *val = found_val;
    return key == found_key;
  }

  void insert(u32 key, u32 val) {
    u64 data = (u64)val << 32 | key;
    atomic_store(&this->data[key % N], data, memory_order_relaxed);
  }
};
}  // namespace __sanitizer
