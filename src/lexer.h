#ifndef HIGANCC_LEXER_H
#define HIGANCC_LEXER_H

#include "token.h"
#include "common.h"
#include "memory.h"

typedef struct {
  const char *start;
  const char *current;
  int line;
  ArenaAllocator *allocator; 
} Lexer;

Lexer *Lexer_init(ArenaAllocator *a, const char *source);
ArrayList(Token) *Lexer_scanTokens(Lexer *lexer, usize *len);
Token Lexer_scanToken(Lexer *lexer);

#endif
