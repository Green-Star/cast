#pragma once

#include <limits.h>
#include "common.h"
#include "server_types.h"
#include "write_pipe.h"

bool init_vlc(int _readfd, int _writefd);

bool get_time_vlc(int _readfd, int _writefd, int *_new_time);
bool set_time_vlc(int _readfd, int _writefd, int _time);

bool get_length_vlc(int _readfd, int _writefd, int *_new_length);

bool get_volume_vlc(int _readfd, int _writefd, int *_new_volume);
bool set_volume_vlc(int _readfd, int _writefd, int _volume);

bool get_video_tracks_vlc(int _readfd, int _writefd, int *_nb_video_tracks, struct track **_new_video_tracks);
bool set_video_track_vlc(int _readfd, int _writefd, struct track _video_track);

bool get_audio_tracks_vlc(int _readfd, int _writefd, int *_nb_audio_tracks, struct track **_new_audio_tracks);
bool set_audio_track_vlc(int _readfd, int _writefd, struct track _audio_track);

bool get_subtitles_tracks_vlc(int _readfd, int _writefd, int *_nb_subtitles_tracks, struct track **_new_subtitles_tracks);
bool set_subtitles_track_vlc(int _readfd, int _writefd, struct track _subtitles_track);

bool pause_vlc(int _readfd, int _writefd);
bool play_vlc(int _readfd, int _writefd);
bool stop_vlc(int _readfd, int _writefd);
bool shutdown_vlc(int _readfd, int _writefd);
bool set_fullscreen_vlc(int _readfd, int _writefd);
