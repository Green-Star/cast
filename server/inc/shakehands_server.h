#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#include "common.h"

struct cast_file_server {
  struct cast_file file;
  bool upload;
  int fd;
};

bool shakehands_server(int sockfd, struct cast_file_server *_info);
bool search_file(char *_path, long _file_length);
