#include "context.h"

bool init_context(int _context_type, struct context *_context, int _readfd, int _writefd, int *_upload_percentage) {
  _context->readfd = _readfd;
  _context->writefd = _writefd;
  _context->upload_percentage = _upload_percentage;
  _context->upload_complete = (*_upload_percentage == 100);
  _context->is_playing = false;
  _context->time = 0;
  _context->length = 0;
  _context->volume = 0;
  _context->nb_video_tracks = 0;
  _context->nb_audio_tracks = 0;
  _context->nb_subtitles_tracks = 0;
  _context->video_tracks = NULL;
  _context->audio_tracks = NULL;
  _context->subtitles_tracks = NULL;

  switch(_context_type) {
    case VLC:
    default:
      _context->init_player = init_vlc;
      _context->get_time = get_time_vlc;
      _context->set_time = set_time_vlc;
      _context->get_length = get_length_vlc;
      _context->get_volume = get_volume_vlc;
      _context->set_volume = set_volume_vlc;
      _context->get_video_tracks = get_video_tracks_vlc;
      _context->set_video_track = set_video_track_vlc;
      _context->get_audio_tracks = get_audio_tracks_vlc;
      _context->set_audio_track = set_audio_track_vlc;
      _context->get_subtitles_tracks = get_subtitles_tracks_vlc;
      _context->set_subtitles_track = set_subtitles_track_vlc;
      _context->pause = pause_vlc;
      _context->play = play_vlc;
      _context->stop = stop_vlc;
      _context->shutdown = shutdown_vlc;
      break;
  }

  return (*_context->init_player)(_context->readfd, _context->writefd);
}


bool get_time(struct context *_context) {
  return (*_context->get_time)(_context->readfd, _context->writefd, &(_context->time));
}

bool set_time(struct context *_context, int _time) {
  bool ret;

  ret = (*_context->set_time)(_context->readfd, _context->writefd, _time);
  if (ret == false) {
    return false;
  }

  return get_time(_context);
}

bool get_length(struct context *_context) {
  return (*_context->get_length)(_context->readfd, _context->writefd, &(_context->length));
}

bool get_volume(struct context *_context) {
  return (*_context->get_volume)(_context->readfd, _context->writefd, &(_context->volume));
}

bool set_volume(struct context *_context, int _volume) {
  bool ret;
  
  ret = (*_context->set_volume)(_context->readfd, _context->writefd, _volume);
  if (ret == false) {
    return false;
  }

  return get_volume(_context);
}

bool get_video_tracks(struct context *_context) {
  return (*_context->get_video_tracks)(_context->readfd,
				       _context->writefd,
				       &(_context->nb_video_tracks),
				       &(_context->video_tracks));
}

bool set_video_track(struct context *_context, int _trackid) {
  struct track *track;
  int i;
  bool ret;

  track = NULL;
  for (i = 0; i < _context->nb_video_tracks; i++) {
    if (_context->video_tracks[i].id == _trackid) {
      track = &(_context->video_tracks[i]);
      break;
    }
  }

  /* No track with the given _trackid was found, nothing to do */
  if (track == NULL) {
    return false;
  }

  ret = (*_context->set_video_track)(_context->readfd, _context->writefd, *track);
  if (ret == false) {
    return false;
  }

  /* Update context data */
  ret = get_video_tracks(_context);
  return ret;
}

bool get_audio_tracks(struct context *_context) {
  return (*_context->get_audio_tracks)(_context->readfd,
				       _context->writefd,
				       &(_context->nb_audio_tracks),
				       &(_context->audio_tracks));
}

bool set_audio_track(struct context *_context, int _trackid) {
  struct track *track;
  int i;
  bool ret;

  track = NULL;
  for (i = 0; i < _context->nb_audio_tracks; i++) {
    if (_context->audio_tracks[i].id == _trackid) {
      track = &(_context->audio_tracks[i]);
      break;
    }
  }

  /* No track with the given _trackid was found, nothing to do */
  if (track == NULL) {
    return false;
  }
  
  ret = (*_context->set_audio_track)(_context->readfd, _context->writefd, *track);
  if (ret == false) {
    return false;
  }

  /* Update context data */
  ret = get_audio_tracks(_context);
  return ret;
}

bool get_subtitles_tracks(struct context *_context) {
  return (*_context->get_subtitles_tracks)(_context->readfd,
					   _context->writefd,
					   &(_context->nb_subtitles_tracks),
					   &(_context->subtitles_tracks));
}

bool set_subtitles_track(struct context *_context, int _trackid) {
  struct track *track;
  int i;
  bool ret;

  track = NULL;
  for (i = 0; i < _context->nb_subtitles_tracks; i++) {
    if (_context->subtitles_tracks[i].id == _trackid) {
      track = &(_context->subtitles_tracks[i]);
      break;
    }
  }

  /* No track with the given _trackid was found, nothing to do */
  if (track == NULL) {
    return false;
  }

  ret = (*_context->set_subtitles_track)(_context->readfd, _context->writefd, *track);
  if (ret == false) {
    return false;
  }

  /* Update context data */
  ret = get_subtitles_tracks(_context);
  return ret;
}

bool set_pause(struct context *_context) {
  bool ret;

  ret = (*_context->pause)(_context->readfd, _context->writefd);
  if (ret == true) {
    _context->is_playing = false;
  }
  
  return ret;
}

bool set_play(struct context *_context) {
  bool ret;
  
  ret = (*_context->play)(_context->readfd, _context->writefd);
  if (ret == true) {
    _context->is_playing = true;
  }

  return ret;
}

bool stop(struct context *_context) {
  return (*_context->stop)(_context->readfd, _context->writefd);
}

bool shutdown(struct context *_context) {
  return (*_context->shutdown)(_context->readfd, _context->writefd);
}

char *context_to_json(struct context _context) {
  static char json_string[PIPE_BUF];
  char tmp[PIPE_BUF];

  memset((void*)json_string, 0, sizeof(json_string));

  sprintf(json_string, "{\"upload_progress\":%d,\"upload_complete\":%s,\"is_playing\":%s,\"time\":%d,\"length\":%d,\"volume\":%d,",
	  *(_context.upload_percentage),
	  bool_to_json(_context.upload_complete),
	  bool_to_json(_context.is_playing),
	  _context.time,
	  _context.length,
	  _context.volume);

  strcpy(tmp, "\"video_tracks\":[");
  strcat(tmp, tracks_to_json(_context.nb_video_tracks, _context.video_tracks));
  strcat(tmp, "],");
  strcat(json_string, tmp);

  strcpy(tmp, "\"audio_tracks\":[");
  strcat(tmp, tracks_to_json(_context.nb_audio_tracks, _context.audio_tracks));
  strcat(tmp, "],");
  strcat(json_string, tmp);

  strcpy(tmp, "\"subtitles_tracks\":[");
  strcat(tmp, tracks_to_json(_context.nb_subtitles_tracks, _context.subtitles_tracks));
  strcat(tmp, "]}");
  strcat(json_string, tmp);

  return json_string;
}

char *tracks_to_json(int nb_tracks, struct track *_tracks) {
  static char json_string[PIPE_BUF];
  char tmp[PIPE_BUF];
  int i;

  memset((void*)json_string, 0, sizeof(json_string));

  for (i = 0; i < nb_tracks; i++) {
    if (i > 0) {
      strcat(json_string, ",");
    }
    strcpy(tmp, track_to_json(_tracks[i]));
    strcat(json_string, tmp);
  }
  
  return json_string;
}

char *track_to_json(struct track _track) {
  static char json_string[PIPE_BUF];

  memset((void*)json_string, 0, sizeof(json_string));
  
  sprintf(json_string, "{\"id\":%d,\"name\":\"%s\",\"language\":\"%s\",\"selected\":%s}",
	  _track.id,
	  _track.name,
	  _track.language,
	  bool_to_json(_track.selected));

  return json_string;
}
