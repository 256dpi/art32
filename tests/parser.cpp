#include <gtest/gtest.h>

#include <string>

extern "C" {
#include <art32/parser.h>
}

static a32_parser_def_t defs[] = {
    {.num = 0, .name = "FOO", .fmt = "ifs"},
    {.num = 1, .name = "BAR", .fmt = ""},
    {.num = 2, .name = "BAZ", .fmt = "old"},
    {.num = 3, .name = "QUZ", .fmt = "ss"},
    {.num = 4, .name = "XXX", .fmt = "ooooooooooooooooo"},
};

static a32_parser_err_t first_str_err(const char* source, size_t* offset) {
  // the parser modifies the source in place, so it needs a mutable copy that outlives it
  std::string buf(source);
  A32_PARSER_MAKE_STRING(p, buf.data(), defs);
  while (true) {
    a32_parser_code_t code;
    a32_parser_err_t err = a32_parser_next(&p, &code);
    if (err != A32_PARSER_ERR_OK) {
      if (offset != nullptr) {
        *offset = code.off;
      }
      return err;
    }
  }
}

static a32_parser_err_t first_bin_err(uint8_t* source, size_t len, size_t* offset) {
  A32_PARSER_MAKE_BINARY(p, source, len, defs);
  while (true) {
    a32_parser_code_t code;
    a32_parser_err_t err = a32_parser_next(&p, &code);
    if (err != A32_PARSER_ERR_OK) {
      if (offset != nullptr) {
        *offset = code.off;
      }
      return err;
    }
  }
}

TEST(Parser, String) {
  a32_parser_code_t code;
  std::string source("FOO 1 2.3 foo; FOO\n# hmm; BAR\nBAZ 7 1 1.4; QUZ quz\nQUZ `foo bar` baz");
  A32_PARSER_MAKE_STRING(p, source.data(), defs);
  int i = 0;
  for (; a32_parser_next(&p, &code) == A32_PARSER_ERR_OK; i++) {
    switch (i) {
      case 0:
        ASSERT_EQ(code.off, 0);
        ASSERT_EQ(code.def, &defs[0]);
        ASSERT_EQ(code.args[0].i, 1);
        ASSERT_EQ(code.args[1].f, (float)2.3);
        ASSERT_STREQ(code.args[2].s, "foo");
        break;
      case 1:
        ASSERT_EQ(code.off, 15);
        ASSERT_EQ(code.def, &defs[0]);
        break;
      case 2:
        ASSERT_EQ(code.off, 26);
        ASSERT_EQ(code.def, &defs[1]);
        break;
      case 3:
        ASSERT_EQ(code.off, 30);
        ASSERT_EQ(code.def, &defs[2]);
        ASSERT_EQ(code.args[0].o, 7);
        ASSERT_EQ(code.args[1].l, 1);
        ASSERT_EQ(code.args[2].d, 1.4);
        break;
      case 4:
        ASSERT_EQ(code.off, 43);
        ASSERT_EQ(code.def, &defs[3]);
        ASSERT_STREQ(code.args[0].s, "quz");
        ASSERT_TRUE(code.args[1].s == nullptr);
        break;
      case 5:
        ASSERT_EQ(code.off, 51);
        ASSERT_EQ(code.def, &defs[3]);
        ASSERT_STREQ(code.args[0].s, "foo bar");
        ASSERT_STREQ(code.args[1].s, "baz");
        break;
      default:
        ASSERT_FALSE(true);
    }
  }
  ASSERT_EQ(i, 6);
}

TEST(Parser, StringFormat) {
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err("FOO", nullptr));
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err(";FOO;", nullptr));
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err(";;FOO;;", nullptr));
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err("\n\nFOO\n\n", nullptr));
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err("\n;FOO\n;", nullptr));
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err(";\nFOO;\n", nullptr));
}

TEST(Parser, StringErr) {
  size_t offset = 0;

  ASSERT_EQ(A32_PARSER_ERR_DONE, first_str_err("FOO", &offset));
  ASSERT_EQ(0, offset);

  ASSERT_EQ(A32_PARSER_ERR_UNKNOWN, first_str_err("QUX", &offset));
  ASSERT_EQ(0, offset);

  ASSERT_EQ(A32_PARSER_ERR_UNKNOWN, first_str_err("FOO; QUX", &offset));
  ASSERT_EQ(5, offset);

  ASSERT_EQ(A32_PARSER_ERR_OVERFLOW, first_str_err("XXX 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16", &offset));
  ASSERT_EQ(0, offset);
}

TEST(Parser, Binary) {
  uint8_t source[] = {
      0x00,                                            // code
      0x01, 0x00, 0x00, 0x00,                          // int
      0x33, 0x33, 0x13, 0x40,                          // float
      'f',  'o',  'o',  0x0,                           // string
      0x01,                                            // code
      0x02,                                            // code
      0x07,                                            // octet
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // long
      0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xf6, 0x3f,  // double
      0x03,                                            // code
      'q',  'u',  'z',  0x0,                           // string
  };

  a32_parser_code_t code;
  A32_PARSER_MAKE_BINARY(p, source, sizeof(source), defs);
  for (int i = 0; a32_parser_next(&p, &code) == A32_PARSER_ERR_OK; i++) {
    switch (i) {
      case 0:
        ASSERT_EQ(code.off, 0);
        ASSERT_EQ(code.def, &defs[0]);
        ASSERT_EQ(code.args[0].i, 1);
        ASSERT_EQ(code.args[1].f, (float)2.3);
        ASSERT_STREQ(code.args[2].s, "foo");
        break;
      case 1:
        ASSERT_EQ(code.off, 13);
        ASSERT_EQ(code.def, &defs[1]);
        break;
      case 2:
        ASSERT_EQ(code.off, 14);
        ASSERT_EQ(code.def, &defs[2]);
        ASSERT_EQ(code.args[0].o, 7);
        ASSERT_EQ(code.args[1].l, 1);
        ASSERT_EQ(code.args[2].d, 1.4);
        break;
      case 3:
        ASSERT_EQ(code.off, 32);
        ASSERT_EQ(code.def, &defs[3]);
        ASSERT_STREQ(code.args[0].s, "quz");
        ASSERT_TRUE(code.args[1].s == nullptr);
        break;
      default:
        ASSERT_FALSE(true);
    }
  }
}

TEST(Parser, BinaryErr) {
  size_t offset = 0;

  uint8_t s1[] = {
      0x00,                    // code
      0x01, 0x00, 0x00, 0x00,  // int
      0x33, 0x33, 0x13, 0x40,  // float
      'f',  'o',  'o',  0x0,   // string
  };
  for (size_t i = 1; i < sizeof(s1); i++) {
    ASSERT_EQ(A32_PARSER_ERR_UNDERFLOW, first_bin_err(s1, i, nullptr));
  }
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_bin_err(s1, sizeof(s1), nullptr));

  uint8_t s2[] = {5};
  ASSERT_EQ(A32_PARSER_ERR_UNKNOWN, first_bin_err(s2, sizeof(s2), &offset));
  ASSERT_EQ(0, offset);

  uint8_t s3[] = {1, 5};
  ASSERT_EQ(A32_PARSER_ERR_UNKNOWN, first_bin_err(s3, sizeof(s3), &offset));
  ASSERT_EQ(1, offset);

  uint8_t s4[] = {4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  ASSERT_EQ(A32_PARSER_ERR_OVERFLOW, first_bin_err(s4, sizeof(s4), &offset));
  ASSERT_EQ(0, offset);
}

TEST(Parser, EncodeString) {
  char buf[64];

  a32_parser_arg_t args[] = {
      {.i = 1},
      {.f = (float)2.3},
      {.s = (char*)"foo"},
  };
  int n = a32_parser_encode_string(&defs[0], args, buf, sizeof(buf));
  ASSERT_STREQ(buf, "FOO 1 2.3 foo");
  ASSERT_EQ(n, 13);

  args[2].s = nullptr;
  n = a32_parser_encode_string(&defs[0], args, buf, sizeof(buf));
  ASSERT_STREQ(buf, "FOO 1 2.3 ``");
  ASSERT_EQ(n, 12);

  args[2].s = (char*)"";
  n = a32_parser_encode_string(&defs[0], args, buf, sizeof(buf));
  ASSERT_STREQ(buf, "FOO 1 2.3 ``");
  ASSERT_EQ(n, 12);

  args[2].s = (char*)"foo bar";
  n = a32_parser_encode_string(&defs[0], args, buf, sizeof(buf));
  ASSERT_STREQ(buf, "FOO 1 2.3 `foo bar`");
  ASSERT_EQ(n, 19);
}

TEST(Parser, EncodeStringOctet) {
  char buf[64];

  // Regression: octets were encoded as signed, so 200 became -56.
  a32_parser_arg_t args[] = {{.o = 200}, {.l = 1}, {.d = 1.4}};
  int n = a32_parser_encode_string(&defs[2], args, buf, sizeof(buf));
  ASSERT_STREQ(buf, "BAZ 200 1 1.4");
  ASSERT_EQ(n, 13);
}

// Regression: the encoder walked the whole format string without bounding the argument
// index by A32_PARSER_MAX_ARGS, reading past the end of the caller's array.
TEST(Parser, EncodeStringTooManyArgs) {
  // this definition declares more arguments than a code can carry
  ASSERT_GT(strlen(defs[4].fmt), (size_t)A32_PARSER_MAX_ARGS);

  a32_parser_arg_t args[A32_PARSER_MAX_ARGS] = {};
  char buf[128];
  ASSERT_EQ(-1, a32_parser_encode_string(&defs[4], args, buf, sizeof(buf)));
}

TEST(Parser, EncodeStringTooSmall) {
  a32_parser_arg_t args[] = {
      {.i = 1234567},
      {.f = (float)2.3},
      {.s = (char*)"foo"},
  };

  // Regression: the name was copied without checking the buffer. The buffer must be
  // measured against the name, not against the format, so a definition with a short
  // format and a name that does not fit has to be rejected.
  char tiny[3];
  ASSERT_EQ(-1, a32_parser_encode_string(&defs[1], nullptr, tiny, sizeof(tiny)));

  // the name plus terminator exactly fits
  char exact[4];
  ASSERT_EQ(3, a32_parser_encode_string(&defs[1], nullptr, exact, sizeof(exact)));
  ASSERT_STREQ(exact, "BAR");

  char small[4];
  ASSERT_EQ(-1, a32_parser_encode_string(&defs[0], args, small, sizeof(small)));

  // Regression: truncation was not detected and a size beyond the buffer was returned.
  for (size_t i = 1; i < 22; i++) {
    char buf[22] = {0};
    int n = a32_parser_encode_string(&defs[0], args, buf, i);
    ASSERT_TRUE(n == -1 || (size_t)n < i) << "at length " << i << " got " << n;
  }

  // the full encoding still fits
  char buf[22];
  ASSERT_EQ(19, a32_parser_encode_string(&defs[0], args, buf, sizeof(buf)));
  ASSERT_STREQ(buf, "FOO 1234567 2.3 foo");
}

// Regression: surplus arguments indexed past the end of the format string.
TEST(Parser, StringSurplusArgs) {
  a32_parser_code_t code;
  std::string source("FOO 1 2.3 foo bar baz qux");
  A32_PARSER_MAKE_STRING(p, source.data(), defs);
  ASSERT_EQ(A32_PARSER_ERR_OK, a32_parser_next(&p, &code));
  ASSERT_EQ(code.def, &defs[0]);
  ASSERT_EQ(code.args[0].i, 1);
  ASSERT_EQ(code.args[1].f, (float)2.3);
  ASSERT_STREQ(code.args[2].s, "foo");
  ASSERT_EQ(A32_PARSER_ERR_DONE, a32_parser_next(&p, &code));
}

// Regression: the binary string argument was scanned with strlen before the bounds
// check, reading past the end of the buffer when no terminator was present.
TEST(Parser, BinaryUnterminatedString) {
  uint8_t* source = (uint8_t*)malloc(2);
  source[0] = 0x03;  // QUZ
  source[1] = 'x';   // no terminator

  a32_parser_code_t code;
  A32_PARSER_MAKE_BINARY(p, source, 2, defs);
  ASSERT_EQ(A32_PARSER_ERR_UNDERFLOW, a32_parser_next(&p, &code));

  free(source);
}

// Regression: multi-byte values were read through misaligned pointer casts.
TEST(Parser, BinaryUnalignedValues) {
  static a32_parser_def_t local[] = {{.num = 0, .name = "FOO", .fmt = "ifld"}};

  uint8_t source[1 + 4 + 4 + 8 + 8];
  source[0] = 0x00;
  int32_t i = -7;
  float f = 2.5f;
  int64_t l = 123456789012345LL;
  double d = 1.4;
  memcpy(source + 1, &i, 4);
  memcpy(source + 5, &f, 4);
  memcpy(source + 9, &l, 8);
  memcpy(source + 17, &d, 8);

  a32_parser_code_t code;
  A32_PARSER_MAKE_BINARY(p, source, sizeof(source), local);
  ASSERT_EQ(A32_PARSER_ERR_OK, a32_parser_next(&p, &code));
  ASSERT_EQ(code.args[0].i, -7);
  ASSERT_EQ(code.args[1].f, 2.5f);
  ASSERT_EQ(code.args[2].l, 123456789012345LL);
  ASSERT_EQ(code.args[3].d, 1.4);
}
