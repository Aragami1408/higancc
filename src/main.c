#include <stdio.h>

#include "options.h"
#include "utils.h"
#include "lexer.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("higancc: fatal error: no input files\n");
    printf("compilation terminated\n");
    exit(1);
  }
  Options opts = Options_parse(argc, argv);

  Lexer *lexer = NULL;

  if (opts.lexer) {
    printf("Here's the Token list: \n");
    const char *buffer = read_file(argv[2]);

    lexer = Lexer_init(buffer);
    int line = -1;
    for (;;) {
      Token token = Lexer_scanToken(lexer);
      if (token.line != line) {
        printf("%4d ", token.line);
      }
      else {
        printf("\t| ");
      }
      printf("%2d '%.*s'\n", token.type, token.length, token.start);

      if (token.type == TOKEN_EOF) break;
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
