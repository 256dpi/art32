#include <gtest/gtest.h>

extern "C" {
#include <art32/convert.h>
}

TEST(Strconv, ToString) {
  EXPECT_STREQ("10", a32_i2str(10));
  EXPECT_STREQ("10", a32_l2str(10));
  EXPECT_STREQ("8.2", a32_f2str(8.2));
  EXPECT_STREQ("8.2", a32_d2str(8.2));
  EXPECT_STREQ("8.12346", a32_f2str(8.123456789));
  EXPECT_STREQ("8.12346", a32_d2str(8.123456789));
}

TEST(Strconv, FromString) {
  EXPECT_EQ((int32_t)10, a32_str2i("10"));
  EXPECT_EQ((int64_t)10, a32_str2l("10"));
  EXPECT_EQ((float)8.2, a32_str2f("8.2"));
  EXPECT_EQ((double)8.2, a32_str2d("8.2"));
  EXPECT_EQ((float)8.12346, a32_str2f("8.12346"));
  EXPECT_EQ((double)8.12346, a32_str2d("8.12346"));
}
