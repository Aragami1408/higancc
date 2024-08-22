#include "utils.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *read_file(ArenaAllocator *a, const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file '%s'\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  usize file_size = ftell(file);
  rewind(file);

  char* buffer = (char*)ArenaAllocator_alloc(a, file_size + 1);
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

char *substring(ArenaAllocator *a, const char *str, int start, int length) {
  if (str == NULL || start < 0 || length <= 0) {
    return NULL;
  }

  int str_len = strlen(str);

  if (start >= str_len) {
    return NULL;
  }

  if ((start + length) > str_len) {
    length = str_len - start;
  }

  char *result = (char*)ArenaAllocator_alloc(a, (length+1) * sizeof(char));
  if (result == NULL) {
    return NULL;
  }

  strncpy(result, str+start, length);
  result[length] = '\0';

  return result;
}

int str_to_num(const char *str, void *result, char type) {
  char *endptr;
  errno = 0;

  switch (type) {
    case 'd':
      *(double *)result = strtod(str, &endptr);
      break;
    case 'i':
      *(int *)result = (int)strtol(str, &endptr, 10);
      break;
    default:
      return 1; // Invalid type
  }

  if (errno != 0 || *endptr != '\0') {
    return 1;
  }

  return 0;
}
