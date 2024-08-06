#ifndef HIGANCC_TOKEN_H
#define HIGANCC_TOKEN_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  // Single-character tokens.
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR, TOKEN_PERCENT,

  // One or two character tokens.
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,

  // Literals.
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_INT,
  TOKEN_FLOAT, TOKEN_CHAR,

  // Keywords.
  TOKEN_KW_INT, TOKEN_KW_VOID, TOKEN_KW_RETURN,
  TOKEN_KW_IF, TOKEN_KW_FLOAT, TOKEN_KW_CHAR,

  TOKEN_ERROR, TOKEN_EOF

} TokenType;


typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
} Token;

void print_token(const Token *token);
#endif
