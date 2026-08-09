#include <gtest/gtest.h>

extern "C" {
#include <art32/vector.h>
}

TEST(Vector, Use) {
  double values[] = {1, 2, 3};
  a32_vector_t vec = a32_vector_use(values, 3);
  ASSERT_EQ(vec.length, (size_t)3);
  ASSERT_EQ(vec.values[0], 1);
  ASSERT_EQ(vec.values[2], 3);
}

TEST(Vector, NewAndFree) {
  a32_vector_t vec = a32_vector_new(4);
  ASSERT_EQ(vec.length, (size_t)4);
  for (size_t i = 0; i < vec.length; i++) {
    ASSERT_EQ(vec.values[i], 0);
  }
  a32_vector_free(vec);
}

TEST(Vector, Make) {
  A32_VECTOR_MAKE(vec, 3);
  ASSERT_EQ(vec.length, (size_t)3);
  for (size_t i = 0; i < vec.length; i++) {
    ASSERT_EQ(vec.values[i], 0);
  }
}

TEST(Vector, Copy) {
  double values[] = {1, 2, 3};
  a32_vector_t src = a32_vector_use(values, 3);
  A32_VECTOR_MAKE(dst, 3);
  a32_vector_copy(dst, src);
  ASSERT_EQ(dst.values[0], 1);
  ASSERT_EQ(dst.values[1], 2);
  ASSERT_EQ(dst.values[2], 3);
}

TEST(Vector, View) {
  double values[] = {1, 2, 3, 4};
  a32_vector_t vec = a32_vector_use(values, 4);
  a32_vector_t view = a32_vector_view(vec, 1, 2);
  ASSERT_EQ(view.length, (size_t)2);
  ASSERT_EQ(view.values[0], 2);
  ASSERT_EQ(view.values[1], 3);

  // a view aliases the source
  view.values[0] = 20;
  ASSERT_EQ(values[1], 20);
}

TEST(Vector, MinMax) {
  double values[] = {3, -1, 4, 1};
  a32_vector_t vec = a32_vector_use(values, 4);
  ASSERT_EQ(a32_vector_max(vec), 4);
  ASSERT_EQ(a32_vector_min(vec), -1);
}

TEST(Vector, Mag) {
  double values[] = {3, 4};
  a32_vector_t vec = a32_vector_use(values, 2);
  ASSERT_DOUBLE_EQ(a32_vector_mag(vec), 5);
}

TEST(Vector, Norm) {
  double values[] = {3, 4};
  a32_vector_t vec = a32_vector_use(values, 2);
  a32_vector_norm(vec);
  ASSERT_DOUBLE_EQ(vec.values[0], 0.6);
  ASSERT_DOUBLE_EQ(vec.values[1], 0.8);
  ASSERT_DOUBLE_EQ(a32_vector_mag(vec), 1);
}

TEST(Vector, NormZero) {
  double values[] = {0, 0};
  a32_vector_t vec = a32_vector_use(values, 2);
  a32_vector_norm(vec);
  ASSERT_EQ(vec.values[0], 0);
  ASSERT_EQ(vec.values[1], 0);
}

TEST(Vector, Multiply) {
  double v1[] = {1, 2, 3};
  double v2[] = {4, 5, 6};
  a32_vector_t vec1 = a32_vector_use(v1, 3);
  a32_vector_t vec2 = a32_vector_use(v2, 3);
  a32_vector_multiply(vec1, vec2);
  ASSERT_EQ(vec1.values[0], 4);
  ASSERT_EQ(vec1.values[1], 10);
  ASSERT_EQ(vec1.values[2], 18);
}

// Regression: min and max seeded themselves from the first element without checking
// that the vector had one.
TEST(Vector, MinMaxEmpty) {
  a32_vector_t vec = a32_vector_new(0);
  ASSERT_EQ(0, a32_vector_max(vec));
  ASSERT_EQ(0, a32_vector_min(vec));
  a32_vector_free(vec);
}
