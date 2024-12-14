#include "argparse.h"

void print_usage(const char *program_name) {
	printf("Usage: %s [options] <input_file>\n", program_name);
	printf("Options:\n");
	printf("  -l, --lex               Perform lexical analysis and print tokens\n");
	printf("  -p, --parse             Perform parsing and print AST\n");
	printf("  -c, --codegen           Perform code generation and print assembly code\n");
	printf("  -t, --tacky             Perform IR conversion and print TACKY code\n");
	printf("  -o, --output <file>     Specify output file (default: a.out)\n");
	printf("  -h, --help              Display this help message\n");
}
