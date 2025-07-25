#include "higancc.h"

void print_tokens(Token *token) {
	Token *cur = token;
	while (cur != NULL) {
		if (cur->kind == TK_EOF)
			printf("Token: EOF\n");
		else
			printf("Token: %.*s\n", cur->len, cur->str);
		cur = cur->next;
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Incorrect number of arguments\n");
		return 1;
	}

	user_input = argv[1];
	token = tokenize(user_input);
	// print_tokens(token);
	program();

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");

	// Prologue
	// Allocate space for 26 variables (from 'a' to 'z')
	printf("    push rbp\n");
	printf("    mov rbp, rsp\n");
	printf("    sub rsp, 208\n");

	for (int i = 0; code[i]; i++) {
		gen(code[i]);

		// Since there should be one value remaining on the stack as the result of evaluating the expression
		// Pop it to prevent to stack from overflowing
		printf("    pop rax\n");
	}

	// Epilogue
	// The result of the last expression remains in RAX, so that becomes the return value
	printf("    mov rsp, rbp\n");
	printf("    pop rbp\n");
	printf("    ret\n");

	return 0;
}
