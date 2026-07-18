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
 * Produces a pointer one past the last element of an array. It serves the
 * usual pattern for iterating over sequence data:
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
 * Suppresses compiler warnings about unused parameters. Place it at the very
 * beginning of a function body. It is meant for function parameters rather
 * than for unused local variables.
 */
#define UNREFERENCED_PARAM(param) \
    ((void)(0 ? ((param) = (param)) : (param)))

/**
 * Converts a token to a string in the preprocessor.
 */
#define _STR(x) #x
#define STR(x) _STR(x)

/**
 * Converts formatted output, similar to snprintf with logged error handling.
 * The function writes at most len-1 of the printed characters into the output
 * string (the len'th character then gets the terminating `\0'). If the return
 * value is greater than or equal to the len argument, the string was too
 * short and some of the printed characters were discarded. The output is
 * always null-terminated, unless len is 0.
 * @param str Output buffer.
 * @param len Maximum number of bytes to write.
 * @param fmt Format string, similar to snprintf.
 * @return The number of characters that would have been printed had the size
 * been unlimited, not including the final `\0'. Returns a negative value if
 * an error occurs.
 */
int print_to_string(char *str, size_t len, const char *fmt, ...);

/**
 * Improves on strerror_r, inspired by the article:
 *   http://www.club.cc.cmu.edu/~cmccabe/blog_strerror.html
 * @param err The errno value to describe.
 * @param str Points to the first byte available for printing.
 * @param len Maximum number of bytes to write.
 * @return Pointer to the formatted error string.
 */
const char *strerror_r_improved(int err, char *str, size_t len);

/**
 * Returns a pointer to a string with a timestamp formatted similar to:
 *   Mon Feb 17 13:22:05 2020
 * @param str Points to the first byte available for printing.
 * @param len Maximum number of bytes to write.
 * @return Pointer to the formatted timestamp, the string 'error' on error.
 */
const char *humanized_timestamp(char *str, size_t len);

#endif  // CBASE_BASE_H
