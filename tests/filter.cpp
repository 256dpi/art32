#include <gtest/gtest.h>

extern "C" {
#include <art32/filter.h>
}

TEST(Filter, Update) {
  a32_filter_t* filter = a32_filter_new(5);
  EXPECT_EQ(5, a32_filter_update(filter, 5));
  EXPECT_EQ(5, a32_filter_update(filter, 5));
  EXPECT_EQ(5, a32_filter_update(filter, 15));
  EXPECT_EQ(7.5, a32_filter_update(filter, 10));
  EXPECT_EQ(10, a32_filter_update(filter, 15));
  EXPECT_EQ(15, a32_filter_update(filter, 15));
  a32_filter_free(filter);
}

// Regression: the insertion sort walked off the front of the buffer whenever the new
// value sorted to position zero.
TEST(Filter, DescendingValues) {
  a32_filter_t* filter = a32_filter_new(5);
  EXPECT_EQ(10, a32_filter_update(filter, 10));
  EXPECT_EQ(7.5, a32_filter_update(filter, 5));
  EXPECT_EQ(5, a32_filter_update(filter, 4));
  EXPECT_EQ(4.5, a32_filter_update(filter, 3));
  EXPECT_EQ(4, a32_filter_update(filter, 2));
  EXPECT_EQ(3, a32_filter_update(filter, 1));
  a32_filter_free(filter);
}
