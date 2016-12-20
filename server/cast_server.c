#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>

#include "shakehands_server.h"
#include "cast_receiver.h"
#include "cast_streaming.h"

int main(int argc, char **argv) {
  int sockfd;
  struct cast_file_server upload_file;
  struct cast_receiver receiver_file;
  pthread_t receiver_thread_id, streaming_thread_id;
  int *receiver_thread_return_code, streaming_thread_return_code;
  int upload_percentage;

  sockfd = strtol(argv[1], NULL, 10);
 
  if (shakehands_server(sockfd, &upload_file) == false) {
    close(sockfd);
    return EXIT_FAILURE;
  }

  receiver_file.file = upload_file.file;
  receiver_file.filefd = upload_file.fd;
  receiver_file.sockfd = sockfd;
  receiver_file.upload_percentage = &upload_percentage;
  
  /* Start receiver part */
  if (upload_file.upload) {
    if (pthread_create(&receiver_thread_id, NULL, cast_receiver, &receiver_file) != 0)
    {
      handle_pthread_create_error();
      close(sockfd);
      close(receiver_file.filefd);
      return EXIT_FAILURE;
    }
  }

  /* Start streaming part */
  if (pthread_create(&streaming_thread_id, NULL, cast_streaming, &receiver_file) != 0) {
    handle_pthread_create_error();
    pthread_kill(receiver_thread_id, SIGKILL);
    close(sockfd);
    close(receiver_file.filefd);
    return EXIT_FAILURE;
  }

  if (pthread_join(receiver_thread_id, (void**)&receiver_thread_return_code) != 0)
  {
    handle_pthread_join_error();
  }
  
  close(sockfd);

  return EXIT_SUCCESS;
}
