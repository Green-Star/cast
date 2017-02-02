#include "cast_streaming.h"

void* start_display_writer(void *_arg);
int display_writer(int _readfd, int _writefd);
  
void * cast_streaming(void *_arg) {
  struct cast_receiver *stream;
  int parent_to_child_pipe[2], child_to_parent_pipe[2];
  pid_t pid;
  FILE *new_stdout, *new_stderr;
  char path[PATH_LENGTH];
  char file_separator[2];
  char *default_directory = ".";
  struct context c;

  stream = (struct cast_receiver *)_arg;
  
  file_separator[0] = get_file_separator();
  file_separator[1] = 0;
  
  /*
    We're going to need 2 pipes : 
     - 1 pipe for the parent -> child communication
     - 1 pipe for the child -> parent communication
  */
  /* 1st pipe : parent -> child communication */
  if (pipe(parent_to_child_pipe) == -1) {
    handle_pipe_error();
    return (void*)EXIT_FAILURE;
  }

  /* 2nd pipe : child -> parent communication */
  if (pipe(child_to_parent_pipe) == -1) {
    handle_pipe_error();
    return (void*)EXIT_FAILURE;
  }

  pid = fork();
  if (pid == -1) {
    handle_fork_error();
    return (void*)EXIT_FAILURE;
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
      return (void*)EXIT_FAILURE;
    }

    /* We pipe stdout onto the write end of the child -> parent pipe */      
    if (dup2(child_to_parent_pipe[1], STDOUT_FILENO) < 0) {
      print_error("dup2");
      return (void*)EXIT_FAILURE;
    }
    
    /* Redirect stdout and stderr on file */
    
    new_stdout = fopen("./stdout.txt", "w");
    dup2(fileno(new_stdout), STDOUT_FILENO);
    fclose(new_stdout);
    

    new_stderr = fopen("./stderr.log", "w");
    dup2(fileno(new_stderr), STDERR_FILENO);
    fclose(new_stderr);
    
    /* Starting VLC */
    strcpy(path, default_directory);
    strcat(path, file_separator);
    strcat(path, stream->file.file_name);

    execlp("vlc", "vlc", "-I", "rc", path, (char*) NULL);
    handle_execlp_error();
    return (void*)EXIT_FAILURE;
  }
  /* Parent process */
  else {
    /* We're closing the read end of the parent -> child pipe */
    close(parent_to_child_pipe[0]);
    /* and the write end of the child -> parent pipe */
    close(child_to_parent_pipe[1]);    

    init_context(VLC, &c, child_to_parent_pipe[0], parent_to_child_pipe[1], stream->upload_percentage);
    update_context_data(&c);
    write_pipe(pipefd, "%s\n", context_to_json(c));

    /* TODO : Start WebUI */

    /* TODO : Interface between WebUI and VLC */
    char input[PIPE_BUF];
    char *command = NULL, *argument = NULL;
    int parameter;
    int pipefd;

    while (1) {
      read_pipe(pipefd, input);
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
      else if (strcmp(command, "shutdown") == 0) {
	shutdown_player(&c);
	*(stream->requested_shutdown) = true;
	/* Kill WebUI process */
	break;
      }

      update_context_data(&c);
      write_pipe(pipefd, "%s\n", context_to_json(c));
    }    
    

#if 0    
    /* TODO : Try communication with VLC */
    char in[100];
    char dummy[100];
    
    bool t;
    t = true;

    write_pipe(parent_to_child_pipe[1], "pause\n");
    sleep(1);
    write_pipe(parent_to_child_pipe[1], "fullscreen\n");
    
    do {
    fscanf(stdin, "%s", in);

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
#endif    
    
    waitpid(-1, NULL, 0);
    return (void*)EXIT_SUCCESS;
  } 
  
  return (void*)EXIT_SUCCESS;
}

