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
} Parser;

Parser Parser_init(ArrayList(Token) *tokens);
ASTProgram *Parser_parse(Parser *parser, ArenaAllocator *allocator);

#endif
