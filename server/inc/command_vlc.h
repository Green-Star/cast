#pragma once

#include <limits.h>
#include "common.h"
#include "write_pipe.h"

#define LANGUAGE_SIZE 20

struct track{
  int id;
  char language[LANGUAGE_SIZE];
} ;


ssize_t init_vlc(int _writefd, int _readfd);

ssize_t pause_vlc(int _writefd);
ssize_t play_vlc(int _writefd);
ssize_t stop_vlc(int _writefd);
ssize_t repeat_vlc(int _writefd, bool _repeat);
ssize_t set_fullscreen_vlc(int _writefd);

ssize_t get_length_vlc(int _writefd, int _readfd, int *_length);
ssize_t get_time_vlc(int _writefd, int _readfd, int *_time);
ssize_t set_time_vlc(int _writefd, int _new_time);

ssize_t get_volume_vlc(int _writefd, int _readfd, int *_volume);
ssize_t set_volume_vlc(int _writefd, int _volume);
ssize_t mute(int _writefd);

ssize_t get_video_track_vlc(int _writefd, int _readfd, struct track *_video_tracks);
ssize_t set_video_track_vlc(int _writefd, int _id_video_track);
ssize_t get_audio_track_vlc(int _writefd, int _readfd, struct track *_audio_tracks);
ssize_t set_audio_track_vlc(int _writefd, int _id_audio_track);
ssize_t get_subtitles_track_vlc(int _writefd, int _readfd, struct track *_subtitles_tracks);
ssize_t set_subtitles_track_vlc(int _writefd, int _id_subtitles_track);
