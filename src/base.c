// SPDX-FileCopyrightText: 2025 David Rabkin
// SPDX-License-Identifier: 0BSD

#include "base.h"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>  // va_list
#include <stdio.h>  // vsnprintf
#include <string.h>  // strerror_r
#include <time.h>

int print_to_string(char *str, size_t len, const char *fmt, ...) {
    assert(str != NULL);
    assert(fmt != NULL);
    va_list ap;
    va_start(ap, fmt);
    int num = vsnprintf(str, len, fmt, ap);
    va_end(ap);
    if (num < 0) {
        // char buf[BUFSIZ];
        // LOGE("Unable to print %s: %s.",
        //     fmt, strerror_r_improved(errno, buf, ARRAYSIZE(buf)));
        return -1;
    }
    // Part of the string was written, the output of snprintf is always null
    // terminated and can be printed for debug.
    if ((size_t)num >= len) {
        // LOGE("Buffer %zu is smaller than needed %d: %s.",
        //     len, num + 1, str);
        // Cancels partially printed data.
        str[0] = '\0';
        return -1;
    }
    assert('\0' == str[num] && "Result should be null terminated.");
    return num;
}

#if defined(_GNU_SOURCE) && !defined(__APPLE__)
const char *strerror_r_improved(int err, char *str, size_t len) {
    return strerror_r(err, str, len);
}
#else
const char *strerror_r_improved(int err, char *str, size_t len) {
    const char *unknown = "unknown error";
    int rc = strerror_r(err, str, len);
    if (rc != 0) {
        rc = print_to_string(str, len, "%s %d", unknown, err);
    }
    return rc >= 0 ? str : unknown;
}
#endif

const char *humanized_timestamp(char *str, size_t len) {
    const char *err = "error";
    struct tm tm;
    time_t t = time(NULL);
    const struct tm *ptm = localtime_r(&t, &tm);
    if (NULL == ptm) {
        // char buf[BUFSIZ];
        // LOGE("Unable to read localtime: %s.",
        //     strerror_r_improved(errno, buf, ARRAYSIZE(buf)));
        return err;
    }
    size_t bytes = strftime(str, len, "%c", ptm);
    if (0 == bytes) {
        // strftime doesn't set errno.
        // LOGE("Unable to print time to a buffer of size %zu.", len);
        return err;
    }
    assert(strlen(str) == bytes);
    return str;
}
