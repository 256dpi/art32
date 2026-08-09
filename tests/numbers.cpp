#include <gtest/gtest.h>

extern "C" {
#include <art32/numbers.h>
}

// Regression: the macro arguments were not parenthesized, so an argument binding looser
// than the comparison operators was regrouped.
TEST(Numbers, MacroPrecedence) {
  int a = 6, b = 5;

  // `&` binds looser than `>`, so an unparenthesized `num` became `a & (b > max)`
  EXPECT_EQ(3, A32_CONSTRAIN(a & b, 0, 3));

  // an unparenthesized `out_min` turned `100 - (a - b)` into `100 - a - b`
  EXPECT_EQ(100, A32_MAP(10, 0, 10, a - b, 100));
}

TEST(Numbers, Constrain) {
  EXPECT_EQ(2, a32_constrain_i(1, 2, 3));
  EXPECT_EQ(2, a32_constrain_l(1, 2, 3));
  EXPECT_EQ(2, a32_constrain_f(1, 2, 3));
  EXPECT_EQ(2, a32_constrain_d(1, 2, 3));

  EXPECT_EQ(3, a32_constrain_i(4, 2, 3));
  EXPECT_EQ(3, a32_constrain_l(4, 2, 3));
  EXPECT_EQ(3, a32_constrain_f(4, 2, 3));
  EXPECT_EQ(3, a32_constrain_d(4, 2, 3));
}

TEST(Numbers, Map) {
  EXPECT_EQ(50, a32_map_i(5, 0, 10, 0, 100));
  EXPECT_EQ(50, a32_map_l(5, 0, 10, 0, 100));
  EXPECT_EQ(50, a32_map_f(5, 0, 10, 0, 100));
  EXPECT_EQ(50, a32_map_d(5, 0, 10, 0, 100));

  EXPECT_EQ(40, a32_map_i(5, 0, 10, 80, 0));
  EXPECT_EQ(40, a32_map_l(5, 0, 10, 80, 0));
  EXPECT_EQ(40, a32_map_f(5, 0, 10, 80, 0));
  EXPECT_EQ(40, a32_map_d(5, 0, 10, 80, 0));

  EXPECT_EQ(10, a32_map_i(1, 3, 8, 30, 80));
  EXPECT_EQ(10, a32_map_l(1, 3, 8, 30, 80));
  EXPECT_EQ(10, a32_map_f(1, 3, 8, 30, 80));
  EXPECT_EQ(10, a32_map_d(1, 3, 8, 30, 80));

  EXPECT_EQ(100, a32_map_i(10, 3, 8, 30, 80));
  EXPECT_EQ(100, a32_map_l(10, 3, 8, 30, 80));
  EXPECT_EQ(100, a32_map_f(10, 3, 8, 30, 80));
  EXPECT_EQ(100, a32_map_d(10, 3, 8, 30, 80));
}

TEST(Numbers, SafeMap) {
  EXPECT_EQ(50, a32_safe_map_i(5, 0, 10, 0, 100));
  EXPECT_EQ(50, a32_safe_map_l(5, 0, 10, 0, 100));
  EXPECT_EQ(50, a32_safe_map_f(5, 0, 10, 0, 100));
  EXPECT_EQ(50, a32_safe_map_d(5, 0, 10, 0, 100));

  EXPECT_EQ(40, a32_safe_map_i(5, 0, 10, 80, 0));
  EXPECT_EQ(40, a32_safe_map_l(5, 0, 10, 80, 0));
  EXPECT_EQ(40, a32_safe_map_f(5, 0, 10, 80, 0));
  EXPECT_EQ(40, a32_safe_map_d(5, 0, 10, 80, 0));

  EXPECT_EQ(30, a32_safe_map_i(1, 3, 8, 30, 80));
  EXPECT_EQ(30, a32_safe_map_l(1, 3, 8, 30, 80));
  EXPECT_EQ(30, a32_safe_map_f(1, 3, 8, 30, 80));
  EXPECT_EQ(30, a32_safe_map_d(1, 3, 8, 30, 80));

  EXPECT_EQ(80, a32_safe_map_i(10, 3, 8, 30, 80));
  EXPECT_EQ(80, a32_safe_map_l(10, 3, 8, 30, 80));
  EXPECT_EQ(80, a32_safe_map_f(10, 3, 8, 30, 80));
  EXPECT_EQ(80, a32_safe_map_d(10, 3, 8, 30, 80));
}

// Regression: the mapping multiplied the input offset with the output range in the
// argument type, which overflowed for ranges that are individually representable.
TEST(Numbers, SafeMapOverflow) {
  // both of these overflow an int32 intermediate
  EXPECT_EQ(50000000, a32_safe_map_i(50, 0, 100, 0, 100000000));
  EXPECT_EQ(50000, a32_safe_map_i(1000000, 0, 2000000, 0, 100000));

  // the widest ranges the types can express
  EXPECT_EQ(0, a32_safe_map_i(0, INT32_MIN, INT32_MAX, INT32_MIN, INT32_MAX));
  EXPECT_EQ(INT32_MIN, a32_safe_map_i(INT32_MIN, INT32_MIN, INT32_MAX, INT32_MIN, INT32_MAX));
  EXPECT_EQ(INT32_MAX, a32_safe_map_i(INT32_MAX, INT32_MIN, INT32_MAX, INT32_MIN, INT32_MAX));
  EXPECT_EQ(INT64_MIN, a32_safe_map_l(INT64_MIN, INT64_MIN, INT64_MAX, INT64_MIN, INT64_MAX));
  EXPECT_EQ(INT64_MAX, a32_safe_map_l(INT64_MAX, INT64_MIN, INT64_MAX, INT64_MIN, INT64_MAX));

  // the 64 bit mapping is exact as long as the product fits
  EXPECT_EQ(50000000, a32_safe_map_l(50, 0, 100, 0, 100000000));
  EXPECT_EQ(1500000000000LL, a32_safe_map_l(3000000, 0, 4000000, 0, 2000000000000LL));

  // an inverted input range maps in reverse
  EXPECT_EQ(0, a32_safe_map_i(100, 100, 0, 0, 100000000));
  EXPECT_EQ(100000000, a32_safe_map_i(0, 100, 0, 0, 100000000));
  EXPECT_EQ(50000000, a32_safe_map_i(50, 100, 0, 0, 100000000));
}

// The safe variants constrain out of range inputs instead of extrapolating.
TEST(Numbers, SafeMapConstrainsInput) {
  EXPECT_EQ(0, a32_safe_map_i(INT32_MIN, 0, 100, 0, 100000000));
  EXPECT_EQ(100000000, a32_safe_map_i(INT32_MAX, 0, 100, 0, 100000000));
  EXPECT_EQ(100000000, a32_safe_map_i(INT32_MIN, 0, 100, 100000000, 0));
  EXPECT_EQ(0, a32_safe_map_i(INT32_MAX, 0, 100, 100000000, 0));
}

// The safe variants must agree with the plain ones wherever those do not overflow.
TEST(Numbers, SafeMapMatchesPlainMap) {
  const int32_t points[] = {-1000, -37, -1, 0, 1, 7, 100, 999};
  const int32_t bounds[] = {-500, -3, 0, 2, 250};

  for (int32_t num : points) {
    for (int32_t in_min : bounds) {
      for (int32_t in_max : bounds) {
        if (in_min == in_max) {
          continue;
        }
        for (int32_t out_min : bounds) {
          for (int32_t out_max : bounds) {
            // the plain mapping extrapolates, so only compare within the input range
            int32_t lo = in_min < in_max ? in_min : in_max;
            int32_t hi = in_min < in_max ? in_max : in_min;
            if (num < lo || num > hi) {
              continue;
            }

            int32_t want = a32_map_i(num, in_min, in_max, out_min, out_max);
            int32_t got = a32_safe_map_i(num, in_min, in_max, out_min, out_max);
            EXPECT_EQ(want, got) << num << " in [" << in_min << "," << in_max << "] out [" << out_min << "," << out_max
                                 << "]";

            int64_t got_l = a32_safe_map_l(num, in_min, in_max, out_min, out_max);
            EXPECT_EQ((int64_t)want, got_l);
          }
        }
      }
    }
  }
}

// Regression: an empty input range divided by zero.
TEST(Numbers, SafeMapEmptyRange) {
  EXPECT_EQ(7, a32_safe_map_i(5, 3, 3, 7, 100));
  EXPECT_EQ(7, a32_safe_map_l(5, 3, 3, 7, 100));
  EXPECT_EQ(7, a32_safe_map_f(5, 3, 3, 7, 100));
  EXPECT_EQ(7, a32_safe_map_d(5, 3, 3, 7, 100));

  // also holds for an inverted output range
  EXPECT_EQ(100, a32_safe_map_i(5, 3, 3, 100, 7));
  EXPECT_EQ(100, a32_safe_map_l(5, 3, 3, 100, 7));
  EXPECT_EQ(100, a32_safe_map_f(5, 3, 3, 100, 7));
  EXPECT_EQ(100, a32_safe_map_d(5, 3, 3, 100, 7));
}
