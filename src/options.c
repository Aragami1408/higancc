#include "options.h"


void Options_printHelp() {
  printf("Usage: program [OPTIONS]\n");
  printf("OPTIONS:\n");
  printf("  --lexer         Print the tokens and exit\n");
  printf("  --parse        Print the Abstract Syntax Tree only\n");
  printf("  -S              Emit assembly only\n");
  printf("  -o <file>       Output file name\n");
  printf("  --help          Display this message\n");
}
Options Options_parse(int argc, char **argv) {
  Options opts = {false, false, false, NULL};

  static struct option long_options[] = {
    {"lexer", no_argument, 0, 'l'},
    {"parse", no_argument, 0, 'p'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;

  while ((c = getopt_long(argc, argv, "So:", long_options, &option_index)) != -1) {
    switch (c) {
      case 'l':
        opts.lexer = true;
        break;
      case 'p':
        opts.parser = true;
        break;
      case 'S':
        opts.emit_assembly = true;
        break;
      case 'o':
        opts.output_file = optarg;
        break;
      case 'h':
        Options_printHelp();
        exit(0);
      case '?':
        // getopt_long already printed an error message
        Options_printHelp();
        exit(1);
      default:
        abort();
    }
  }

  return opts;
}
