#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include "common.h"
#include "config.h"
#include "shakehands_client.h"
#include "file.h"
#include "cast_file.h"

static void usage(char *_argv0);

static void usage(char *_argv0) {
  fprintf(stderr, "Usage : %s <file> <address> <port>\n", _argv0);
  fprintf(stderr, "  options : Not Implemented Yet\n");
}

int main(int argc, char **argv) {
  int sockfd;
  int error;
  bool upload, upload_complete;
  struct addrinfo hints, result;
  struct addrinfo *info, *p;
  struct cast_file f;

  if (argc < 4) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  get_config();

  f.file = fopen(argv[1], "rb");
  if (f.file == NULL) {
    handle_fopen_error();
    return EXIT_FAILURE;
  }

  if (get_file_length(argv[1], &(f.file_length)) == false) {
    return EXIT_FAILURE;
  }

  if (get_file_name(argv[1], f.file_name, &(f.file_name_length)) == false) {
    return EXIT_FAILURE;
  }

  printf("argv : [%s], file_name : [%s], file_name_length : [%d], file_length : [%ld]\n", argv[1], f.file_name, f.file_name_length, f.file_length);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; /* Either IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Connection socket is a TCP socket */
  hints.ai_protocol = 0;

  error = getaddrinfo(argv[2], argv[3], &hints, &info);
  if (error != 0) {
    fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(error));
    fclose(f.file);
    return EXIT_FAILURE;
  }


  for(p = info; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd < 0) {
      handle_socket_error();
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
      handle_connect_error();
      close(sockfd);
      continue;
    }

    /* Simply use the first addrinfo we can connect to */
    result = *p;
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "No suitable connection found !\nBetter luck next time.\n");
    freeaddrinfo(info);
    fclose(f.file);
    return EXIT_FAILURE;
  }

  freeaddrinfo(info);
  
  upload = shakehands_client(sockfd, f);

  if (upload) {
    upload_complete = cast_file(sockfd, f);
  }
  else {
    upload_complete = true;
  }

  fclose(f.file);
  
  close(sockfd);
  
  return (upload_complete) ? EXIT_SUCCESS : EXIT_FAILURE;
}
