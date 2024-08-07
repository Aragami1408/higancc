
#include <stdarg.h>

#include "parser.h"
#include "ast.h"
#include "utils.h"

#include "stb_ds.h"

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

static void error(const Parser *parser, const char *msg) {
  Token current_token = peek(parser);
  fprintf(stderr, "[PARSER ERROR - line %d] %s\n", current_token.line, msg);
  exit(1);
}

// RECURSIVE DESCENT PARSING
// -------------------------

AST *parse_function(Parser *parser);
AST *parse_statement(Parser *parser);
AST *parse_exp(Parser *parser);


AST *parse_function(Parser *parser) {
  if (!(check(parser, TOKEN_KW_INT) || check(parser, TOKEN_KW_FLOAT))) {
    error(parser, "Expect 'int' or 'float' before function name.");
  }
  advance(parser);

  if (!check(parser, TOKEN_IDENTIFIER)) {
    error(parser, "Expect valid function name after 'int'");
  }
  Token function_name = peek(parser);
  advance(parser);

  if (!check(parser, TOKEN_LEFT_PAREN)) {
    error(parser, "Expect '(' after function name");
  }
  advance(parser);

  if (!check(parser, TOKEN_KW_VOID)) {
    error(parser, "Expect 'void' after '('");
  }
  advance(parser);

  if (!check(parser, TOKEN_RIGHT_PAREN)) {
    error(parser, "Expect ')' after 'void'");
  }
  advance(parser);

  if (!check(parser, TOKEN_LEFT_BRACE)) {
    error(parser, "Expect '{' after ')'");
  }
  advance(parser);

  AST *statement = parse_statement(parser);

  if (!check(parser, TOKEN_RIGHT_BRACE)) {
    error(parser, "Expect '}' after statement");
  }
  advance(parser);

  return AST_NEW(AST_FUNCTION, function_name, statement);

}

AST *parse_statement(Parser *parser) {
  if (!check(parser, TOKEN_KW_RETURN)) {
    error(parser, "Expect 'return' before expression.");
  }
  advance(parser);
  AST *return_val = parse_exp(parser);
  advance(parser);
  if (!check(parser, TOKEN_SEMICOLON)) {
    error(parser, "Expect ';' after expression.");
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
    error(parser, "Expression must be a number.");
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

AST *Parser_parse(Parser *parser, usize *len) {
  AST *asts = NULL;

  AST *ast = NULL;
  while (!is_at_end(parser)) {
    ast = parse_function(parser);
    arrput(asts, *ast);
  }

  *len = arrlenu(asts);

  return asts;
}

void Parser_destroy(Parser *parser) {
  free(parser);
}

