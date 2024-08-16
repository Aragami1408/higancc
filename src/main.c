#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "common.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "asm_tree.h"

#include "stb_ds.h"

void print_usage(const char* program_name) {
  printf("Usage: %s [options] <input_file>\n", program_name);
  printf("Options:\n");
  printf("  -l, --lex               Perform lexical analysis and print tokens\n");
  printf("  -p, --parse             Perform parsing and print AST\n");
  printf("  -c, --codegen           Perform code generation and print assembly tree\n");
  printf("  -o, --output <file>     Specify output file (default: a.out)\n");
  printf("  -h, --help              Display this help message\n");
}

void print_tokens(Token *tokens, usize len) {
  for (usize i = 0; i < len; i++) {
    Token token = tokens[i];
    print_token(&token);
    if (token.type == TOKEN_ERROR || token.type == TOKEN_EOF) {
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  char *input_file = NULL;
  const char *output_file = "a.out";
  bool do_lex = false;
  bool do_parse = false;
  bool do_codegen = false;

  static struct option long_options[] = {
    {"lex",     no_argument,       0, 'l'},
    {"parse",   no_argument,       0, 'p'},
    {"codegen", no_argument,       0, 'c'},
    {"output",  required_argument, 0, 'o'},
    {"help",    no_argument,       0, 'h'},
    {0, 0, 0, 0}
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "lpco:h", long_options, NULL)) != -1) {
    switch (opt) {
      case 'l': do_lex = true; break;
      case 'p': do_parse = true; break;
      case 'c': do_codegen = true; break;
      case 'o': output_file = optarg; break;
      case 'h': print_usage(argv[0]); return 0;
      default: fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]); return 1;
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Error: No input file specified\n");
    print_usage(argv[0]);
    return 1;
  }

  input_file = argv[optind];

  if (!do_lex && !do_parse && !do_codegen) {
    do_lex = do_parse = do_codegen = true;
  }

  char *source = read_file(input_file);  

  Lexer *lexer = Lexer_init(source);
  usize token_list_size = 0;
  Token *tokens = Lexer_scanTokens(lexer, &token_list_size);
  if (tokens == NULL) {
    fprintf(stderr, "Lexical analysis failed\n");
    arrfree(tokens);  
    Lexer_free(lexer);
    return 1;
  }

  for (usize i = 0; i < token_list_size; i++) {
    Token token = tokens[i];
    if (token.type == TOKEN_ERROR) {
      fprintf(stderr, "[LEXER ERROR - line %d] %.*s\n", token.line, token.length, token.start);
      return 1;
    }
  }

  if (do_lex) {
    printf("[LEX ONLY]\n");
    print_tokens(tokens, token_list_size);
    printf("\n");
  }

  Parser *parser = Parser_init(tokens);
  usize global_ast_size = 0;
  AST *global_ast = Parser_parse(parser, &global_ast_size);
  if (global_ast == NULL) {
    fprintf(stderr, "Parsing failed\n"); 
    arrfree(tokens);  
    Lexer_free(lexer);
    ast_free(global_ast);
    Parser_free(parser);
    return 1;
  }

  if (do_parse) {
    printf("[PARSING ONLY]\n");
    ast_print(global_ast);
    printf("\n");
  }

  // TODO: implement proper codegen 
  ASMNode* function = ASMNode_createFunction("main");
  ASMNode* mov_inst = ASMNode_createMov(ASMNode_createImm(42), ASMNode_createRegister(REG_EAX));
  ASMNode_addInstruction(function, mov_inst);
  ASMNode_addInstruction(function, ASMNode_createRet());
  ASMNode* program = ASMNode_createProgram(function);

  if (do_codegen) {
    printf("[CODEGEN ONLY]\n");
    print_asm_tree(program, 0);
    printf("\n");
  }

  FILE *out_file = fopen(output_file, "w");
  if (out_file == NULL) {
    perror("Error opening output file");
    arrfree(tokens);  
    Lexer_free(lexer);
    ast_free(global_ast);
    Parser_free(parser);
    ASMNode_free(program);
    return 1;
  }

  arrfree(tokens);  
  Lexer_free(lexer);
  ast_free(global_ast);
  Parser_free(parser);
  ASMNode_free(program);

  return 0; 
}
