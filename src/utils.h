#ifndef HIGANCC_UTILS_H
#define HIGANCC_UTILS_H

#include "memory.h"

char *read_file(ArenaAllocator *a, const char *path);
char *substring(ArenaAllocator *a, const char *str, int start, int length);
int str_to_num(const char *str, void *result, char type);

#endif
