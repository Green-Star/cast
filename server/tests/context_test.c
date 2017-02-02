#include "context.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  struct context c;
  char input[PIPE_BUF];
  char *command = NULL, *argument = NULL;
  int upload = 0;
  int parameter;

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
  else {
    /* We're closing the read end of the parent -> child pipe */
    close(parent_to_child_pipe[0]);
    /* and the write end of the child -> parent pipe */
    close(child_to_parent_pipe[1]);    

    int readfd = child_to_parent_pipe[0];
    int writefd = parent_to_child_pipe[1];

    init_context(VLC, &c, readfd, writefd, &upload);
    update_context_data(&c);

    while (1) {
      read_pipe(STDIN_FILENO, input);
      parse_input(input, &command, &argument);

      if (strcmp(command, "set_time") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	set_time(&c, parameter);
      }
      else if (strcmp(command, "pause") == 0) {
	set_pause(&c);
      }
      else if (strcmp(command, "play") == 0) {
	set_play(&c);
      }
      else if (strcmp(command, "stop") == 0) {
	stop(&c);
      }
      else if (strcmp(command, "add_time") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	add_time(&c, parameter);
      }
      else if (strcmp(command, "set_volume") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	set_volume(&c, parameter);
      }
      else if (strcmp(command, "add_volume") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	add_volume(&c, parameter);
      }
      else if (strcmp(command, "set_video_track") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	set_video_track(&c, parameter);
      }
      else if (strcmp(command, "set_audio_track") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	set_audio_track(&c, parameter);
      }
      else if (strcmp(command, "set_subtitles_track") == 0) {
	if (argument == NULL) {
	  continue;
	}
	parameter = strtol(argument, NULL, 10);
	set_subtitles_track(&c, parameter);
      }
      else if (strcmp(command, "volume_up") == 0) {
	printf("TODO\n");
      }
      else if (strcmp(command, "volume_down") == 0) {
	printf("TODO\n");
      }
      else if (strcmp(command, "shutdown") == 0) {
	shutdown_player(&c);
	/* Useless statement
	*(stream->requested_shutdown) = true;
	*/
	/* Kill WebUI process */
	break;
      }

      update_context_data(&c);
      write_pipe(STDOUT_FILENO, "%s\n", context_to_json(c));
    }    
  }
  waitpid(-1, NULL, 0);
  
  return EXIT_SUCCESS;
}
