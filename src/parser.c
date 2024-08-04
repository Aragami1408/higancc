#include "parser.h"
#include <stdarg.h>

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

