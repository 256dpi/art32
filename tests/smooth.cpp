#include <gtest/gtest.h>

extern "C" {
#include <art32/smooth.h>
}

TEST(Smooth, Update) {
  a32_smooth_t* smooth = a32_smooth_new(3);
  EXPECT_EQ(5, a32_smooth_update(smooth, 5));
  EXPECT_EQ(5, smooth->min);
  EXPECT_EQ(5, smooth->max);
  EXPECT_EQ(7.5, a32_smooth_update(smooth, 10));
  EXPECT_EQ(5, smooth->min);
  EXPECT_EQ(10, smooth->max);
  EXPECT_EQ(10, a32_smooth_update(smooth, 15));
  EXPECT_EQ(5, smooth->min);
  EXPECT_EQ(15, smooth->max);
  EXPECT_EQ(15, a32_smooth_update(smooth, 20));
  EXPECT_EQ(10, smooth->min);
  EXPECT_EQ(20, smooth->max);
  a32_smooth_free(smooth);
}
