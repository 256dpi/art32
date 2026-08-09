#include <gtest/gtest.h>

#include <algorithm>
#include <deque>
#include <random>
#include <vector>

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

// Regression: a zero sized history yielded an object whose update wrote past the
// allocation and then indexed the sorted values with an underflowed count.
TEST(Filter, ZeroSize) { ASSERT_EQ(nullptr, a32_filter_new(0)); }

// Freeing is expected to accept NULL like free does, as new returns NULL on failure.
TEST(Filter, FreeNull) { a32_filter_free(nullptr); }

// The sorted values are maintained across updates instead of being re-sorted, so compare
// a long run of readings against a median computed from scratch. Duplicates and a window
// that is still filling are covered by the small value range and the short warm-up.
TEST(Filter, MatchesReferenceMedian) {
  for (size_t num : {(size_t)1, (size_t)2, (size_t)5, (size_t)8, (size_t)17}) {
    a32_filter_t* filter = a32_filter_new(num);
    ASSERT_NE(nullptr, filter);

    std::mt19937 rng(1234 + (unsigned)num);
    std::uniform_int_distribution<int> dist(-4, 4);
    std::deque<double> window;

    for (int i = 0; i < 2000; i++) {
      double v = dist(rng);

      // maintain the reference window
      window.push_back(v);
      if (window.size() > num) {
        window.pop_front();
      }

      // compute the reference median from scratch
      std::vector<double> sorted(window.begin(), window.end());
      std::sort(sorted.begin(), sorted.end());
      size_t n = sorted.size();
      double want = n % 2 == 1 ? sorted[n / 2] : (sorted[n / 2 - 1] + sorted[n / 2]) / 2;

      EXPECT_EQ(want, a32_filter_update(filter, v)) << "num " << num << " step " << i;

      // the maintained values must stay sorted
      for (size_t j = 1; j < filter->count; j++) {
        ASSERT_LE(filter->sorted[j - 1], filter->sorted[j]) << "num " << num << " step " << i;
      }
      ASSERT_EQ(n, filter->count);
    }

    a32_filter_free(filter);
  }
}
