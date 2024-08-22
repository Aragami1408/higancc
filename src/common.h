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

typedef float f32;
typedef double f64;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define HIGANCC_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define HIGANCC_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define HIGANCC_PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define HIGANCC_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define HIGANCC_PLATFORM_POSIX 1
#elif __APPLE__
// Apple platforms
#define HIGANCC_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define HIGANCC_PLATFORM_IOS 1
#define HIGANCC_PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define HIGANCC_PLATFORM_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#endif
