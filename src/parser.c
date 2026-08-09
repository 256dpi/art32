#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "art32/parser.h"
#include "art32/convert.h"

// Splits the source into codes on semicolons and newlines. Unlike strtok_r it keeps track
// of backtick quoting, so that a quoted argument may contain separator characters.
static char* strctok_r(char* str, char** cache) {
  // handle null input
  if (str == NULL) {
    str = *cache;
  }

  // skip leading separator characters
  while (*str == ';' || *str == '\n') {
    str++;
  }

  // handle end of string
  if (*str == '\0') {
    *cache = str;
    return NULL;
  }

  // check for a comment, which runs to the end of the line and is not subject to quoting
  char* peek = str;
  while (*peek == ' ' || *peek == '\t' || *peek == '\r') {
    peek++;
  }
  bool comment = *peek == '#';

  // prepare state
  char* token = str;
  bool ticked = false;

  // scan string
  while (*str != '\0') {
    // toggle quoting on tick characters, otherwise return the token on a separator
    // character that is not quoted
    if (!comment && *str == '`') {
      ticked = !ticked;
    } else if (!ticked && (*str == ';' || *str == '\n')) {
      *str = '\0';
      *cache = str + 1;
      return token;
    }

    // otherwise, advance
    str++;
  }

  // save state
  *cache = str;

  return token;
}

static char* strbtok_r(char* str, const char* delim, char** cache) {
  // handle null input
  if (str == NULL) {
    str = *cache;
  }

  // skip leading delimiter characters
  while (*str != '\0' && strchr(delim, *str) != NULL) {
    str++;
  }

  // handle end of string
  if (*str == '\0') {
    *cache = str;
    return NULL;
  }

  // prepare state
  char* token = str;
  bool ticked = *str == '`';
  if (ticked) {
    str++;
    token++;
  }

  // scan string
  while (*str != '\0') {
    // return token on closing tick character
    if (ticked && *str == '`') {
      *str = '\0';
      *cache = str + 1;
      return token;
    }

    // return on delimiter character
    if (strchr(delim, *str) != NULL && !ticked) {
      *str = '\0';
      *cache = str + 1;
      return token;
    }

    // otherwise, advance
    str++;
  }

  // save state
  *cache = str;

  return token;
}

static a32_parser_err_t a32_parser_next_binary(a32_parser_t* p, a32_parser_code_t* c) {
  // clear code
  memset(c, 0, sizeof(a32_parser_code_t));

  // check offset
  if (p->off == p->length) {
    return A32_PARSER_ERR_DONE;
  }

  // set offset
  c->off = p->off;

  // get num
  uint8_t num = p->source[p->off];

  // find def
  a32_parser_def_t* def = NULL;
  for (size_t i = 0; i < p->num_defs; i++) {
    if (num == p->defs[i].num) {
      def = &p->defs[i];
    }
  }
  if (def == NULL) {
    return A32_PARSER_ERR_UNKNOWN;
  }

  // set def
  c->def = def;

  // increment
  p->off++;

  // parse arguments
  for (int i = 0; def->fmt[i] != 0; i++) {
    // check overflow
    if (i >= A32_PARSER_MAX_ARGS) {
      return A32_PARSER_ERR_OVERFLOW;
    }

    // handle argument
    switch (def->fmt[i]) {
      case 'o': {
        // check length
        if (p->length - p->off < 1) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // set value
        c->args[i].o = p->source[p->off];

        // increment
        p->off += 1;

        break;
      }
      case 'i': {
        // check length
        if (p->length - p->off < 4) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // set value
        memcpy(&c->args[i].i, p->source + p->off, sizeof(int32_t));

        // increment
        p->off += 4;

        break;
      }
      case 'l': {
        // check length
        if (p->length - p->off < 8) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // set value
        memcpy(&c->args[i].l, p->source + p->off, sizeof(int64_t));

        // increment
        p->off += 8;

        break;
      }
      case 'f': {
        // check length
        if (p->length - p->off < 4) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // set value
        memcpy(&c->args[i].f, p->source + p->off, sizeof(float));

        // increment
        p->off += 4;

        break;
      }
      case 'd': {
        // check length
        if (p->length - p->off < 8) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // set value
        memcpy(&c->args[i].d, p->source + p->off, sizeof(double));

        // increment
        p->off += 8;

        break;
      }
      case 's': {
        // find terminator within the remaining bytes
        const uint8_t* end = memchr(p->source + p->off, 0, p->length - p->off);
        if (end == NULL) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // get length
        size_t len = (size_t)(end - (p->source + p->off));

        // set value
        c->args[i].s = (char*)(p->source + p->off);

        // increment
        p->off += len + 1;

        break;
      }
      default: {
        // unknown format character
        return A32_PARSER_ERR_INVALID;
      }
    }
  }

  return A32_PARSER_ERR_OK;
}

static a32_parser_err_t a32_parser_next_string(a32_parser_t* p, a32_parser_code_t* c) {
  // clear code
  memset(c, 0, sizeof(a32_parser_code_t));

  // prepare code token
  char* code_token = NULL;

  while (true) {
    // get first or next code token
    if (!p->init) {
      code_token = strctok_r((char*)p->source, &p->cache);
      p->init = true;
    } else {
      code_token = strctok_r(NULL, &p->cache);
    }

    // handle end
    if (code_token == NULL) {
      return A32_PARSER_ERR_DONE;
    }

    // trim token
    code_token = a32_trim(code_token);

    // skip whitespace
    if (strlen(code_token) == 0) {
      continue;
    }

    // skip comments
    if (code_token[0] == '#') {
      continue;
    }

    /* code parsing */

    // prepare cache
    char* cache;

    // get arg token
    char* arg_token = strbtok_r(code_token, " ", &cache);

    // set offset
    c->off = code_token - (char*)p->source;

    // find def
    a32_parser_def_t* def = NULL;
    for (size_t i = 0; i < p->num_defs; i++) {
      if (p->defs[i].name != NULL && strcmp(p->defs[i].name, arg_token) == 0) {
        def = &p->defs[i];
      }
    }
    if (def == NULL) {
      return A32_PARSER_ERR_UNKNOWN;
    }

    // set def
    c->def = def;

    // get number of arguments
    size_t num_args = strlen(def->fmt);

    // check args
    if (num_args == 0) {
      return A32_PARSER_ERR_OK;
    }

    /* arg parsing */

    // check token
    size_t i = 0;
    for (; arg_token != NULL; i++) {
      // check overflow
      if (i >= A32_PARSER_MAX_ARGS) {
        return A32_PARSER_ERR_OVERFLOW;
      }

      // get next arg
      arg_token = strbtok_r(NULL, " ", &cache);
      if (arg_token == NULL) {
        break;
      }

      // stop if the format has been exhausted
      if (i >= num_args) {
        break;
      }

      // handle argument
      switch (def->fmt[i]) {
        case 'o':
          c->args[i].o = (uint8_t)a32_str2i(arg_token);
          break;
        case 'i':
          c->args[i].i = a32_str2i(arg_token);
          break;
        case 'l':
          c->args[i].l = a32_str2l(arg_token);
          break;
        case 'f':
          c->args[i].f = a32_str2f(arg_token);
          break;
        case 'd':
          c->args[i].d = a32_str2d(arg_token);
          break;
        case 's':
          c->args[i].s = arg_token;
          break;
        default:
          // unknown format character
          return A32_PARSER_ERR_INVALID;
      }
    }

    // check underflow
    if (i < num_args) {
      return A32_PARSER_ERR_UNDERFLOW;
    }

    return A32_PARSER_ERR_OK;
  }
}

a32_parser_t a32_parser_make_string(char* source, a32_parser_def_t* defs, size_t num_defs) {
  a32_parser_t p = {
      .source = (uint8_t*)source,
      .defs = defs,
      .num_defs = num_defs,
  };
  return p;
}

a32_parser_t a32_parser_make_binary(uint8_t* source, size_t length, a32_parser_def_t* defs, size_t num_defs) {
  a32_parser_t p = {
      .source = source,
      .defs = defs,
      .num_defs = num_defs,
      .binary = true,
      .length = length,
  };
  return p;
}

a32_parser_err_t a32_parser_next(a32_parser_t* p, a32_parser_code_t* c) {
  if (p->binary) {
    return a32_parser_next_binary(p, c);
  } else {
    return a32_parser_next_string(p, c);
  }
}

int a32_parser_encode_string(a32_parser_def_t* def, a32_parser_arg_t* args, char* buf, size_t len) {
  // prepare size
  size_t size = strlen(def->name);

  // check buffer
  if (len < size + 1) {
    return -1;
  }

  // encode name
  strcpy(buf, def->name);

  // encode arguments
  size_t num_args = strlen(def->fmt);
  for (size_t i = 0; i < num_args; i++) {
    // check overflow
    if (i >= A32_PARSER_MAX_ARGS) {
      return -1;
    }

    // check buffer (space, at least one character and the terminator)
    if (len < size + 2) {
      return -1;
    }

    // add space
    buf[size] = ' ';
    size++;

    // encode value
    int n;
    switch (def->fmt[i]) {
      case 'o':
        // encode octet
        n = snprintf(buf + size, len - size, "%" PRIu8, args[i].o);
        break;
      case 'i':
        // encode integer
        n = snprintf(buf + size, len - size, "%" PRId32, args[i].i);
        break;
      case 'l':
        // encode long
        n = snprintf(buf + size, len - size, "%" PRId64, args[i].l);
        break;
      case 'f':
        // encode float
        n = snprintf(buf + size, len - size, "%g", args[i].f);
        break;
      case 'd':
        // encode double
        n = snprintf(buf + size, len - size, "%g", args[i].d);
        break;
      case 's':
        // encode string
        if (args[i].s == NULL || strlen(args[i].s) == 0) {
          n = snprintf(buf + size, len - size, "``");
        } else if (strchr(args[i].s, '`') != NULL) {
          // a tick cannot be represented, as it is the quoting character itself
          return -1;
        } else if (strpbrk(args[i].s, " \t\n\v\f\r;") != NULL) {
          // quote strings that contain an argument or code separator
          n = snprintf(buf + size, len - size, "`%s`", args[i].s);
        } else {
          n = snprintf(buf + size, len - size, "%s", args[i].s);
        }
        break;
      default:
        // unknown format character
        return -1;
    }

    // check encoding error and truncation
    if (n < 0 || (size_t)n >= len - size) {
      return -1;
    }

    // increment
    size += n;
  }

  return (int)size;
}
