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

#include "context.h"

void * cast_streaming(void *_arg);
void start_webUI(int *_parent_to_UI_pipe, int *_UI_to_parent_pipe);
