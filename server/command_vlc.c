#include "command_vlc.h"

bool init_vlc(int _readfd, int _writefd) {
  char buf[PIPE_BUF];

  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }
  return true;
}

bool get_time_vlc(int _readfd, int _writefd, int *_new_time) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "get_time\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  *_new_time = strtol(buf, NULL, 10);
  return true;
}
  
bool set_time_vlc(int _readfd, int _writefd, int _time) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "seek %d\n", _time) < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool get_length_vlc(int _readfd, int _writefd, int *_new_length) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "get_length\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  *_new_length = strtol(buf, NULL, 10);
  return true;
}

bool get_volume_vlc(int _readfd, int _writefd, int *_new_volume) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "volume\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  *_new_volume = strtol(buf, NULL, 10);
  return true;
}

bool set_volume_vlc(int _readfd, int _writefd, int _volume) {
  char buf[PIPE_BUF];
  
  if (write_pipe(_writefd, "volume %d\n", _volume) < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool get_video_tracks_vlc(int _readfd, int _writefd, int *_nb_video_tracks, struct track **_new_video_tracks);
bool set_video_track_vlc(int _readfd, int _writefd, struct track _video_track) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "vtrack %d\n", _video_track.id) < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool get_audio_tracks_vlc(int _reafd, int _writefd, struct track **_new_audio_tracks);
bool set_audio_track_vlc(int _readfd, int _writefd, struct track _audio_track) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "atrack %d\n", _audio_track.id) < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool get_subtitles_tracks_vlc(int _reafd, int _writefd, struct track **_new_subtitles_tracks);
bool set_subtitles_track_vlc(int _readfd, int _writefd, struct track _subtitles_track) {
  char buf[PIPE_BUF];
  
  if (write_pipe(_writefd, "strack %d\n", _subtitles_track.id) < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool pause_vlc(int _readfd, int _writefd) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "pause\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool play_vlc(int _readfd, int _writefd) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "play\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool stop_vlc(int _readfd, int _writefd) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "stop\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool shutdown_vlc(int _readfd, int _writefd) {
    char buf[PIPE_BUF];

  if (write_pipe(_writefd, "shutdown\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}

bool set_fullscreen_vlc(int _readfd, int _writefd) {
    char buf[PIPE_BUF];

  if (write_pipe(_writefd, "fullscreen\n") < 0) {
    return false;
  }
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  return true;
}



/*
ssize_t init_vlc(int _writefd, int _readfd) {
  char buf[PIPE_BUF];

  return read_pipe(_readfd, buf);
}

ssize_t pause_vlc(int _writefd) {
  return write_pipe(_writefd, "pause\n");
}

ssize_t play_vlc(int _writefd) {
  return write_pipe(_writefd, "play\n");
}

ssize_t stop_vlc(int _writefd) {
  return write_pipe(_writefd, "stop\n");
}

ssize_t shutdown_vlc(int _writefd) {
  return write_pipe(_writefd, "shutdown\n");
}

ssize_t repeat_vlc(int _writefd, bool _repeat) {
  if (_repeat == true) {
    return write_pipe(_writefd, "repeat on\n");
  } else {
    return write_pipe(_writefd, "repeat off\n");
  }
}

ssize_t set_fullscreen_vlc(int _writefd) {
  return write_pipe(_writefd, "fullscreen\n");
}

ssize_t get_length_vlc(int _writefd, int _readfd, int *_length) {
  char buf[PIPE_BUF];
  
  write_pipe(_writefd, "get_length\n");
  read_pipe(_readfd, (void*)buf);
  *_length = strtol(buf, NULL, 10);
}

ssize_t get_time_vlc(int _writefd, int _readfd, int *_time) {
  char buf[PIPE_BUF];
  
  write_pipe(_writefd, "get_time\n");
  read_pipe(_readfd, buf);
  *_time = strtol(buf, NULL, 10);
}

ssize_t set_time_vlc(int _writefd, int _new_time) {
  return write_pipe(_writefd, "seek %d\n", _new_time);
}

ssize_t get_volume_vlc(int _writefd, int _readfd, int *_volume) {
  char buf[PIPE_BUF];

  write_pipe(_writefd, "volume\n");
  read_pipe(_readfd, buf);
  *_volume = strtol(buf, NULL, 10);
}

ssize_t set_volume_vlc(int _writefd, int _volume) {
  write_pipe(_writefd, "volume %d\n", _volume);
}

ssize_t mute(int _writefd) {
  return set_volume_vlc(_writefd, 0);
}

ssize_t get_video_track_vlc(int _writefd, int _readfd, struct track *_video_tracks);
ssize_t set_video_track_vlc(int _writefd, int _id_video_track) {
  return write_pipe(_writefd, "vtrack %d\n", _id_video_track);
}

ssize_t get_audio_track_vlc(int _writefd, int _readfd, struct track *_audio_tracks);

ssize_t set_audio_track_vlc(int _writefd, int _id_audio_track) {
  return write_pipe(_writefd, "atrack %d\n", _id_audio_track);
}

ssize_t get_subtitles_track_vlc(int _writefd, int _readfd, struct track *_subtitles_tracks);
ssize_t set_subtitles_track_vlc(int _writefd, int _id_subtitles_track) {
  return write_pipe(_writefd, "strack %d\n", _id_subtitles_track);
}
*/
