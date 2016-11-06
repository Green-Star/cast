#include "shakehands_server.h"

void shakehands_server(int sockfd) {
  char buffer[SHAKEHAND_PACKET_SIZE];
  int ret, ret_total;
  char protocol;
  char force_upload;
  int title_len;
  char *title;

  ret_total = 0;

  /* Shakehanding is initiated by the client */
  do {
    ret = recv(sockfd, (void*)buffer, SHAKEHAND_PACKET_SIZE, 0);
    ret_total += ret;
  } while(ret_total < SHAKEHAND_PACKET_SIZE);

  protocol = buffer[0];
  force_upload = buffer[1];
  title_len = (int)buffer[2];
  title = &buffer[3];

  if (force_upload == 1) {
    ;
  }
  
  return;
}
