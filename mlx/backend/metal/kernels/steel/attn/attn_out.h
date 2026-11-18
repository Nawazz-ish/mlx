// Copyright © 2026 Apple Inc.

#pragma once

// With FC 305 the attention output buffer holds float32 elements, not T.
constant bool output_f32 [[function_constant(305)]];

// Attention output pointer. Offsets count elements, so they apply to T and
// float32 output the same way.
template <typename T>
struct AttnOut {
  device T* base;
  ulong offset;

  METAL_FUNC AttnOut(device T* ptr) : base(ptr), offset(0) {}

  METAL_FUNC void operator+=(ulong n) thread {
    offset += n;
  }

  // Calls store_fn(dst, tag) with a T or float32 destination; tag gives the
  // element type.
  template <typename F>
  METAL_FUNC void store(F store_fn) const thread {
    if (output_f32) {
      store_fn(reinterpret_cast<device float*>(base) + offset, float(0));
    } else {
      store_fn(base + offset, T(0));
    }
  }
};
