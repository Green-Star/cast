#pragma once

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "common.h"

bool get_file_name(char *_path, char *_file_name, int *_file_name_length);
bool get_file_length(char *_path, long *_file_length);
