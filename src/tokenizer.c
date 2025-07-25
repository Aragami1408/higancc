#include "higancc.h"

Token *token;
char *user_input;


void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, " ");
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

bool consume(char *op) {
	if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}

Token *consume_ident(void) {
	if (token->kind != TK_IDENT)
		return NULL;
	return token;
}

void expect(char *op) {
	if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		error_at(token->str, "'%s' not found", op);
	token = token->next;
}

int expect_number() {
	if (token->kind != TK_NUM)
		error_at(token->str, "no number found");
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}


Token *new_token(TokenKind kind, Token *cur, char *str) {
	Token *tok = (Token *) calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = 1;
	cur->next = tok;
	return tok;
}

Token *tokenize(char *p) {
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}

		switch (*p) {
			case '+':
			case '-':
			case '*':
			case '/':
			case '(':
			case ')':
			case ';':
				cur = new_token(TK_RESERVED, cur, p++);
				continue;
			case '<':
			case '>':
				cur = new_token(TK_RESERVED, cur, p++);
				if (*p == '=') {
					p++;
					cur->len = 2;
				}
				continue;
			case '!':
			case '=':
				cur = new_token(TK_RESERVED, cur, p++);
				if (*p == '=') {
					p++;
					cur->len = 2;
				}
				continue;
			default: {
				if (isdigit(*p)) {
					cur = new_token(TK_NUM, cur, p);
					cur->val = strtol(p, &p, 10);
					cur->len = floor(log10(abs(cur->val))) + 1;
					continue;
				}
				if ('a' <= *p && *p <= 'z') {
					cur = new_token(TK_IDENT, cur, p++);
					cur->len = 1;
					continue;
				}
				error_at(cur->str, "Can't tokenize");
			}
		}
	}

	new_token(TK_EOF, cur, p);
	return head.next;
}
