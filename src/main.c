#include <stdio.h>
#include <string.h>

#include "common.h"
#include "options.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "asm_tree.h"

#include "stb_ds.h"
#define STB_DS_IMPLEMENTATION

void print_usage(const char *program_name) {
  printf("Usage: %s [options] input_file\n", program_name);
  printf("Options:\n");
  printf("  -o <file>    Specify output file\n");
  printf("  --lex        Perform lexical analysis only and print tokens\n");
  printf("  --parse      Perform lexical and syntax analysis and print AST\n");
  printf("  -S           Perform lexical, syntax analysis and code generation, print ASM tree\n");
}

int main(int argc, char *argv[]) {
  char *input_file = NULL;
  char *output_file = "a.out";
  bool lex_only = false;
  bool parse_only = false;
  bool codegen_only = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      output_file = argv[++i];
    }
    else if (strcmp(argv[i], "--lex") == 0) {
      lex_only = true;
    }
    else if (strcmp(argv[i], "--parse") == 0) {
      parse_only = true;
    }
    else if (strcmp(argv[i], "-S") == 0) {
      codegen_only = true;
    }
    else {
      fprintf(stderr, "Unknown argument: %s\n", argv[i]);
      print_usage(argv[0]);
      return 1;
    }
  }

  if (input_file == NULL) {
    fprintf(stderr, "No input file specified\n");
    print_usage(argv[0]);
    return 1;
  }

  char *source = read_file(input_file);  

  Lexer *lexer = Lexer_init(source);

  usize token_list_size = 0;
  Token *tokens = Lexer_scanTokens(lexer, &token_list_size);
  if (tokens == NULL) {
    fprintf(stderr, "Lexical analysis failed\n");
    return 1;
  }

  if (lex_only) {
    print_tokens(tokens);
    arrfree(tokens);  
    Lexer_free(lexer);
    return 0;
  }

  
}
