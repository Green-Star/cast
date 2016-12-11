#pragma once

static inline void print_error(char *_msg) {
#ifdef __linux__
  perror(_msg);
#else
  #error "Not implemented yet"
#endif
  
  return;
}


static inline void handle_fopen_error() {
  print_error("fopen");
}

static inline void handle_fdopen_error() {
  print_error("fdopen");
}

static inline void handle_fork_error() {
  perror("fork");
}

static inline void handle_socket_error() {
  print_error("socket");
}

static inline void handle_connect_error() {
  print_error("connect");
}

static inline void handle_bind_error() {
  perror("bind");
}

static inline void handle_listen_error() {
  perror("listen");
}

static inline void handle_accept_error() {
  perror("accept");
}

static inline void handle_send_error() {
  print_error("send");
}

static inline void handle_recv_error() {
  print_error("recv");
}

static inline void handle_getsockopt_error() {
  print_error("getsockopt");
}
