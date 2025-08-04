// SPDX-FileCopyrightText: 2025 David Rabkin
// SPDX-License-Identifier: 0BSD

#ifndef CBASE_BASE_H
#define CBASE_BASE_H

#include <stddef.h>  // size_t

/**
 * A macro that returns the minimum of a and b. Operates on any arithmetic type
 * and evaluates each of its arguments exactly once, see more:
 *   https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Typeof.html
 * @param First element to compare.
 * @param Second element to compare.
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
 * A macro that returns the maximum of a and b. Operates on any arithmetic type
 * and evaluates each of its arguments exactly once, see more:
 *   https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Typeof.html
 * @param First element to compare.
 * @param Second element to compare.
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
 * Counts of element in an array. Ensures compile time error in case of
 * non-array argument, e.g.:
 *   int arr[] = { 1, 2 };
 *   int *ptr = arr;
 *   size_t arrsz = ARRAYSIZE(arr); // OK.
 *   size_t ptrsz = ARRAYSIZE(ptr); // Compilation error.
 * See more details here:
 *   https://stackoverflow.com/questions/1598773
 * @param An array to count element of.
 * @return Size of the array of size_t type.
 */
#define ARRAYSIZE(arr)                              \
(                                                   \
    (sizeof(arr) / sizeof(0[arr])) /                \
        ((size_t)(!(sizeof(arr) % sizeof(0[arr])))) \
)

/**
 * Counts a pointer to an element of an array after last. It is used in a
 * pattern to go through sequence data:
 *   int arr[] = { 1, 2 };
 *   for (int *p = arr; p != ARRAYEND(arr); ++p) {
 *       ...
 *   }
 * @param arr An array to count a pointer to an element after last.
 * @return The pointer to an element after last.
 */
#define ARRAYEND(arr) ((arr) + ARRAYSIZE(arr))

/**
 * Counts elements of a data member of a type.
 * @param type Data structure name.
 * @param member Data member name of the data structure.
 * @return Size of the array of size_t type.
 */
#define MEMBERSIZE(type, member) (ARRAYSIZE(((type *)0)->member))

/**
 * Use this macro for unused parameters right in the beginning of a function
 * body to suppress compiler warnings about unused parameters. This is mainly
 * meant for function parameters and not for unused local variables.
 */
#define UNREFERENCED_PARAM(param) \
    ((void)(0 ? ((param) = (param)) : (param)))

/**
 * Convert to string in preprocessor.
 * */
#define _STR(x) #x
#define STR(x) _STR(x)

/**
 * Formatted output conversion, similar to snprintf with logged error handling.
 * The functions will write at most size-1 of the characters printed into the
 * output string (the size'th character then gets the terminating `\0'); if the
 * the return value is greater than or equal to the size argument, the string
 * was too short and some of the printed characters were discarded. The output
 * is always null-terminated, unless size is 0.
 * @param str Output buffer.
 * @param len Maximum number of bytes to write.
 * @param fmt Format string, similar to snprintf.
 * @return the number of characters that would have been printed if the size
 * were unlimited, not including the final `\0'. Returns a negative value if an
 * error occurs.
 */
int print_to_string(char *str, size_t len, const char *fmt, ...);

/**
 * Better version of strerror_r inspired by the article:
 *   http://www.club.cc.cmu.edu/~cmccabe/blog_strerror.html
 * @param err errno.
 * @param str Points to a starting byte allowed to print.
 * @param len Maximum amount of bytes to write.
 * @return Pointer to formatted error string.
 */
const char *strerror_r_improved(int err, char *str, size_t len);

/**
 * Returns pointer to a string with a timestamp formatted similar to:
 *   Mon Feb 17 13:22:05 2020
 * @param str Points to a starting byte allowed to print.
 * @param len Maximum amount of bytes to write.
 * @return Pointer to formatted timestamp, 'error' string on error.
 */
const char *humanized_timestamp(char *str, size_t len);

#endif  // CBASE_BASE_H
