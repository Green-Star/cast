#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"

void shakehands_client(int _sockfd, struct cast_file _file_to_send);
