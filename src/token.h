#ifndef HIGANCC_TOKEN_H
#define HIGANCC_TOKEN_H

#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"

#define LIST_OF_TOKEN_TYPES \
  X(TOKEN_LEFT_PAREN) \
  X(TOKEN_RIGHT_PAREN) \
  X(TOKEN_LEFT_BRACE) \
  X(TOKEN_RIGHT_BRACE) \
  X(TOKEN_COMMA) \
  X(TOKEN_DOT) \
  X(TOKEN_MINUS) \
  X(TOKEN_MINUS_MINUS) \
  X(TOKEN_PLUS) \
  X(TOKEN_PLUS_PLUS) \
  X(TOKEN_SEMICOLON) \
  X(TOKEN_SLASH) \
  X(TOKEN_STAR) \
  X(TOKEN_PERCENT) \
  X(TOKEN_TILDE) \
  X(TOKEN_BANG) \
  X(TOKEN_BANG_EQUAL) \
  X(TOKEN_EQUAL) \
  X(TOKEN_EQUAL_EQUAL) \
  X(TOKEN_GREATER) \
  X(TOKEN_GREATER_EQUAL) \
  X(TOKEN_LESS) \
  X(TOKEN_LESS_EQUAL) \
  X(TOKEN_IDENTIFIER) \
  X(TOKEN_STRING) \
  X(TOKEN_INT) \
  X(TOKEN_FLOAT) \
  X(TOKEN_CHAR) \
  X(TOKEN_KW_INT) \
  X(TOKEN_KW_VOID) \
  X(TOKEN_KW_RETURN) \
  X(TOKEN_KW_IF) \
  X(TOKEN_KW_FLOAT) \
  X(TOKEN_KW_CHAR) \
  X(TOKEN_ERROR) \
  X(TOKEN_EOF) \
  X(TOKEN_COUNT) \


#define X(name) name,
typedef enum {
  LIST_OF_TOKEN_TYPES
} TokenType;

#undef X

extern const char *token_type_strings[];

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
} Token;

ARRAYLIST_PROTOTYPE(Token)
#endif
