#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "common.h"

bool cast_file(int _sockfd, struct cast_file _file);
