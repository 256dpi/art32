#include <gtest/gtest.h>

extern "C" {
#include <art32/motion.h>
}

TEST(Motion, Update) {
  a32_motion_t motion = {.max_velocity = 10, .max_acceleration = 1};
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
}
