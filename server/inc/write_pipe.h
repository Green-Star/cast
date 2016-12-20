#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

#include "common.h"

ssize_t write_pipe(int const pipe_fd, char const * const fmt, ...) __attribute__ ((format(printf, 2, 3)));

ssize_t read_pipe(int const pipe_fd, char * const buf);
