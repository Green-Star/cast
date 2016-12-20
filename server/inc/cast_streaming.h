#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "server_types.h"

#include "write_pipe.h"
#include "command_vlc.h"

void * cast_streaming(void *_arg);
