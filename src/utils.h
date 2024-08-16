#ifndef HIGANCC_UTILS_H
#define HIGANCC_UTILS_H

char *read_file(const char *path);
char *substring(const char *str, int start, int length);
int str_to_num(const char *str, void *result, char type);

#endif
