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

// Regression: a zero sized history yielded an object whose update read and wrote past
// the allocation and then divided by a zero count.
TEST(Smooth, ZeroSize) { ASSERT_EQ(nullptr, a32_smooth_new(0)); }

// Regression: the total was carried across updates, so it accumulated the rounding error
// of every reading ever added and never returned to zero.
TEST(Smooth, NoDrift) {
  a32_smooth_t* smooth = a32_smooth_new(8);

  // churn through readings that do not sum exactly
  for (int i = 0; i < 100000; i++) {
    a32_smooth_update(smooth, 0.1f + (float)(i % 3) * 0.7f);
  }

  // flushing the history with zeros must yield exactly zero again
  float last = 0;
  for (int i = 0; i < 8; i++) {
    last = a32_smooth_update(smooth, 0.0f);
  }
  EXPECT_EQ(0.0f, last);
  EXPECT_EQ(0.0f, smooth->total);
  EXPECT_EQ(0.0f, smooth->min);
  EXPECT_EQ(0.0f, smooth->max);

  a32_smooth_free(smooth);
}

// Freeing is expected to accept NULL like free does, as new returns NULL on failure.
TEST(Smooth, FreeNull) { a32_smooth_free(nullptr); }
