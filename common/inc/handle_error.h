#pragma once

static inline void print_error(char *_msg) {
#ifdef __linux__
  perror(_msg);
#else
  #error "Not implemented yet"
#endif
  
  return;
}

static inline void handle_sigaction_error() {
  print_error("sigaction");
}

static inline void handle_fopen_error() {
  print_error("fopen");
}

static inline void handle_fdopen_error() {
  print_error("fdopen");
}

static inline void handle_fork_error() {
  print_error("fork");
}

static inline void handle_kill_error() {
  print_error("kill");
}

static inline void handle_execlp_error() {
  print_error("execlp");
}

static inline void handle_pipe_error() {
  print_error("pipe");
}

static inline void handle_socket_error() {
  print_error("socket");
}

static inline void handle_connect_error() {
  print_error("connect");
}

static inline void handle_bind_error() {
  print_error("bind");
}

static inline void handle_listen_error() {
  print_error("listen");
}

static inline void handle_accept_error() {
  print_error("accept");
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

static inline void handle_pthread_create_error() {
  print_error("pthread_create");
}

static inline void handle_pthread_join_error() {
  print_error("pthread_error");
}

static inline void handle_chdir_error() {
  print_error("chdir");
}
