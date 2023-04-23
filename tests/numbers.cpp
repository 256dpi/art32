#include <gtest/gtest.h>

extern "C" {
#include <art32/numbers.h>
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
