#ifndef A32_PARSER_H
#define A32_PARSER_H

#include <stdbool.h>

/**
 * A parser definition.
 */
typedef struct {
  int id;            // 1
  const char* name;  // "FOO"
  const char* fmt;   // "ifs"
} a32_def_t;

/**
 * The parser object.
 */
typedef struct {
  char* source;
  a32_def_t* defs;
  int num_defs;
  // private:
  bool init;
  char* token;
  char* cache;
} a32_parser_t;

/**
 * A single parsed argument.
 */
typedef struct {
  char type;

  union {
    int i;
    long l;
    float f;
    double d;
    char* s;
  };
} a32_arg_t;

/**
 * A single parsed code.
 */
typedef struct {
  size_t off;
  a32_def_t* def;
  a32_arg_t args[8];
} a32_code_t;

#define A32_PARSER_MAKE(name, _source, _defs) \
  a32_parser_t name = {.source = _source, .defs = _defs, .num_defs = sizeof(_defs) / sizeof(a32_def_t)}

/**
 * Will advance the parser to the next code and decode it.
 *
 * @param p The parser.
 * @param c The code.
 * @return Whether a code is available.
 */
bool a32_parser_next(a32_parser_t* p, a32_code_t* c);

#endif  // A32_PARSER_H
