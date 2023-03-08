#include <string.h>

#include "art32/parser.h"
#include "art32/strconv.h"

bool a32_parser_next(a32_parser_t* p, a32_parser_code_t* c) {
  // clear code
  memset(c, 0, sizeof(a32_parser_code_t));

  // parse binary
  if (p->binary) {
    // check offset
    if (p->off >= p->length) {
      return false;
    }

    // get num
    uint8_t num = p->source[p->off];

    // find def
    a32_parser_def_t* def = NULL;
    for (size_t i = 0; i < p->num_defs; i++) {
      if (num == p->defs[i].num) {
        def = &p->defs[i];
      }
    }

    // stop at unknown code
    if (def == NULL) {
      return false;
    }

    // set offset
    c->off = p->off;

    // set def
    c->def = def;

    // increment
    p->off++;

    // parse arguments
    for (int i = 0; def->fmt[i] != 0; i++) {
      switch (def->fmt[i]) {
        case 'o': {
          // check size
          if (p->length - p->off < 1) {
            return false;
          }

          // set value
          c->args[i].o = p->source[p->off];

          // increment
          p->off += 1;

          break;
        }
        case 'i': {
          // check size
          if (p->length - p->off < 4) {
            return false;
          }

          // set value
          c->args[i].i = *((int32_t*)(p->source + p->off));

          // increment
          p->off += 4;

          break;
        }
        case 'l': {
          // check size
          if (p->length - p->off < 8) {
            return false;
          }

          // set value
          c->args[i].l = *((int64_t*)(p->source + p->off));

          // increment
          p->off += 8;

          break;
        }
        case 'f': {
          // check size
          if (p->length - p->off < 4) {
            return false;
          }

          // set value
          c->args[i].f = *((float*)(p->source + p->off));

          // increment
          p->off += 4;

          break;
        }
        case 'd': {
          // check size
          if (p->length - p->off < 8) {
            return false;
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

          // set value
          c->args[i].s = (char*)(p->source + p->off);

          // increment
          p->off += len + 1;

          break;
        }
      }
    }

    return true;
  }

  // prepare code token
  char* code_token = NULL;

  // advance parser
  while (true) {
    // get first or next token
    if (!p->init) {
      code_token = strtok_r((char*)p->source, ";\n", &p->cache);
      p->init = true;
    } else {
      code_token = strtok_r(NULL, ";\n", &p->cache);
    }

    // check end
    if (code_token == NULL) {
      return false;
    }

    // trim token
    code_token = a32_trim(code_token);

    // skip invalid
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

    // find def
    a32_parser_def_t* def = NULL;
    for (size_t i = 0; i < p->num_defs; i++) {
      if (strcmp(p->defs[i].name, arg_token) == 0) {
        def = &p->defs[i];
      }
    }

    // skip unknown code
    if (def == NULL) {
      continue;
    }

    // set offset
    c->off = code_token - (char*)p->source;

    // set def
    c->def = def;

    // check args
    if (strlen(def->fmt) == 0) {
      return true;
    }

    /* arg parsing */

    // get first arg
    arg_token = strtok_r(NULL, " ", &cache);

    // check token
    size_t i = 0;
    while (arg_token != NULL) {
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

      // advance
      i++;
      arg_token = strtok_r(NULL, " ", &cache);
    }

    return true;
  }
}
