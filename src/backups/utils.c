#include "utils.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

char* read_file(ArenaAllocator* a, const char* path) {
	FILE* file = fopen(path, "rb");
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

char* substring(ArenaAllocator* a, const char* str, int start, int length) {
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

	char* result = (char*)ArenaAllocator_alloc(a, (length + 1) * sizeof(char));
	if (result == NULL) {
		return NULL;
	}

	strncpy(result, str + start, length);
	result[length] = '\0';

	return result;
}

int str_to_num(const char* str, void* result, char type) {
	char* endptr;
	errno = 0;

	switch (type) {
	case 'd':
		*(double*)result = strtod(str, &endptr);
		break;
	case 'i':
		*(int*)result = (int)strtol(str, &endptr, 10);
		break;
	default:
		return 1; // Invalid type
	}

	if (errno != 0 || *endptr != '\0') {
		return 1;
	}

	return 0;
}


char* format_string(const char* format, ...) {
	va_list args;
	va_start(args, format);

	// Calculate the required buffer size
	va_list args_copy;
	va_copy(args_copy, args);
	int size = vsnprintf(NULL, 0, format, args_copy);
	va_end(args_copy);

	if (size < 0) {
		va_end(args);
		return NULL;
	}

	// Allocate memory for the formatted string
	char* formatted_str = (char*)malloc(size + 1);
	if (formatted_str == NULL) {
		va_end(args);
		return NULL;
	}

	// Format the string
	vsnprintf(formatted_str, size + 1, format, args);
	va_end(args);

	return formatted_str;
}

#ifndef HIGANCC_PLATFORM_POSIX
char *strdup(const char *s) {
	usize size = strlen(s) + 1;
	char *p = malloc(size);
	if (p != NULL) {
		memcpy(p, s, size);
	}
	return p;
}

char *strndup(const char *s, usize n) {
	char *p;
	usize n1;

	for (n1 = 0; n1 < n && s[n1] != '\0'; n1++)
		continue;
	p = malloc(n + 1);
	if (p != NULL) {
		memcpy(p, s, n1);
		p[n1] = '\0';
	}
	return p;
}
#endif
