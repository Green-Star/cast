#pragma once

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "common.h"

struct cast_receiver {
  struct cast_file file;
  int filefd;
  int sockfd;
  int *upload_percentage;
};

  
void * cast_receiver(void *_arg);
