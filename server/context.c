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

  (*_context->init_player)(_context->_readfd, _context->writefd);
}
