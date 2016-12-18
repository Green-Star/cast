#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include "shakehands_server.h"
#include "cast_receiver.h"

int main(int argc, char **argv) {
  int sockfd;
  struct cast_file_server upload_file;
  struct cast_receiver receiver_file;
  pthread_t receiver_thread_id;
  int *receiver_thread_return_code;

  sockfd = strtol(argv[1], NULL, 10);
 
  if (shakehands_server(sockfd, &upload_file) == false) {
    close(sockfd);
    return EXIT_FAILURE;
  }

  receiver_file.file = upload_file.file;
  receiver_file.filefd = upload_file.fd;
  receiver_file.sockfd = sockfd;
  
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

  if (pthread_join(receiver_thread_id, (void**)&receiver_thread_return_code) != 0)
  {
    handle_pthread_join_error();
  }
  
  close(sockfd);

  return EXIT_SUCCESS;
}
