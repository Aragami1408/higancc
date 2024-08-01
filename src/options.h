#ifndef HIGANCC_OPTIONS_H
#define HIGANCC_OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

typedef struct {
  bool lexer;
  bool parser;
  bool emit_assembly;
  char *output_file;
} Options;

void Options_printHelp();
Options Options_parse(int argc, char **argv);

#endif
