#ifndef HIGANCC_LEXER_H
#define HIGANCC_LEXER_H

#include "token.h"
#include "common.h"
#include "memory.h"

typedef struct {
  const char *start;
  const char *current;
  int line;
} Lexer;

Lexer Lexer_init(const char *source);
ArrayList(Token) *Lexer_scanTokens(Lexer *lexer, ArenaAllocator *allocator);
Token Lexer_scanToken(Lexer *lexer);

#endif
