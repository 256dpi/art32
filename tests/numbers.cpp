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
