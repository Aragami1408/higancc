#include <stdio.h>
#include <string.h>

#include "common.h"
#include "argparse.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "memory.h"
#include "arraylist.h"
#include "dump.h"
#include "tacky.h"

// TODO(higanbana): build yourself an arg parser
#include <getopt.h>

int main(int argc, char *argv[]) {
	char *input_file = NULL;

	// Default values for command line arguments
	const char *output_file = "out.S";
	bool do_lex = false;
	bool do_parse = false;
	bool do_codegen = false;
	bool do_tacky = false;

	static struct option long_options[] = {
		{"lex",     no_argument,       0, 'l'},
		{"parse",   no_argument,       0, 'p'},
		{"codegen", no_argument,       0, 'c'},
		{"tacky",   no_argument,       0, 't'},
		{"output",  optional_argument, 0, 'o'},
		{"help",    no_argument,       0, 'h'},
		{0, 0, 0, 0}
	};

	int opt;
	while ((opt = getopt_long(argc, argv, "lpcto:h", long_options, NULL)) != -1) {
		switch (opt) {
			case 'l': do_lex = true; break;
			case 'p': do_parse = true; break;
			case 'c': do_codegen = true; break;
			case 't': do_tacky = true; break;
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

	if (!do_lex && !do_parse && !do_codegen && !do_tacky) {
		do_lex = do_parse = do_codegen = do_tacky = true;
	}

	u8 backing_buffer[BACKING_BUFFER_LENGTH];
	ArenaAllocator allocator;
	ArenaAllocator_init(&allocator, backing_buffer, BACKING_BUFFER_LENGTH);

	char *source = read_file(&allocator, input_file);

    if (source != NULL) {
        printf("[SOURCE CODE]\n");
        printf("%s\n\n", source);
    }

	Lexer lexer = Lexer_init(source);
	ArrayList(Token) *tokens = Lexer_scanTokens(&lexer, &allocator);
	if (tokens == NULL) {
		fprintf(stderr, "Lexical analysis failed\n");
		ArenaAllocator_freeAll(&allocator);
		return 1;
	}

	for (usize i = 0; i < ArrayList_size(Token, tokens); i++) {
		Token token = ArrayList_get(Token, tokens, i);
		if (token.type == TOKEN_ERROR) {
			fprintf(stderr, "[LEXER ERROR - line %d] %.*s\n", token.line, token.length, token.start);
			return 1;
		}
	}

	if (do_lex) {
		printf("[LEXER - TOKEN DUMP]\n");
        dump_tokens(tokens);
		printf("\n");
	}

	Parser parser = Parser_init(tokens);
	ASTProgram *ast_program = Parser_parse(&parser, &allocator);
	if (ast_program == NULL) {
		fprintf(stderr, "Parsing failed\n");
		ArenaAllocator_freeAll(&allocator);
		return 1;
	}

	if (do_parse) {
		printf("[Parser - AST DUMP]\n");
		dump_ast(ast_program);
		printf("\n");
	}

	Tacky *tacky = Tacky_create(&allocator);
	TackyProgram *tacky_program = emit_tacky(tacky, ast_program);
	if (tacky_program == NULL) {
		fprintf(stderr, "Failed converting to TACKY\n");
		ArenaAllocator_freeAll(&allocator);
		return 1;
	}

	if (do_tacky) {
		printf("[IR - TACKY DUMP]\n");
		dump_tacky(tacky_program);
		printf("\n");
	}


	FILE *out_file = fopen(output_file, "w");
	if (out_file == NULL) {
		perror("Error opening output file");
		ArenaAllocator_freeAll(&allocator);
		fclose(out_file);
		return 1;
	}

	ArenaAllocator_freeAll(&allocator);
	fclose(out_file);

	return 0; 
}
