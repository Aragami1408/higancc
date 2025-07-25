#ifndef HIGANCC_H
#define HIGANCC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// tokenizer.c
typedef enum {
	TK_RESERVED,
	TK_IDENT,
	TK_NUM,
	TK_EOF
} TokenKind;

typedef struct Token Token;

struct Token {
	TokenKind kind;
	Token *next;
	int val; // is set when TK_NUM
	char *str;
	int len;
};

extern Token *token;

extern char *user_input;

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

bool consume(char *op);
Token *consume_ident(void);
void expect(char *op);
int expect_number(void);
bool at_eof(void);
Token *new_token(TokenKind kind, Token *cur, char *str);
Token *tokenize(char *p);

// parser.c
typedef enum {
	ND_ADD, ND_SUB, ND_MUL, ND_DIV,

	ND_EQ, ND_NEQ, ND_LT, ND_GT, ND_LE, ND_GE,

	ND_ASSIGN,
	ND_LVAR, // Local variable

	ND_NUM
} NodeKind;

typedef struct Node Node;

struct Node {
	NodeKind kind;
	Node *lhs;
	Node *rhs;
	int val; // use if kind = ND_NUM
	int offset; // use if kind = ND_LVAR
};

extern Node *code[100];

void program(void);

// codegen.c
void gen(Node *node);


#endif
