#pragma once

#include "common.h"
#include "server_types.h"
#include "command_vlc.h"

#define VLC 1

bool init_context(int _context_type, struct context *_context, int _readfd, int _writefd, int *_upload_percentage);

char *context_to_json(struct context _context);

char *track_to_json(struct track _track);
