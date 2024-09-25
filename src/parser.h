#ifndef HIGANCC_PARSER_H
#define HIGANCC_PARSER_H

#include "token.h"
#include "common.h"
#include "ast.h"
#include "memory.h"
#include "arraylist.h"


typedef struct {
  ArrayList(Token) *tokens;
  usize current;
  ArenaAllocator *allocator;
} Parser;

Parser *Parser_init(ArenaAllocator *a, ArrayList(Token) *tokens);
AST *Parser_parse(Parser *parser);

#endif
