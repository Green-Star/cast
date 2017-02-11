#include "cast_streaming.h"

void* start_display_writer(void *_arg);
int display_writer(int _readfd, int _writefd);
  
void * cast_streaming(void *_arg) {
  struct cast_receiver *stream;
  int parent_to_child_pipe[2], child_to_parent_pipe[2];
  int UI_to_parent_pipe[2], parent_to_UI_pipe[2];
  pid_t pid, UI_pid;
  FILE *new_stdout, *new_stderr;
  char path[PATH_LENGTH];
  char file_separator[2];
  char *default_directory = ".";
  char input[PIPE_BUF];
  char *command = NULL, *argument = NULL;
  int parameter;    
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
    /*
    new_stdout = fopen("./stdout.txt", "w");
    dup2(fileno(new_stdout), STDOUT_FILENO);
    fclose(new_stdout);
    */

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
    
    /*
      We're going to need 2 pipes : 
      - 1 pipe for the parent -> UI communication
      - 1 pipe for the UI -> parent communication
    */
    /* 1st pipe : parent -> UI communication */
    if (pipe(parent_to_UI_pipe) == -1) {
      handle_pipe_error();
      shutdown_player(&c);
      return (void*)EXIT_FAILURE;
    }

    /* 2nd pipe : UI -> parent communication */
    if (pipe(UI_to_parent_pipe) == -1) {
      handle_pipe_error();
      shutdown_player(&c);
      return (void*)EXIT_FAILURE;
    }

    /* Start WebUI */
    UI_pid = fork();
    if (UI_pid == -1) {
      handle_fork_error();
      shutdown_player(&c);
      return (void*)EXIT_FAILURE;
    }
    if (UI_pid == 0) {
      start_webUI(parent_to_UI_pipe, UI_to_parent_pipe);
      /* We're not supposed ti return from the start_webUI call */
      return (void*)EXIT_FAILURE;
    }
    else {
      /* We close the read end of the parent -> UI pipe */
      close(parent_to_UI_pipe[0]);
      /* We close the write end of the UI -> parent pipe */
      close(UI_to_parent_pipe[1]);
      
      update_context_data(&c);
      write_pipe(parent_to_UI_pipe[1], "%s\n", context_to_json(c));
    
      /* Infinite loop to interface between WebUI and player */
      while (1) {
	/* Read commands from the UI */
	read_pipe(UI_to_parent_pipe[0], input);
	parse_input(input, &command, &argument);

	/* Send it to the player */
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
	  /* Stop receiving part if necessary */
	  *(stream->requested_shutdown) = true;
	  /* Kill WebUI process */
	  if (kill(UI_pid, SIGUSR1) == -1) {
	    handle_kill_error();
	    /* Terminate everything the hard way ... */
	    kill(0, SIGKILL);
	    return (void*)EXIT_FAILURE;
	  }
	  break;
	}

	/* Update player data */
	update_context_data(&c);
	/* Send it to the WebUI */
	write_pipe(parent_to_UI_pipe[1], "%s\n", context_to_json(c));
      }    

      /* Reap sons processes before terminating */
      waitpid(pid, NULL, 0);
      waitpid(UI_pid, NULL, 0);
      return (void*)EXIT_SUCCESS;
    } 
  }
  
  return (void*)EXIT_SUCCESS;
}

void start_webUI(int *_parent_to_UI_pipe, int *_UI_to_parent_pipe) {
  FILE *new_stdout, *new_stderr;

  /* Close the write end of the parent_to_UI_pipe */
  close(_parent_to_UI_pipe[1]);
  
  /* Close the read end of the UI_to_parent_pipe */
  close(_UI_to_parent_pipe[0]);
  
  /* Redirect stdout and stderr on file */
  /*
  new_stdout = fopen("./node-stdout.txt", "w");
  dup2(fileno(new_stdout), STDOUT_FILENO);
  fclose(new_stdout);
    
  */
  new_stderr = fopen("./node-stderr.log", "w");
  dup2(fileno(new_stderr), STDERR_FILENO);
  fclose(new_stderr);
  
  /* Go to the WebUI directory */
  if (chdir("../../webui") < 0) {
    handle_chdir_error();
    return;
  }
  
  /* Pipe stdin and stdout */
  if (dup2(_parent_to_UI_pipe[0], STDIN_FILENO) < 0) {
    print_error("dup2");
    return;
  }

  if (dup2(_UI_to_parent_pipe[1], STDOUT_FILENO) < 0) {
    print_error("dup2");
    return;
  }  

  /* Start WebUI */
  execlp("node", "node", "index.js", (char*)NULL);
  handle_execlp_error();
  return;
}
