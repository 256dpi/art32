#ifndef A32_PARSER_H
#define A32_PARSER_H

#include <stdint.h>
#include <stdbool.h>

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
  int num_defs;
  bool binary;
  size_t length;  // binary
  // private:
  bool init;
  char* token;
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
  A32_PARSER_ERR_OVERFLOW = -2,
} a32_parser_err_t;

/**
 * A single parsed code.
 */
typedef struct {
  size_t off;
  a32_parser_def_t* def;
  a32_parser_arg_t args[8];
} a32_parser_code_t;

#define A32_PARSER_MAKE(name, _source, _defs) \
  a32_parser_t name = {.source = (uint8_t*)_source, .defs = _defs, .num_defs = sizeof(_defs) / sizeof(a32_parser_def_t)}

#define A32_PARSER_MAKE_BINARY(name, _source, _length, _defs)                \
  a32_parser_t name = {.source = _source,                                    \
                       .defs = _defs,                                        \
                       .num_defs = sizeof(_defs) / sizeof(a32_parser_def_t), \
                       .binary = true,                                       \
                       .length = _length}

/**
 * Will advance the parser to the next code and decode it.
 *
 * @param p The parser.
 * @param c The code.
 * @return Whether a code is available.
 */
a32_parser_err_t a32_parser_next(a32_parser_t* p, a32_parser_code_t* c);

#endif  // A32_PARSER_H
