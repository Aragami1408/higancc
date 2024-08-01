#include "token.h"

#include <stdio.h>

void print_token(const Token *token) {
  printf("%4d ", token->line); 
  printf("%2d '%.*s'\n", token->type, token->length, token->start);
}
