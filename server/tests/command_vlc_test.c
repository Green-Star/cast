#include "sys/types.h"
#include "sys/wait.h"
#include "command_vlc.h"
  
int main(int argc, char **argv) {
  int parent_to_child_pipe[2], child_to_parent_pipe[2];
  pid_t pid;
  FILE *new_stdout, *new_stderr;
  
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

    printf("fork to pid [%d]\n", getpid());
    
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
    /*
    new_stdout = fopen("./stdout.txt", "w");
    dup2(fileno(new_stdout), STDOUT_FILENO);
    fclose(new_stdout);
    */

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

    
    
    /* TODO : Try communication with VLC */
    char in[100];
    char dummy[100];

    int readfd = child_to_parent_pipe[0];
    int writefd = parent_to_child_pipe[1];
    
    bool t;
    t = true;
    
    int nb_video_tracks = 0;
    struct track *video_tracks = NULL;
    int nb_audio_tracks = 0;
    struct track *audio_tracks = NULL;
    int nb_subtitles_tracks = 0;
    struct track *subtitles_tracks = NULL;
    

    if (init_vlc(readfd, writefd) == false)
    {
      printf("false\n");
    }
    else
    {
      printf("true\n");
    }

    get_video_tracks_vlc(readfd, writefd, &nb_video_tracks, &video_tracks);
    sleep(1);
    get_video_tracks_vlc(readfd, writefd, NULL, NULL);

    do {
      fscanf(stdin, "%s", in);

      if (strcmp(in, "get_video_track\n") == 0) {
	get_video_tracks_vlc(readfd, writefd, NULL, NULL);
      }
      
      if (strcmp(in, "pause") == 0) {
	write_pipe(parent_to_child_pipe[1], "pause\n");
	//read_pipe(child_to_parent_pipe[0], dummy); 
	//printf("%s", dummy);
      }
      else if(strcmp(in, "play") == 0) {
	write_pipe(parent_to_child_pipe[1], "play\n");
	//read_pipe(child_to_parent_pipe[0], dummy);
	//printf("%s", dummy);
      }
      else if (strcmp(in, "quit") == 0 || strcmp(in, "shutdown") == 0) {
	write_pipe(parent_to_child_pipe[1], "shutdown\n");
	t =false;
      }
      in[0] = 0;
    
    } while(t);
    
    
    waitpid(-1, NULL, 0);
    return EXIT_SUCCESS;
  } 
  
  return EXIT_SUCCESS;
}
