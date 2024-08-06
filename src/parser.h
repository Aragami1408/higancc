#ifndef HIGANCC_PARSER_H
#define HIGANCC_PARSER_H

#include "token.h"
#include "common.h"
#include "ast.h"

typedef struct {
  Token *tokens;
  usize current;
} Parser;

Parser *Parser_init(Token *tokens);
AST *Parser_parse(Parser *parser);
void Parser_destroy(Parser *parser);

#endif
