
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

static bool is_at_end(const Lexer *lexer) {
  return *lexer->current == '\0';
}

static Token make_token(const Lexer *lexer, TokenType type) {
  Token token;

  token.type = type;
  token.start = lexer->start;
  token.length = (int)(lexer->current - lexer->start);
  token.line = lexer->line; 

  return token;
}

static Token error_token(const Lexer *lexer, const char *message) {
  Token token;

  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = lexer->line; 

  return token;
}

static char advance(Lexer *lexer) {
  lexer->current++;
  return lexer->current[-1];
}

static bool match(Lexer *lexer, const char expected) {
  if (is_at_end(lexer)) return false;
  if (*lexer->current != expected) return false;
  lexer->current++;
  return true;
}

static char peek(const Lexer *lexer) {
  return *lexer->current;
}

static char peek_next(const Lexer *lexer) {
  if (is_at_end(lexer)) return '\0';
  return lexer->current[1];
}

static void skip_whitespace(Lexer *lexer) {
  for (;;) {
    char c = peek(lexer);
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance(lexer);
        break;
      case '\n':
        lexer->line++;
        advance(lexer);
        break;
      case '/':
        if (peek_next(lexer) == '*') {
          while (!is_at_end(lexer)) {
            if (peek(lexer) == '\n') {
              lexer->line++;
            }
            if (peek(lexer) == '*' && peek_next(lexer) == '/') {
              advance(lexer);
              advance(lexer);
              break;
            }
            advance(lexer);
          }
        }
        else if (peek_next(lexer) == '/') {
          while (peek(lexer) != '\n' && !is_at_end(lexer)) advance(lexer);
        }
        else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static Token string(Lexer *lexer) {
  while (peek(lexer) != '"' && !is_at_end(lexer)) {
    if (peek(lexer) == '\n') lexer->line++;
    advance(lexer);
  }

  if (is_at_end(lexer)) return error_token(lexer, "Unterminated string.");

  advance(lexer);
  return make_token(lexer, TOKEN_STRING);
}

static Token character(Lexer *lexer) {
  // TODO(higanbana): only capture one character enclosed in '', otherwise return error token
  while (peek(lexer) != '\'' && !is_at_end(lexer)) {
    if (peek(lexer) == '\n') lexer->line++;
    advance(lexer);
  }

  if (is_at_end(lexer)) return error_token(lexer, "Unterminated char.");

  advance(lexer);
  return make_token(lexer, TOKEN_CHAR);
}

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
          c == '_';
}

static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static TokenType check_keyword(const Lexer *lexer, int start, int length, const char *rest, TokenType type) {
  if (lexer->current - lexer->start == start + length && memcmp(lexer->start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType identifier_or_keyword_type(const Lexer *lexer) {
  switch (lexer->start[0]) {
    case 'c': return check_keyword(lexer, 1, 3, "har", TOKEN_KW_CHAR);
    case 'f':
      if (lexer->current - lexer->start > 1) {
        switch (lexer->start[1]) {
          case 'l': return check_keyword(lexer, 2, 3, "oat", TOKEN_KW_FLOAT);
        }
      }
      break;
    case 'i': 
      if (lexer->current - lexer->start > 1) {
        switch (lexer->start[1]) {
          case 'n': return check_keyword(lexer, 2, 1, "t", TOKEN_KW_INT);
          case 'f': return TOKEN_KW_IF;
        }
      }
      break;
    case 'r': return check_keyword(lexer, 1, 5, "eturn", TOKEN_KW_RETURN);
    case 'v': return check_keyword(lexer, 1, 3, "oid", TOKEN_KW_VOID);
  }
  return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer *lexer) {
  while (is_alpha(peek(lexer)) || is_digit(peek(lexer))) advance(lexer);
  return make_token(lexer, identifier_or_keyword_type(lexer));
}

static Token number(Lexer *lexer) {
  if (is_alpha(peek_next(lexer))) {
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer))) advance(lexer); 
    return error_token(lexer, "Identifier must not contain any number at the beginning");
  }
  while (is_digit(peek(lexer))) advance(lexer);

  if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
    advance(lexer);

    while (is_digit(peek(lexer))) advance(lexer);
    return make_token(lexer, TOKEN_FLOAT);
  }
  
  return make_token(lexer, TOKEN_INT);
}

Lexer *Lexer_init(ArenaAllocator *a, const char *source) {
  Lexer *lexer = (Lexer *)ArenaAllocator_alloc(a, sizeof(Lexer));
  if (lexer == NULL) {
    fprintf(stderr, "Failed to allocate lexer\n");
    exit(1);
  }

  lexer->start = source;
  lexer->current = source;
  lexer->line = 1;
  lexer->allocator = a;

  return lexer;
}

Token *Lexer_scanTokens(Lexer *lexer, usize *len) {
  Token *tokens = NULL;

  Token token;
  do {
    token = Lexer_scanToken(lexer);
    arrput(tokens, token);
  } while (token.type != TOKEN_EOF);

  *len = arrlenu(tokens);

  return tokens;
}

Token Lexer_scanToken(Lexer *lexer) {
  skip_whitespace(lexer);
  lexer->start = lexer->current;

  if (is_at_end(lexer)) return make_token(lexer, TOKEN_EOF);

  char c = advance(lexer);
  if (is_alpha(c)) return identifier(lexer);
  if (is_digit(c)) return number(lexer);

  switch (c) {
    case '(': return make_token(lexer, TOKEN_LEFT_PAREN);
    case ')': return make_token(lexer, TOKEN_RIGHT_PAREN);
    case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
    case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);
    case ';': return make_token(lexer, TOKEN_SEMICOLON);
    case ',': return make_token(lexer, TOKEN_COMMA);
    case '.': return make_token(lexer, TOKEN_DOT);
    case '-': return make_token(lexer, TOKEN_MINUS);
    case '+': return make_token(lexer, TOKEN_PLUS);
    case '/': return make_token(lexer, TOKEN_SLASH);
    case '*': return make_token(lexer, TOKEN_STAR);
    case '!':
              return make_token(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
              return make_token(lexer, match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
              return make_token(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
              return make_token(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"':
              return string(lexer);
    case '\'':
              return character(lexer);
  }


  return error_token(lexer, "Unexpected character.");
}

