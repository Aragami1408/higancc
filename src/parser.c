#include "parser.h"
#include "ast.h"
#include "utils.h"

#include <stdarg.h>

// HELPER FUNCTIONS
// ----------------

static Token peek(const Parser *parser) {
  return parser->tokens[parser->current];
}

static Token previous(const Parser *parser) {
  return parser->tokens[parser->current - 1];
}

static bool is_at_end(const Parser *parser) {
  return peek(parser).type == TOKEN_EOF;
}

static Token advance(Parser *parser) {
  if (!is_at_end(parser)) parser->current++;
  return previous(parser);
}

static bool check(const Parser *parser, TokenType type) {
  if (is_at_end(parser)) return false;
  return peek(parser).type == type;
}

static bool match(Parser *parser, usize count, ...) {
  va_list args; 
  va_start(args, count);

  for (int i = 0; i < count; i++) {
    TokenType type = va_arg(args, TokenType);
    if (check(parser, type)) {
      advance(parser);
      va_end(args);
      return true;
    }
  }

  va_end(args);
  return false;
}

// RECURSIVE DESCENT PARSING
// -------------------------

AST *parse_function(Parser *parser);
AST *parse_statement(Parser *parser);
AST *parse_exp(Parser *parser);


AST *parse_function(Parser *parser) {
  if (!(check(parser, TOKEN_KW_INT) || check(parser, TOKEN_KW_FLOAT))) {
    fprintf(stderr, "Expect 'int' or 'float' before function name.\n");
    exit(1);
  }
  advance(parser);

  if (!check(parser, TOKEN_IDENTIFIER)) {
    fprintf(stderr, "Expect valid function name after 'int'\n");
    exit(1);
  }
  Token function_name = peek(parser);
  advance(parser);

  if (!check(parser, TOKEN_LEFT_PAREN)) {
    fprintf(stderr, "Expect '(' after function name\n");
    exit(1);
  }
  advance(parser);

  if (!check(parser, TOKEN_KW_VOID)) {
    fprintf(stderr, "Expect 'void' after '('\n");
    exit(1);
  }
  advance(parser);

  if (!check(parser, TOKEN_RIGHT_PAREN)) {
    fprintf(stderr, "Expect ')' after 'void'\n");
    exit(1);
  }
  advance(parser);

  if (!check(parser, TOKEN_LEFT_BRACE)) {
    fprintf(stderr, "Expect '{' after ')'\n");
    exit(1);
  }
  advance(parser);

  AST *statement = parse_statement(parser);

  if (!check(parser, TOKEN_RIGHT_BRACE)) {
    fprintf(stderr, "Expect '}' after statement\n");
    exit(1);
  }
  advance(parser);

  return AST_NEW(AST_FUNCTION, function_name, statement);

}

AST *parse_statement(Parser *parser) {
  if (!check(parser, TOKEN_KW_RETURN)) {
    fprintf(stderr, "Expect 'return' before expression.\n");
    exit(1);
  }
  advance(parser);
  AST *return_val = parse_exp(parser);
  advance(parser);
  if (!check(parser, TOKEN_SEMICOLON)) {
    fprintf(stderr, "Expect ';' after expression.\n");
    exit(1);
  }
  advance(parser);
  return AST_NEW(AST_RETURN, return_val);
}

AST *parse_exp(Parser *parser) {
  if (check(parser, TOKEN_INT) || check(parser, TOKEN_FLOAT)) {
    return AST_NEW(AST_EXP, substring(peek(parser).start, 0, peek(parser).length));
  }
  else if (check(parser, TOKEN_CHAR)) {
    return AST_NEW(AST_EXP, substring(peek(parser).start, 1, peek(parser).length-2));
  }
  else {
    fprintf(stderr, "Expression must be a number.");
    exit(1);
  }
}

// PUBLIC METHODS
// --------------
Parser *Parser_init(Token *tokens) {
  Parser *parser = (Parser *)malloc(sizeof(Parser));
  parser->tokens = tokens;
  parser->current = 0;
  return parser;
}

AST *Parser_parse(Parser *parser) {
  return parse_function(parser);
}

void Parser_destroy(Parser *parser) {
  free(parser);
}

