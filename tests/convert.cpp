#include <gtest/gtest.h>

extern "C" {
#include <art32/convert.h>
}

TEST(Strconv, Trim) {
  char s1[] = "  foo  ";
  EXPECT_STREQ("foo", a32_trim(s1));

  char s2[] = "foo";
  EXPECT_STREQ("foo", a32_trim(s2));

  char s3[] = "";
  EXPECT_STREQ("", a32_trim(s3));

  // Regression: an all-whitespace string underflowed the buffer.
  char s4[] = "   ";
  EXPECT_STREQ("", a32_trim(s4));

  char s5[] = "\t foo \r\n";
  EXPECT_STREQ("foo", a32_trim(s5));
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

// Regression: the parsed number was narrowed to the return type, so out of range input
// wrapped around instead of saturating.
TEST(Strconv, FromStringSaturates) {
  EXPECT_EQ(INT32_MAX, a32_str2i("3000000000"));
  EXPECT_EQ(INT32_MIN, a32_str2i("-3000000000"));
  EXPECT_EQ(INT32_MAX, a32_str2i("99999999999999999999"));
  EXPECT_EQ(INT32_MIN, a32_str2i("-99999999999999999999"));

  // values within the range are unaffected
  EXPECT_EQ(INT32_MAX, a32_str2i("2147483647"));
  EXPECT_EQ(INT32_MIN, a32_str2i("-2147483648"));

  // unparsable input still yields zero
  EXPECT_EQ(0, a32_str2i("abc"));
}
