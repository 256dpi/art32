#include <string.h>

#include "art32/parser.h"
#include "art32/convert.h"

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
  for (int i = 0; i < p->num_defs; i++) {
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
    switch (def->fmt[i]) {
      case 'o': {
        // check length
        if (p->length - p->off < 1) {
          return A32_PARSER_ERR_OVERFLOW;
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
          return A32_PARSER_ERR_OVERFLOW;
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
          return A32_PARSER_ERR_OVERFLOW;
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
          return A32_PARSER_ERR_OVERFLOW;
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
          return A32_PARSER_ERR_OVERFLOW;
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
          return A32_PARSER_ERR_OVERFLOW;
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
    char* arg_token = strtok_r(code_token, " ", &cache);

    // set offset
    c->off = code_token - (char*)p->source;

    // find def
    a32_parser_def_t* def = NULL;
    for (int i = 0; i < p->num_defs; i++) {
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

    // get first arg
    arg_token = strtok_r(NULL, " ", &cache);

    // check token
    for (size_t i = 0; arg_token != NULL; i++) {
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

      // get next arg
      arg_token = strtok_r(NULL, " ", &cache);
    }

    return A32_PARSER_ERR_OK;
  }
}

a32_parser_err_t a32_parser_next(a32_parser_t* p, a32_parser_code_t* c) {
  if (p->binary) {
    return a32_parser_next_binary(p, c);
  } else {
    return a32_parser_next_string(p, c);
  }
}
