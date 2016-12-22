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
    print_error("execlp");
    return (void*)EXIT_FAILURE;
  }
  /* Parent process */
  else {
    /* We're closing the read end of the parent -> child pipe */
    close(parent_to_child_pipe[0]);
    /* and the write end of the child -> parent pipe */
    close(child_to_parent_pipe[1]);    

    /* TODO : Try communication with VLC */
    
    waitpid(-1, NULL, 0);
    return (void*)EXIT_SUCCESS;
  } 
  
  return (void*)EXIT_SUCCESS;
}

/* TMP */
void* start_display_writer(void *_arg) {
  int *ptr;

  ptr = (int*)_arg;
    
  display_writer(ptr[2], ptr[1]);
  
  return NULL;
}

int display_writer(int _readfd, int _writefd) {
  char in[100];
  int offset;
  int c;

  c=0;
  do {
    sleep(10);
    c++;
  } while(c < 20);

#if 0  
  write_pipe(_writefd, "pause\n");
  
  /* Let's give some rest to VLC to let it initialize its stuff */
  sleep(1);
  /*
  write_pipe(_pipefd, "get_length\n");
  write_pipe(_pipefd, "volume\n");
  write_pipe(_pipefd, "atrack\n");
  write_pipe(_pipefd, "vtrack\n");
  write_pipe(_pipefd, "strack\n");
  write_pipe(_pipefd, "pause\n");
  write_pipe(_pipefd, "fullscreen\n");
  */
  init_vlc(_writefd, _readfd);

  sleep(1);
  int total_length = 0;
  perror("avant");
  get_length_vlc(_writefd, _readfd, &total_length);
  perror("apres:");
  printf("Total length : %d\n", total_length);
  
  
  do {
    
    offset = 0;
    do {
      c = fgetc(stdin);
      in[offset] = (char)c;
      offset++;
    } while(c != '\n');
    in[offset] = 0;

    
    printf("[Thread display_writer] : j'ai lu [%s]\noffset : [%d]\n", in, offset);

    perror("avant");

    
    if (strcmp("get_length\n", in) == 0)
      {
	get_length_vlc(_writefd, _readfd, &total_length);
	printf("[THREAD] Longueur lue : %d\n", total_length);
      }
    else
      offset = write_pipe(_writefd, "%s", in);
    printf("[THREAD] ecrits : [%d]\n", offset);
    
    perror("write_pipe");
  } while (strcmp(in, "quit\n") != 0);

  printf("[Thread display_writer] : j'ai fini, je m'en vais\n");
#endif
  return EXIT_SUCCESS;
}
