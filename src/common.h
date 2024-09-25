#ifndef HIGANCC_COMMON_H
#define HIGANCC_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BACKING_BUFFER_LENGTH 2048

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t usize;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define HIGANCC_PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
#define HIGANCC_PLATFORM_LINUX 1
#elif defined(__unix__)
#define HIGANCC_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define HIGANCC_PLATFORM_POSIX 1
#elif __APPLE__
#define HIGANCC_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#else
#error "Unknown platform!"
#endif
#endif
