#include "utils.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file '%s'\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  usize file_size = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read '%s'\n", path);
    exit(74);
  }

  usize bytes_read = fread(buffer, sizeof(char), file_size, file);
  buffer[bytes_read] = '\0';

  if (bytes_read < file_size) {
    fprintf(stderr, "Could not read file '%s'\n", path);
    exit(74);
  }

  fclose(file);
  return buffer;
}
