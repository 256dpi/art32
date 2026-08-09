#ifndef A32_PARSER_H
#define A32_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A32_PARSER_MAX_ARGS 16

/**
 * A parser definition.
 */
typedef struct {
  int num;           // 1
  const char* name;  // "FOO"
  const char* fmt;   // "ifs"
} a32_parser_def_t;

/**
 * The parser object.
 */
typedef struct {
  uint8_t* source;
  a32_parser_def_t* defs;
  size_t num_defs;
  bool binary;
  size_t length;  // binary
  // private:
  bool init;
  char* cache;
  size_t off;  // binary
} a32_parser_t;

/**
 * A single parsed argument.
 */
typedef union {
  uint8_t o;
  int32_t i;
  int64_t l;
  float f;
  double d;
  char* s;
} a32_parser_arg_t;

typedef enum {
  A32_PARSER_ERR_OK = 1,
  A32_PARSER_ERR_DONE = 0,
  A32_PARSER_ERR_UNKNOWN = -1,
  A32_PARSER_ERR_UNDERFLOW = -2,
  A32_PARSER_ERR_OVERFLOW = -3,
  A32_PARSER_ERR_INVALID = -4,  // the definition uses an unknown format character
} a32_parser_err_t;

/**
 * A single parsed code.
 */
typedef struct {
  size_t off;
  a32_parser_def_t* def;
  a32_parser_arg_t args[A32_PARSER_MAX_ARGS];
} a32_parser_code_t;

/**
 * Constructs a parser that reads codes from a string.
 *
 * Codes are separated by semicolons and newlines, arguments by spaces, and a code that
 * begins with a hash is a comment. An argument may be wrapped in ticks to contain spaces
 * and separators, but never a tick itself. Definitions with a NULL name are skipped.
 *
 * Note: The parser will modify the passed string and parsed string arguments point into
 * it, therefore it must stay valid and unmodified for the lifetime of the parser.
 *
 * @param source The source string.
 * @param defs The definitions.
 * @param num_defs The number of definitions.
 * @return A parser object.
 */
a32_parser_t a32_parser_make_string(char* source, a32_parser_def_t* defs, size_t num_defs);

/**
 * Constructs a parser that reads codes from a buffer.
 *
 * Note: Parsed string arguments point into the buffer, therefore it must stay valid and
 * unmodified for the lifetime of the parser.
 *
 * @param source The source buffer.
 * @param length The length of the buffer.
 * @param defs The definitions.
 * @param num_defs The number of definitions.
 * @return A parser object.
 */
a32_parser_t a32_parser_make_binary(uint8_t* source, size_t length, a32_parser_def_t* defs, size_t num_defs);

/**
 * Macro to initialize a parser with a string.
 */
#define A32_PARSER_MAKE_STRING(name, _source, _defs) \
  a32_parser_t name = a32_parser_make_string(_source, _defs, sizeof(_defs) / sizeof(a32_parser_def_t))

/**
 * Macro to initialize a parser with a buffer.
 */
#define A32_PARSER_MAKE_BINARY(name, _source, _length, _defs) \
  a32_parser_t name = a32_parser_make_binary(_source, _length, _defs, sizeof(_defs) / sizeof(a32_parser_def_t))

/**
 * Will advance the parser to the next code and decode it.
 *
 * Note: The parser must not be used after it returned an error, as it may have consumed
 * part of the code that failed.
 *
 * @param p The parser.
 * @param c The code.
 * @return Whether a code is available.
 */
a32_parser_err_t a32_parser_next(a32_parser_t* p, a32_parser_code_t* c);

/**
 * Will encode the specified code into a string.
 *
 * String arguments that contain a separator are quoted with ticks, so that the result can
 * be read back by a string parser. A string argument that contains a tick itself cannot
 * be represented and is rejected.
 *
 * @param def The definition.
 * @param args The arguments.
 * @param buf The buffer.
 * @param len The length of the buffer.
 * @return The number of bytes encoded or -1 if the buffer is too small, a string argument
 *         contains a tick, or the definition is invalid (unknown format character or more
 *         than A32_PARSER_MAX_ARGS arguments).
 */
int a32_parser_encode_string(a32_parser_def_t* def, a32_parser_arg_t* args, char* buf, size_t len);

#endif  // A32_PARSER_H
