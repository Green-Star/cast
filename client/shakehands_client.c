#include "shakehands_client.h"

static void handle_send_error();

static void handle_send_error() {
  perror("send");
}

void shakehands_client(int _sockfd, struct cast_file _file_to_send) {
  char buffer[SHAKEHAND_PACKET_SIZE];
  char path[FILE_LENGTH];
  ssize_t total_sent, sent;
  
  buffer[0] = 0; /* TODO : Replace it with protocol (in a common header) */
  buffer[1] = 1; /* TODO : Replace it with the value of --force-upload */
  sprintf(buffer + 2, "%d", strlen(_file_to_send.file_name));
  strcpy(buffer + 3, _file_to_send.file_name);
  sprintf(buffer + 3 + buffer[2], "%d", _file_to_send.file_length);

  printf("Shakehanding with the server ...\n");
  
  total_sent = 0;
  while(total_sent < SHAKEHAND_PACKET_SIZE * sizeof(char)) {
    sent = send(_sockfd, buffer, SHAKEHAND_PACKET_SIZE * sizeof(char), 0);
    if (sent < 0) {
      handle_send_error();
      fclose(_file_to_send.file);
      close(_sockfd);
      exit(EXIT_FAILURE);
    }
    
    total_sent += sent;
  }
  
  printf("Waiting for response from the server ...\n");
  
  return;
}
