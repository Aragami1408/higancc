#include "higancc.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Incorrect number of arguments\n");
		return 1;
	}

	user_input = argv[1];

	token = tokenize(user_input);

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");

	printf("    mov rax, %d\n", expect_number());

	while (!at_eof()) {
		if (consume('+')) {
			printf("    add rax, %d\n", expect_number());
			continue;
		}

		expect('-');
		printf("    sub rax, %d\n", expect_number());
	}

	printf("    ret\n");
	return 0;
}
