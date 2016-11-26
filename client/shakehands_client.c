#include "shakehands_client.h"

static void handle_send_error();

static void handle_send_error() {
  perror("send");
}

void shakehands_client(int _sockfd, struct cast_file _file_to_send) {
  char buffer[SHAKEHAND_PACKET_SIZE];
  char path[FILE_LENGTH];
  size_t total_sent;
  ssize_t sent;
  
  buffer[0] = 0; /* TODO : Replace it with protocol (in a common header) */
  buffer[1] = 1; /* TODO : Replace it with the value of --force-upload */
  serialize_long((long)_file_to_send.file_name_length, buffer + 2);
  strcpy(buffer + 10, _file_to_send.file_name);
  serialize_long(_file_to_send.file_length, buffer + 10 + _file_to_send.file_name_length);

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
