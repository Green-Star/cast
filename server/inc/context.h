#pragma once

#include "common.h"
#include "server_types.h"
#include "command_vlc.h"

#include <string.h>

#define VLC 1

bool init_context(int _context_type, struct context *_context, int _readfd, int _writefd, int *_upload_percentage);

bool get_time(struct context *_context);
bool set_time(struct context *_context, int _time);
bool add_time(struct context *_context, int _add);
bool get_length(struct context *_context);
bool get_volume(struct context *_context);
bool set_volume(struct context *_context, int _volume);
bool add_volume(struct context *_context, int _add);
bool get_video_tracks(struct context *_context);
bool set_video_track(struct context *_context, int _trackid);
bool get_audio_tracks(struct context *_context);
bool set_audio_track(struct context *_context, int _trackid);
bool get_subtitles_tracks(struct context *_context);
bool set_subtitles_track(struct context *_context, int _trackid);
bool set_pause(struct context *_context);
bool set_play(struct context *_context);
bool stop(struct context *_context);
bool shutdown_player(struct context *_context);

void update_context_data(struct context *_context);

char *context_to_json(struct context _context);
char *tracks_to_json(int _nb_tracks, struct track *_tracks);
char *track_to_json(struct track _track);

static inline char *bool_to_json(bool b) {
  return (b) ? "true" : "false";
}

bool parse_input(char *_input, char **_command, char **_argument);
