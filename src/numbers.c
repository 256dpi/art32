#include <stdbool.h>
#include <stdint.h>

#include "art32/numbers.h"

/*
 * The mapping multiplies the input offset with the output range before dividing by the
 * input range. That product overflows the argument type long before either operand does,
 * so the safe variants below work on unsigned spans, which the offset never exceeds.
 * Spans are also computed unsigned, as the difference of two extreme values overflows the
 * signed type.
 */

static uint32_t a32_span32(int32_t lo, int32_t hi) {
  return hi >= lo ? (uint32_t)hi - (uint32_t)lo : (uint32_t)lo - (uint32_t)hi;
}

static int32_t a32_offset32(int32_t base, uint32_t offset, bool ascending) {
  // apply the offset using unsigned arithmetic, as the result is only guaranteed to fit
  // the signed type after the offset has been applied
  return ascending ? (int32_t)((uint32_t)base + offset) : (int32_t)((uint32_t)base - offset);
}

static uint64_t a32_span(int64_t lo, int64_t hi) {
  return hi >= lo ? (uint64_t)hi - (uint64_t)lo : (uint64_t)lo - (uint64_t)hi;
}

static uint64_t a32_scale(uint64_t offset, uint64_t out_span, uint64_t in_span) {
  // handle empty output range
  if (out_span == 0) {
    return 0;
  }

  // Shift the offset and the input range down in tandem until the product is
  // representable. This preserves their ratio but drops the lowest bits. Only the 64 bit
  // mapping needs this, as the product of two 32 bit spans always fits.
  while (in_span > 1 && offset > UINT64_MAX / out_span) {
    offset >>= 1;
    in_span >>= 1;
  }

  return offset * out_span / in_span;
}

static int64_t a32_offset(int64_t base, uint64_t offset, bool ascending) {
  // apply the offset using unsigned arithmetic, as the result is only guaranteed to fit
  // the signed type after the offset has been applied
  return ascending ? (int64_t)((uint64_t)base + offset) : (int64_t)((uint64_t)base - offset);
}

int32_t a32_constrain_i(int32_t num, int32_t min, int32_t max) { return A32_CONSTRAIN(num, min, max); }

int64_t a32_constrain_l(int64_t num, int64_t min, int64_t max) { return A32_CONSTRAIN(num, min, max); }

float a32_constrain_f(float num, float min, float max) { return A32_CONSTRAIN(num, min, max); }

double a32_constrain_d(double num, double min, double max) { return A32_CONSTRAIN(num, min, max); }

int32_t a32_map_i(int32_t num, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return A32_MAP(num, in_min, in_max, out_min, out_max);
}

int64_t a32_map_l(int64_t num, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max) {
  return A32_MAP(num, in_min, in_max, out_min, out_max);
}

float a32_map_f(float num, float in_min, float in_max, float out_min, float out_max) {
  return A32_MAP(num, in_min, in_max, out_min, out_max);
}

double a32_map_d(double num, double in_min, double in_max, double out_min, double out_max) {
  return A32_MAP(num, in_min, in_max, out_min, out_max);
}

int32_t a32_safe_map_i(int32_t num, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  // handle empty input range (the mapping would divide by zero)
  if (in_min == in_max) {
    return out_min;
  }

  // constrain the input to the input range, as the result is constrained to the output
  // range anyway and a bounded offset keeps the scaling exact
  int32_t n;
  if (in_min < in_max) {
    n = A32_CONSTRAIN(num, in_min, in_max);
  } else {
    n = A32_CONSTRAIN(num, in_max, in_min);
  }

  // Scale the offset into the output range. The product of two 32 bit spans always fits
  // 64 bits, so this needs no overflow handling, and the quotient never exceeds the
  // output span and therefore fits 32 bits again.
  uint32_t offset =
      (uint32_t)((uint64_t)a32_span32(in_min, n) * a32_span32(out_min, out_max) / a32_span32(in_min, in_max));

  return a32_offset32(out_min, offset, out_min <= out_max);
}

int64_t a32_safe_map_l(int64_t num, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max) {
  // handle empty input range (the mapping would divide by zero)
  if (in_min == in_max) {
    return out_min;
  }

  // constrain the input to the input range, as the result is constrained to the output
  // range anyway and a bounded offset keeps the product within the wider type
  int64_t n;
  if (in_min < in_max) {
    n = A32_CONSTRAIN(num, in_min, in_max);
  } else {
    n = A32_CONSTRAIN(num, in_max, in_min);
  }

  // scale the offset into the output range
  uint64_t offset = a32_scale(a32_span(in_min, n), a32_span(out_min, out_max), a32_span(in_min, in_max));

  return a32_offset(out_min, offset, out_min <= out_max);
}

float a32_safe_map_f(float num, float in_min, float in_max, float out_min, float out_max) {
  // handle empty input range (the mapping would divide by zero)
  if (in_min == in_max) {
    return out_min;
  }

  float res = A32_MAP(num, in_min, in_max, out_min, out_max);
  if (out_max < out_min) {
    return A32_CONSTRAIN(res, out_max, out_min);
  }
  return A32_CONSTRAIN(res, out_min, out_max);
}

double a32_safe_map_d(double num, double in_min, double in_max, double out_min, double out_max) {
  // handle empty input range (the mapping would divide by zero)
  if (in_min == in_max) {
    return out_min;
  }

  double res = A32_MAP(num, in_min, in_max, out_min, out_max);
  if (out_max < out_min) {
    return A32_CONSTRAIN(res, out_max, out_min);
  }
  return A32_CONSTRAIN(res, out_min, out_max);
}
