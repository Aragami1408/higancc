#ifndef HIGANCC_LEXER_H
#define HIGANCC_LEXER_H

#include "token.h"

typedef struct {
  const char *start;
  const char *current;
  int line;
} Lexer;

Lexer *Lexer_init(const char *source);
void Lexer_scanTokens(Lexer *lexer);
Token Lexer_scanToken(Lexer *lexer);

void Lexer_free(Lexer *lexer);


#endif
