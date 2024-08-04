#include <stdio.h>
#include <string.h>

#include "options.h"
#include "utils.h"
#include "lexer.h"


int main(int argc, char **argv) {
  if (argc == 1) {
    printf("higancc: fatal error: no input files\n");
    printf("compilation terminated\n");
    return 1;
  }
  Options opts = Options_parse(argc, argv);

  Lexer *lexer = NULL;
  Token *tokens = NULL;

  if (opts.lexer) {
    printf("Here's the Token list: \n");
    const char *buffer = read_file(argv[2]);

    lexer = Lexer_init(buffer);

    usize tokens_len = 0;
    tokens = Lexer_scanTokens(lexer, &tokens_len);
    for (int i = 0; i < tokens_len; i++) {
      Token token = tokens[i];
      print_token(&token);
      if (token.type == TOKEN_ERROR || token.type == TOKEN_EOF) {
        break;
      }
    }
  }
  if (opts.parser) {
    printf("Here's the AST: \n");
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
