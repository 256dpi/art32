#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "art32/parser.h"
#include "art32/convert.h"

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
        c->args[i].i = *((int32_t*)(p->source + p->off));

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
        c->args[i].l = *((int64_t*)(p->source + p->off));

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
        c->args[i].f = *((float*)(p->source + p->off));

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
        c->args[i].d = *((double*)(p->source + p->off));

        // increment
        p->off += 8;

        break;
      }
      case 's': {
        // get length
        size_t len = strlen((char*)(p->source + p->off));

        // check length
        if (len >= p->length - p->off) {
          return A32_PARSER_ERR_UNDERFLOW;
        }

        // set value
        c->args[i].s = (char*)(p->source + p->off);

        // increment
        p->off += len + 1;

        break;
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
      code_token = strtok_r((char*)p->source, ";\n", &p->cache);
      p->init = true;
    } else {
      code_token = strtok_r(NULL, ";\n", &p->cache);
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
      if (strcmp(p->defs[i].name, arg_token) == 0) {
        def = &p->defs[i];
      }
    }
    if (def == NULL) {
      return A32_PARSER_ERR_UNKNOWN;
    }

    // set def
    c->def = def;

    // check args
    if (strlen(def->fmt) == 0) {
      return A32_PARSER_ERR_OK;
    }

    /* arg parsing */

    // check token
    for (size_t i = 0; arg_token != NULL; i++) {
      // check overflow
      if (i >= A32_PARSER_MAX_ARGS) {
        return A32_PARSER_ERR_OVERFLOW;
      }

      // get next arg
      arg_token = strbtok_r(NULL, " ", &cache);
      if (arg_token == NULL) {
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
          break;
      }
    }

    return A32_PARSER_ERR_OK;
  }
}

a32_parser_t a32_parser_make_string(const char* source, a32_parser_def_t* defs, size_t num_defs) {
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
  size_t size = 0;

  // check buffer
  if (len < strlen(def->fmt) + 1) {
    return -1;
  }

  // encode name
  strcpy(buf, def->name);
  size += strlen(def->name);

  // encode arguments
  for (size_t i = 0; i < strlen(def->fmt); i++) {
    // check buffer
    if (len < size + 1) {
      return -1;
    }

    // add space
    buf[size] = ' ';
    size++;

    // encode value
    switch (def->fmt[i]) {
      case 'o': {
        // encode octet
        int n = snprintf(buf + size, len - size, "%" PRId8, args[i].o);
        if (n < 0) {
          return -1;
        }
        size += n;

        break;
      }
      case 'i': {
        // encode integer
        int n = snprintf(buf + size, len - size, "%" PRId32, args[i].i);
        if (n < 0) {
          return -1;
        }
        size += n;

        break;
      }
      case 'l': {
        // encode long
        int n = snprintf(buf + size, len - size, "%" PRId64, args[i].l);
        if (n < 0) {
          return -1;
        }
        size += n;

        break;
      }
      case 'f': {
        // encode float
        int n = snprintf(buf + size, len - size, "%g", args[i].f);
        if (n < 0) {
          return -1;
        }
        size += n;

        break;
      }
      case 'd': {
        // encode double
        int n = snprintf(buf + size, len - size, "%g", args[i].d);
        if (n < 0) {
          return -1;
        }
        size += n;

        break;
      }
      case 's': {
        // encode string
        int n;
        if (args[i].s == NULL || strlen(args[i].s) == 0) {
          n = snprintf(buf + size, len - size, "``");
        } else if (strchr(args[i].s, ' ') != NULL) {
          n = snprintf(buf + size, len - size, "`%s`", args[i].s);
        } else {
          n = snprintf(buf + size, len - size, "%s", args[i].s);
        }
        if (n < 0) {
          return -1;
        }
        size += n;

        break;
      }
    }
  }

  return (int)size;
}
