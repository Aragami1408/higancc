#include <stdio.h>
#include <string.h>

#include "options.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"


Lexer *lexer = NULL;

Token *tokens = NULL;
usize tokens_len = 0;

Parser *parser = NULL;

AST *global_ast = NULL;

#ifndef NDEBUG
void lexer_log() {
  if (lexer == NULL) {
    fprintf(stderr, "The lexer hasn't been properly initialized\n");
    exit(1);
  }
  printf("LEXER DEBUG\n");
  printf("Token list: \n");

  for (int i = 0; i < tokens_len; i++) {
    Token token = tokens[i];
    print_token(&token);
    if (token.type == TOKEN_ERROR || token.type == TOKEN_EOF) {
      break;
    }
  }
}

void parser_log() {
  printf("PARSER DEBUG\n");
  global_ast = Parser_parse(parser);
  ast_print(global_ast);
}

#endif

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("higancc: fatal error: no input files\n");
    printf("compilation terminated\n");
    return 1;
  }

  Options opts = Options_parse(argc, argv);

  const char *buffer = read_file(argv[argc-1]);

  lexer = Lexer_init(buffer);
  tokens = Lexer_scanTokens(lexer, &tokens_len);
  parser = Parser_init(tokens);

  for (int i = 0; i < tokens_len; i++) {
    Token token = tokens[i];
    if (token.type == TOKEN_ERROR) {
      fprintf(stderr, "[LEXER ERROR - line %d] %.*s\n", token.line, token.length, token.start);
      exit(1);
    }
  }

  if (opts.lexer) {
    lexer_log();
  }
  if (opts.parser) {
    parser_log();
  }
  if (opts.emit_assembly) {
    printf("Here's the Assembly: \n");
  }

  if (opts.output_file) {
    printf("Compile to '%s'\n", opts.output_file);
  }

  Lexer_free(lexer);

  return 0;
}
