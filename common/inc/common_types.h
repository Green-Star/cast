#pragma once

#include <stdio.h>

#include "common_defines.h"

typedef enum {false = 0, true = 1} bool;

struct cast_file {
  FILE *file;
  long file_length;
  char file_name[FILE_LENGTH];
  int file_name_length;
};
