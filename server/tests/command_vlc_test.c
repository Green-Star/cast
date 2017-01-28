#include "sys/types.h"
#include "sys/wait.h"
#include "command_vlc.h"

void print_tracks(int readfd, int writefd,
		  int *nb_video_tracks, struct track **video_tracks,
		  int *nb_audio_tracks, struct track **audio_tracks,
		  int *nb_subtitles_tracks, struct track **subtitles_tracks) {
  printf("*** Videos tracks ***\n");
  get_video_tracks_vlc(readfd, writefd, nb_video_tracks, video_tracks);
  for (int i = 0; i < *nb_video_tracks; i++) {
    printf("{\"id\":%d,\"name\":\"%s\",\"language\":\"%s\",\"selected\":%s}\n", (*video_tracks)[i].id, (*video_tracks)[i].name, (*video_tracks)[i].language, ((*video_tracks)[i].selected) ? "true" : "false");
  }
  sleep(1);
  printf("*** Audio tracks ***\n");
  get_audio_tracks_vlc(readfd, writefd, nb_audio_tracks, audio_tracks);
  for (int i = 0; i < *nb_audio_tracks; i++) {
    printf("{\"id\":%d,\"name\":\"%s\",\"language\":\"%s\",\"selected\":%s}\n", (*audio_tracks)[i].id, (*audio_tracks)[i].name, (*audio_tracks)[i].language, ((*audio_tracks)[i].selected) ? "true" : "false");
  }
  sleep(1);
  printf("*** Subtitles tracks ***\n");
  get_subtitles_tracks_vlc(readfd, writefd, nb_subtitles_tracks, subtitles_tracks);
  for (int i = 0; i < *nb_subtitles_tracks; i++) {
    printf("{\"id\":%d,\"name\":\"%s\",\"language\":\"%s\",\"selected\":%s}\n", (*subtitles_tracks)[i].id, (*subtitles_tracks)[i].name, (*subtitles_tracks)[i].language, ((*subtitles_tracks)[i].selected) ? "true" : "false");
  }
  sleep(1);  
}

int main(int argc, char **argv) {
  int parent_to_child_pipe[2], child_to_parent_pipe[2];
  pid_t pid;
  FILE *new_stderr;
  
  /*
    We're going to need 2 pipes : 
     - 1 pipe for the parent -> child communication
     - 1 pipe for the child -> parent communication
  */
  /* 1st pipe : parent -> child communication */
  if (pipe(parent_to_child_pipe) == -1) {
    handle_pipe_error();
    return EXIT_FAILURE;
  }

  /* 2nd pipe : child -> parent communication */
  if (pipe(child_to_parent_pipe) == -1) {
    handle_pipe_error();
    return EXIT_FAILURE;
  }

  pid = fork();
  if (pid == -1) {
    handle_fork_error();
    return EXIT_FAILURE;
  } 
  if (pid == 0) {
    /* Child code */
    /* We're closing the write end of the parent -> child pipe */
    close(parent_to_child_pipe[1]);
    /* and the read end of the child -> parent pipe */
    close(child_to_parent_pipe[0]);
    
    /* We pipe stdin onto the read end of the parent -> child pipe */    
    if (dup2(parent_to_child_pipe[0], STDIN_FILENO) < 0) {
      print_error("dup2");
      return EXIT_FAILURE;
    }

    /* We pipe stdout onto the write end of the child -> parent pipe */      
    if (dup2(child_to_parent_pipe[1], STDOUT_FILENO) < 0) {
      print_error("dup2");
      return EXIT_FAILURE;
    }
    
    /* Redirect stdout and stderr on file */
    new_stderr = fopen("./stderr.log", "w");
    dup2(fileno(new_stderr), STDERR_FILENO);
    fclose(new_stderr);
    
    /* Starting VLC */
    execlp("vlc", "vlc", "-I", "rc", argv[1], (char*) NULL);
    handle_execlp_error();
    return EXIT_FAILURE;
  }
  /* Parent process */
  else {
    /* We're closing the read end of the parent -> child pipe */
    close(parent_to_child_pipe[0]);
    /* and the write end of the child -> parent pipe */
    close(child_to_parent_pipe[1]);    

    int readfd = child_to_parent_pipe[0];
    int writefd = parent_to_child_pipe[1];
    
    int time;
    int length;
    int volume;
    
    int nb_video_tracks = 0;
    struct track *video_tracks = NULL;
    int nb_audio_tracks = 0;
    struct track *audio_tracks = NULL;
    int nb_subtitles_tracks = 0;
    struct track *subtitles_tracks = NULL;
    
    init_vlc(readfd, writefd);

    print_tracks(readfd, writefd,
		  &nb_video_tracks, &video_tracks,
		  &nb_audio_tracks, &audio_tracks,
		  &nb_subtitles_tracks, &subtitles_tracks);

    get_length_vlc(readfd, writefd, &length);
    printf("length : %d\n", length);
    sleep(1);
    
    get_time_vlc(readfd, writefd, &time);
    printf("time : %d\n", time);
    sleep(1);
        
    play_vlc(readfd, writefd);
    set_time_vlc(readfd, writefd, 60);
    get_time_vlc(readfd, writefd, &time);
    printf("time : %d\n", time);
    sleep(1);

    set_video_track_vlc(readfd, writefd, video_tracks[0]);
    set_audio_track_vlc(readfd, writefd, audio_tracks[nb_audio_tracks - 1]);
    set_subtitles_track_vlc(readfd, writefd, subtitles_tracks[nb_subtitles_tracks - 1]);

    print_tracks(readfd, writefd,
		  &nb_video_tracks, &video_tracks,
		  &nb_audio_tracks, &audio_tracks,
		  &nb_subtitles_tracks, &subtitles_tracks);

    sleep(3);
    
    set_video_track_vlc(readfd, writefd, video_tracks[1]);
    print_tracks(readfd, writefd,
		  &nb_video_tracks, &video_tracks,
		  &nb_audio_tracks, &audio_tracks,
		  &nb_subtitles_tracks, &subtitles_tracks);
        
    get_volume_vlc(readfd, writefd, &volume);
    printf("volume : %d\n", volume);
    sleep(1);
    set_volume_vlc(readfd, writefd, 0);
    get_volume_vlc(readfd, writefd, &volume);
    printf("volume : %d\n", volume);
    sleep(1);

    pause_vlc(readfd, writefd);
    sleep(2);

    set_volume_vlc(readfd, writefd, 175);
    get_volume_vlc(readfd, writefd, &volume);
    printf("volume : %d\n", volume);
    sleep(1);

    play_vlc(readfd, writefd);
    
    sleep(10);
    
    shutdown_vlc(readfd, writefd);
    waitpid(-1, NULL, 0);
  } 
  
  return EXIT_SUCCESS;
}
