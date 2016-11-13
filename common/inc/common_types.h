#pragma once

#include <stdio.h>

#include "common_defines.h"

struct cast_file {
  FILE *file;
  long file_length;
  char file_name[FILE_LENGTH];
};
