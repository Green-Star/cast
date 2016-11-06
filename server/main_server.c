#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>

static void usage(char *_argv0);
static void handle_socket_error();
static void handle_bind_error();
static void handle_listen_error();
static void handle_accept_error();
static void handle_fork_error();

static void usage(char *_argv0) {
  fprintf(stderr, "Usage : %s <port to listen>\n", _argv0);
}

static void handle_socket_error() {
  perror("socket");
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

static void handle_fork_error() {
  perror("fork");
}

int main(int argc, char **argv) {
  int sockfd, clientfd;
  int pid;
  int error;
  char *port;
  int backlog = 5;
  struct addrinfo hints, server;
  struct addrinfo *info, *p;
  struct sockaddr_storage client_addr;
  socklen_t client_len;
  char clientfd_param[3];

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

    while(waitpid(-1, NULL, WNOHANG) > 0); /* Reap all sons zombies processes */
    
    client_len = sizeof(client_addr);
    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (clientfd < 0) {
      /* TODO : Handle error */
      handle_accept_error();
      continue;
    }


    
    pid = fork();
    if (pid < 0) {
      handle_fork_error();
      exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
      /* Son code */
      close(sockfd); /* Son doesn't need this */
      /* TODO : execlp */
      sprintf(clientfd_param, "%d", clientfd);
      execlp("cast_server", "cast_server", clientfd_param, (char*)NULL);
      fprintf(stderr, "execlp failed\n");
      exit(EXIT_FAILURE);
    }

    /* Parent does not need to interact with the client */
    close(clientfd);
  }
  
  return EXIT_SUCCESS;
}
