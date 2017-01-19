#pragma once

struct cast_receiver {
  struct cast_file file;
  int filefd;
  int sockfd;
  int *upload_percentage;
};

#define LANGUAGE_SIZE 100
#define NAME_SIZE 20

struct track{
  int id;
  char name[NAME_SIZE];
  char language[LANGUAGE_SIZE];
  bool selected;
} ;

struct context {
  int readfd;
  int writefd;
  
  int *upload_percentage;
  bool upload_complete;
  bool is_playing;
  int time;
  int length;
  int volume;
  int nb_video_tracks;
  struct track *video_tracks;
  int nb_audio_tracks;
  struct track *audio_tracks;
  int nb_subtitles_tracks;
  struct track *subtitles_tracks;

  bool (*init_player)(int _readfd, int _writefd);
  bool (*get_time)(int _readfd, int _writefd, int *_new_time);
  bool (*set_time)(int _readfd, int _writefd, int _time);
  bool (*get_length)(int _readfd, int _writefd, int *_new_length);
  bool (*get_volume)(int _readfd, int _writefd, int *_new_volume);
  bool (*set_volume)(int _readfd, int _writefd, int _volume);
  bool (*get_video_tracks)(int _readfd, int _writefd, int *_nb_video_tracks, struct track **_new_video_tracks);
  bool (*set_video_track)(int _readfd, int _writefd, struct track _video_track);
  bool (*get_audio_tracks)(int _reafd, int _writefd, struct track **_new_audio_tracks);
  bool (*set_audio_track)(int _readfd, int _writefd, struct track _audio_track);
  bool (*get_subtitles_tracks)(int _reafd, int _writefd, struct track **_new_subtitles_tracks);
  bool (*set_subtitles_track)(int _readfd, int _writefd, struct track _subtitles_track);
  bool (*pause)(int _readfd, int _writefd);
  bool (*play)(int _readfd, int _writefd);
  bool (*stop)(int _readfd, int _writefd);
  bool (*shutdown)(int _readfd, int _writefd);
};

  
