#include <string.h>

#include "art32/parser.h"
#include "art32/strconv.h"

bool a32_parser_next(a32_parser_t* p, a32_code_t* c) {
  // clear code
  memset(c, 0, sizeof(a32_code_t));

  // prepare code token
  char* code_token = NULL;

  // advance parser
  while (true) {
    // get first or next token
    if (!p->init) {
      code_token = strtok_r(p->source, ";\n", &p->cache);
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
    a32_def_t* def = NULL;
    for (size_t i = 0; i < p->num_defs; i++) {
      if (strcmp(p->defs[i].name, arg_token) == 0) {
        def = &p->defs[i];
      }
    }

    // skip unknown code
    if (def == NULL) {
      continue;
    }

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
