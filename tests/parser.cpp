#include <gtest/gtest.h>

extern "C" {
#include <art32/parser.h>
}

static a32_parser_def_t defs[] = {
    {.num = 0, .name = "FOO", .fmt = "ifs"},
    {.num = 1, .name = "BAR", .fmt = ""},
    {.num = 2, .name = "BAZ", .fmt = "old"},
    {.num = 3, .name = "QUZ", .fmt = "ss"},
};

static a32_parser_err_t first_str_err(const char* source, size_t* offset) {
  A32_PARSER_MAKE_STRING(p, strdup(source), defs);
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
  char* source = strdup("FOO 1 2.3 foo; FOO\n# hmm; BAR\nBAZ 7 1 1.4; QUZ quz\nQUZ `foo bar` baz");
  A32_PARSER_MAKE_STRING(p, source, defs);
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
    ASSERT_EQ(A32_PARSER_ERR_OVERFLOW, first_bin_err(s1, i, nullptr));
  }
  ASSERT_EQ(A32_PARSER_ERR_DONE, first_bin_err(s1, sizeof(s1), nullptr));

  uint8_t s2[] = {5};
  ASSERT_EQ(A32_PARSER_ERR_UNKNOWN, first_bin_err(s2, sizeof(s2), &offset));
  ASSERT_EQ(0, offset);

  uint8_t s3[] = {1, 5};
  ASSERT_EQ(A32_PARSER_ERR_UNKNOWN, first_bin_err(s3, sizeof(s3), &offset));
  ASSERT_EQ(1, offset);
}
