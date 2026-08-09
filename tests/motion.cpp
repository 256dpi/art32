#include <gtest/gtest.h>

extern "C" {
#include <art32/motion.h>
}

TEST(Motion, Update) {
  a32_motion_t motion = a32_motion_make(10, 1);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(1, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(3, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(6, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(10, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(15, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(21, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(28, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(36, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(45, motion.position);
  a32_motion_update(&motion, 100, 2);
  EXPECT_EQ(65, motion.position);
  a32_motion_update(&motion, 100, 2);
  EXPECT_EQ(81, motion.position);
  a32_motion_update(&motion, 100, 2);
  EXPECT_EQ(93, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(98, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(100.25, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(102.25, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(104, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(106.5, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(108, motion.position);
  a32_motion_update(&motion, 100, 1);
  EXPECT_EQ(108.5, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(108.5, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(108.25, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(107.75, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(107, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(106, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(104.75, motion.position);
  a32_motion_update(&motion, 100, 0.5);
  EXPECT_EQ(103.25, motion.position);
}

// Regression: a zero delta divided by zero, which the NaN guard turned into a velocity of
// zero and left an infinite acceleration behind.
TEST(Motion, ZeroDelta) {
  a32_motion_t motion = a32_motion_make(10, 5);
  a32_motion_update(&motion, 100, 0.1f);
  ASSERT_FLOAT_EQ(0.05, motion.position);
  ASSERT_FLOAT_EQ(0.5, motion.velocity);
  ASSERT_FLOAT_EQ(5, motion.acceleration);

  // the update is a no-op and must not discard the state
  a32_motion_update(&motion, 100, 0);
  EXPECT_FLOAT_EQ(0.05, motion.position);
  EXPECT_FLOAT_EQ(0.5, motion.velocity);
  EXPECT_FLOAT_EQ(5, motion.acceleration);

  // the same holds for a negative delta
  a32_motion_update(&motion, 100, -1);
  EXPECT_FLOAT_EQ(0.05, motion.position);
  EXPECT_FLOAT_EQ(0.5, motion.velocity);
  EXPECT_FLOAT_EQ(5, motion.acceleration);

  // and the profile continues afterwards
  a32_motion_update(&motion, 100, 0.1f);
  EXPECT_FLOAT_EQ(0.15, motion.position);
}

// The profile divides by the maximum acceleration, so it must be configured.
TEST(Motion, ZeroAcceleration) {
  a32_motion_t motion = a32_motion_make(10, 0);
  a32_motion_update(&motion, 100, 1);
  EXPECT_FLOAT_EQ(0, motion.position);
  EXPECT_FLOAT_EQ(0, motion.velocity);
  EXPECT_FLOAT_EQ(0, motion.acceleration);
}

// The profile must settle on the target rather than oscillate around it.
TEST(Motion, Settles) {
  a32_motion_t motion = a32_motion_make(10, 1);
  for (int i = 0; i < 1000; i++) {
    a32_motion_update(&motion, 100, 1);
  }
  EXPECT_FLOAT_EQ(100, motion.position);
  EXPECT_FLOAT_EQ(0, motion.velocity);
}
