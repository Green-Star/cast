#include "socket.h"

static int create_socket(int _protocol, int *_protocol_family);
static int attempt_connection(int _sockfd, struct sockaddr *_addr, ssize_t _size);

int get_socket(char *_address, int _port, int _protocol) {
  int sockfd;
  int protocol_family;
  int local_error;
  struct sockaddr_in addr_in;

  sockfd = create_socket(_protocol, &protocol_family);

  if (sockfd < 0) {
    return -1;
  }

  if (protocol_family == AF_INET) {
    addr_in.sin_family = protocol_family;
    addr_in.sin_port = htons(_port);
    if (inet_pton(protocol_family, _address, &(addr_in.sin_addr)) != 1) {
      perror("inet_pton");
      return -1;
    };
    memset((void*)&(addr_in.sin_zero), 0, sizeof(addr_in.sin_zero));
  }


  do {
    local_error = attempt_connection(sockfd, (struct sockaddr *)&addr_in, sizeof(addr_in));
    if (local_error == 0) {
      printf("Connection refused on %s:%d, we'll retry in few seconds\n", _address, _port);
      sleep(1);
    }
  } while (local_error == 0);

  return sockfd;
}

int close_socket(int _sockfd) {
  return close(_sockfd);
}

static int create_socket(int _protocol, int *_protocol_family) {
  int sockfd;

  switch(_protocol) {
  case TCP :
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    *_protocol_family = AF_INET;
    break;
    
  case UDP:
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    *_protocol_family = AF_INET;
    break;

  default:
    fprintf(stderr, "Unknown _protocol %d\n", _protocol);
    sockfd = -1;
    break;
  }

  if (sockfd < 0) {
    perror("socket");
    return -1;
  }

  return sockfd;
}

static int attempt_connection(int _sockfd, struct sockaddr *_addr, ssize_t _size)
{
  int local_error;

  local_error = connect(_sockfd, _addr, _size);
  
#ifdef __linux__
  if (errno == ECONNREFUSED) {
    return 0;
  }
#endif
  
  if (local_error < 0) {
    perror("connect");
    return -1;
  }
  
  return 1;
}
