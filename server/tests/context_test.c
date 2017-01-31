#include "context.h"

#include <stdlib.h>

int main(int argc, char **argv) {
  struct context c;
  char input[PIPE_BUF];
  char *command, *argument;

  init_context(VLC, &c, STDIN_FILENO, STDOUT_FILENO, );

  /* TODO : Interface between WebUI and VLC */
  int parameter;
  int pipefd;

  while (1) {
    read_pipe(pipefd, input);
    parse_input(input, command, argument);

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
    else if (strcmp(command, "shutdown") == 0) {
      shutdown(&c);
      break;
    }
    else if (strcmp(command, "volume_up") == 0) {
      printf("TODO\n");
    }
    else if (strcmp(command, "volume_down") == 0) {
      printf("TODO\n");
    }
  }    
    

  
  return EXIT_SUCCESS;
}
