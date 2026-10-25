// vi: lbr noet sw=2 ts=2 tw=79 wrap
// SPDX-FileCopyrightText: 2025-2026 David Rabkin
// SPDX-License-Identifier: 0BSD

#ifndef CBASE_BASE_H
#define CBASE_BASE_H

#include <stddef.h>  // size_t

/**
 * Returns the minimum of a and b. Operates on any arithmetic type and
 * evaluates each of its arguments exactly once. See more:
 *   https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Typeof.html
 * @param a First element to compare.
 * @param b Second element to compare.
 * @return Minimum of the two elements.
 */
#ifdef MIN
#undef MIN
#endif
#define MIN(a, b)       \
({                      \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a < _b ? _a : _b;  \
})

/**
 * Returns the maximum of a and b. Operates on any arithmetic type and
 * evaluates each of its arguments exactly once. See more:
 *   https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Typeof.html
 * @param a First element to compare.
 * @param b Second element to compare.
 * @return Maximum of the two elements.
 */
#ifdef MAX
#undef MAX
#endif
#define MAX(a, b)       \
({                      \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a > _b ? _a : _b;  \
})

/**
 * Counts the elements of an array. Triggers a compile-time error for a
 * non-array argument, e.g.:
 *   int arr[] = { 1, 2 };
 *   int *ptr = arr;
 *   size_t arrsz = ARRAYSIZE(arr); // OK.
 *   size_t ptrsz = ARRAYSIZE(ptr); // Compilation error.
 * See more details here:
 *   https://stackoverflow.com/questions/1598773
 * @param arr The array to count the elements of.
 * @return Number of elements as size_t.
 */
#define ARRAYSIZE(arr)                              \
(                                                   \
    (sizeof(arr) / sizeof(0[arr])) /                \
        ((size_t)(!(sizeof(arr) % sizeof(0[arr])))) \
)

/**
 * Produces a pointer one past the last element of an array. It supports the
 * usual pattern for iterating over an array:
 *   int arr[] = { 1, 2 };
 *   for (int *p = arr; p != ARRAYEND(arr); ++p) {
 *       ...
 *   }
 * @param arr The array to point past.
 * @return Pointer to the element after the last one.
 */
#define ARRAYEND(arr) ((arr) + ARRAYSIZE(arr))

/**
 * Counts the elements of an array member of a type.
 * @param type Name of the data structure.
 * @param member Name of the array member within the data structure.
 * @return Number of elements as size_t.
 */
#define MEMBERSIZE(type, member) (ARRAYSIZE(((type *)0)->member))

/**
 * Suppresses compiler warnings about unused parameters. Place it at the start
 * of a function body. Use it for function parameters, not for unused local
 * variables.
 */
#define UNREFERENCED_PARAM(param) \
    ((void)(0 ? ((param) = (param)) : (param)))

/**
 * Converts a token to a string in the preprocessor.
 */
#define _STR(x) #x
#define STR(x) _STR(x)

/**
 * Formats output into a string, like snprintf.
 * If the output does not fit into len bytes, the function discards it, sets
 * the buffer to an empty string, and fails. Otherwise the output is
 * null-terminated.
 * @param str Output buffer.
 * @param len Maximum number of bytes to write, including the final `\0'.
 * @param fmt Format string, as for snprintf.
 * @return The number of characters written, not including the final `\0'.
 * Returns -1 on a formatting error or when the buffer is too small.
 */
int print_to_string(char *str, size_t len, const char *fmt, ...);

/**
 * Describes an errno value portably, improving on strerror_r. Inspired by:
 *   http://www.club.cc.cmu.edu/~cmccabe/blog_strerror.html
 * @param err The errno value to describe.
 * @param str Points to the first byte available for printing.
 * @param len Maximum number of bytes to write.
 * @return Pointer to the formatted error string.
 */
const char *strerror_r_improved(int err, char *str, size_t len);

/**
 * Returns a pointer to a string with a timestamp formatted like:
 *   Mon Feb 17 13:22:05 2020
 * @param str Points to the first byte available for printing.
 * @param len Maximum number of bytes to write.
 * @return Pointer to the formatted timestamp, or the string "error" on
 * failure.
 */
const char *humanized_timestamp(char *str, size_t len);

#endif  // CBASE_BASE_H
