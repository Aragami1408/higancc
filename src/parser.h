#ifndef HIGANCC_PARSER_H
#define HIGANCC_PARSER_H

#include "token.h"
#include "common.h"
#include "ast.h"
#include "memory.h"


typedef struct {
  Token *tokens;
  usize current;
  ArenaAllocator *allocator;
} Parser;

Parser *Parser_init(ArenaAllocator *a, Token *tokens);
AST *Parser_parse(Parser *parser);

#endif
