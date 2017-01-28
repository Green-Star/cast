#include "command_vlc.h"

int get_nb_tracks_vlc(char *_string_tracks) {
  char *s = _string_tracks;
  int nb_tracks = 0;
  bool new_line = true;

  while (*s) {
    /* Tracks description in VLC starts with '|' */
    if (*s == '|' && new_line == true) {
      nb_tracks++;      
    }
    
    if (*s == '\n') {
      new_line = true;
    }
    else {
      new_line = false;
    }
    
    s++;
  }

  return nb_tracks;
}

int parse_one_track_vlc(char *_string_track, struct track *_track) {
  char *s = _string_track;
  char *ptr, *end_line;
  char id_buffer[LANGUAGE_SIZE];
  int nb_chars = 0;
  int offset;

  memset((void*)_track, 0, sizeof(struct track));
  memset((void*)id_buffer, 0, sizeof(id_buffer));

  /* Parse only one line */
  end_line = strstr(s, "\n");
  *end_line = 0;
  
  /* First character of s is the beginning '|'. Just ignore it */
  s++;
  nb_chars++;

  /* Get track id */
  offset = 0;
  ptr = strstr(s, " - ");
  while (s != ptr) {
    id_buffer[offset] = *s;
    offset++;
    nb_chars++;
    s++;
  }
  _track->id = strtol(id_buffer, NULL, 10);

  /* Skip " - " sub-string */
  s += 3;
  nb_chars += 3;

  ptr = strstr(s, " - [");
  /* Here we have 2 possibilities : 
     -> If ptr is not NULL then we have a line with the following format : 
     	NAME - [LANGUAGE]\r 
     or
	NAME - [LANGUAGE] *\r

     -> If ptr is NULL then we have a line with the following format :
     	NAME\r
     or
     	NAME *\r
  */
  if (ptr != NULL) {
    /* Get track name */
    offset = 0;
    while (s != ptr) {
      _track->name[offset] = *s;
      offset++;
      nb_chars++;
      s++;
    }
    /* Skip " - [" */
    nb_chars += 4;
    s += 4;
    
    /* Get track language */
    offset = 0;
    while (*s != ']') {
      _track->language[offset] = *s;
      offset++;
      nb_chars++;
      s++;
    }

    /* Skip "]" */
    nb_chars++;
    s++;

    /* Get selected or not */
    if (strncmp(s, " *", 2) == 0) {
      _track->selected = true;

      nb_chars += 2;
      s += 2;
    }
    else {
      _track->selected = false;
    }
  }
  else {
    /* Seek for " *" sub-string */
    ptr = strstr(s, " *");
    offset = 0;

    /* If the sub-string exists, we can copy the track name and set the selected flag */
    if (ptr != NULL) {
      while (s != ptr) {
	_track->name[offset] = *s;
	offset++;
	nb_chars++;
	s++;
      }

      _track->language[0] = 0;
      _track->selected = true;

      nb_chars += 2;
      s += 2;
    }
    /* If the sub-string does not exist, we just copy the track name */
    else {
      while (*s != '\r') {
	_track->name[offset] = *s;
	offset++;
	nb_chars++;
	s++;
      }

      _track->language[0] = 0;
      _track->selected = false;
    }
  }

  /* Skip '\r' */
  s++;
  nb_chars++;

  /* Reset s in the previous state */
  *end_line = '\n';
  
  return nb_chars;
}

bool parse_tracks_vlc(char *_string_tracks, int *_nb_tracks, struct track **_tracks) {
  bool new_line;
  int nb_tracks, current_track, offset;
  char *s;
  struct track *tracks;

  /* Get number of tracks in the film */
  nb_tracks = get_nb_tracks_vlc(_string_tracks);

  /* Realloc the _tracks structure */
  tracks = (struct track *)realloc((void*)*_tracks, nb_tracks * sizeof(struct track));
  if (tracks == NULL) {
    fprintf(stderr, "Realloc of %d tracks failed !\n", nb_tracks);
    return false;
  }

  s = _string_tracks;
  new_line = true;
  current_track = 0;
  
  while (*s) {
    if (*s == '|' && new_line == true) {
      offset = parse_one_track_vlc(s, &(tracks[current_track]));
      current_track++;
      s += offset;
      continue;
    }
    else if (*s == '\n') {
      new_line = true;
    }
    else {
      new_line = false;
    }
    
    s++;
  }

  *_nb_tracks = nb_tracks;
  *_tracks = tracks;
  
  return true;
}

bool init_vlc(int _readfd, int _writefd) {
  char buf[PIPE_BUF];

  /* Give some time to VLC to let it initialize its part  */
  sleep(1);
  /* Get VLC starting messages */
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  /* Pause VLC */
  if (write_pipe(_writefd, "pause\n") < 0) {
    return false;
  }

  /* Toggle fullscreen */
  if (write_pipe(_writefd, "fullscreen\n") < 0) {
    return false;
  }

  /* Get newlines print by VLC after the two previous commands */
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  /* Give VLC some extra time */
  sleep(1);
  
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

bool get_video_tracks_vlc(int _readfd, int _writefd, int *_nb_video_tracks, struct track **_new_video_tracks) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "vtrack\n") < 0) {
    return false;
  }
  
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  if (parse_tracks_vlc(buf, _nb_video_tracks, _new_video_tracks) == false)
  {
    return false;
  }
  
  return true;
}
  
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

bool get_audio_tracks_vlc(int _readfd, int _writefd, int *_nb_audio_tracks, struct track **_new_audio_tracks) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "atrack\n") < 0) {
    return false;
  }
  
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }
  
  if (parse_tracks_vlc(buf, _nb_audio_tracks, _new_audio_tracks) == false) {
    return false;
  }

  return true;
}

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

bool get_subtitles_tracks_vlc(int _readfd, int _writefd, int *_nb_subtitles_tracks, struct track **_new_subtitles_tracks) {
  char buf[PIPE_BUF];

  if (write_pipe(_writefd, "strack\n") < 0) {
    return false;
  }
  
  if (read_pipe(_readfd, buf) < 0) {
    return false;
  }

  if (parse_tracks_vlc(buf, _nb_subtitles_tracks, _new_subtitles_tracks) == false)
  {
    return false;
  }
  
  return true;
}

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
