#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "shakehands_server.h"

int main(int argc, char **argv) {
  int sockfd;
  struct cast_file_server upload_file;

  sockfd = strtol(argv[1], NULL, 10);

  if (shakehands_server(sockfd, &upload_file) == false) {
    close(sockfd);
    return EXIT_FAILURE;
  }

  close(sockfd);

  return EXIT_SUCCESS;
}
