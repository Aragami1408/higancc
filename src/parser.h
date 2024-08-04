#ifndef HIGANCC_PARSER_H
#define HIGANCC_PARSER_H

#include "token.h"
#include "common.h"

typedef struct {
  Token *tokens;
  usize current;
} Parser;

Parser *Parser_init(Token *tokens);

#endif
