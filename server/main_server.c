#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

static void usage(char *_argv0);
static void handle_socket_error();
static void handle_connect_error();
static void handle_bind_error();
static void handle_listen_error();
static void handle_accept_error();

static void usage(char *_argv0) {
  fprintf(stderr, "Usage : %s <port to listen>\n", _argv0);
}

static void handle_socket_error() {
  perror("socket");
}

static void handle_connect_error() {
  perror("connect");
}

static void handle_bind_error() {
  perror("bind");
}

static void handle_listen_error() {
  perror("listen");
}

static void handle_accept_error() {
  perror("accept");
}

int main(int argc, char **argv) {
  int sockfd;
  int error;
  struct addrinfo hints, server;
  struct addrinfo *info, *p;
  char *port;
  int backlog = 5;

  if (argc < 2) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  /* TODO : config_file */
  port = argv[1];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; /* Either IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Connection socket is a TCP socket */
  hints.ai_flags = AI_PASSIVE;

  error = getaddrinfo(NULL, port, &hints, &info);
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

    /* TODO : Socket option (from config file) */
    
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
      handle_bind_error();
      close(sockfd);
      continue;
    }

    /* Let's use the first bound socket */
    server = *p;
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "Bind failed !\n");
    freeaddrinfo(info);
    return EXIT_FAILURE;
  }
  
  freeaddrinfo(info);

  /* TODO : Use config file for queue length */
  if (listen(sockfd, backlog) < 0) {
    handle_listen_error();
    close(sockfd);
    return EXIT_FAILURE;
  }

  /* Main loop (accept connection and fork to a child process) */
  while(1) {
    ;
  }
  
  return EXIT_SUCCESS;
}
