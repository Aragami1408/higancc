#include "token.h"


#include <stdio.h>

#define X(name) [name] = #name ,

const char *token_type_strings[] = {
  LIST_OF_TOKEN_TYPES
};

#undef X

void print_token(const Token *token) {
  printf("%4d ", token->line); 
  printf("%s \"%.*s\"\n", token_type_strings[token->type], token->length, token->start);
}
