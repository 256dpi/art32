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
