#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include "config.h"
#include "shakehands_client.h"

static void usage(char *_argv0);
static void handle_socket_error();
static void handle_connect_error();

static void usage(char *_argv0) {
  fprintf(stderr, "Usage : %s <file> <address> <port>\n", _argv0);
  fprintf(stderr, "  options : Not Implemented Yet\n");
}

static void handle_socket_error() {
  perror("socket");
}

static void handle_connect_error() {
  perror("connect");
}

int main(int argc, char **argv) {
  int sockfd;
  int error;
  struct addrinfo hints, result;
  struct addrinfo *info, *p;

  if (argc < 4) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  get_config();

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; /* Either IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Connection socket is a TCP socket */
  hints.ai_protocol = 0;

  error = getaddrinfo(argv[2], argv[3], &hints, &info);
  if (error != 0) {
    fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(error));
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
    return EXIT_FAILURE;
  }

  freeaddrinfo(info);
  
  printf("[client] Connection established, we're going to shake hands\n");
  shakehands_client(sockfd);
  
  close(sockfd);
  
  return EXIT_SUCCESS;
}
