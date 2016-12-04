#include "shakehands_client.h"

bool shakehands_client(int _sockfd, struct cast_file _file_to_send) {
  char buffer[SHAKEHAND_PACKET_SIZE];
  char path[FILE_LENGTH];
  size_t total_sent, total_received;
  ssize_t sent, received;
  long error_code;
  bool ret;
  
  buffer[0] = 0; /* TODO : Replace it with protocol (in a common header) */
  buffer[1] = 1; /* TODO : Replace it with the value of --force-upload */
  serialize_long((long)_file_to_send.file_name_length, buffer + 2);
  strcpy(buffer + 10, _file_to_send.file_name);
  serialize_long(_file_to_send.file_length, buffer + 10 + _file_to_send.file_name_length);

  printf("Shakehanding with the server ...\n");
  
  total_sent = 0;
  do {
    sent = send(_sockfd, (void*)buffer, SHAKEHAND_PACKET_SIZE, 0);
    if (sent < 0) {
      handle_send_error();
      fclose(_file_to_send.file);
      close(_sockfd);
      exit(EXIT_FAILURE);
    }
    
    total_sent += sent;
  } while(total_sent < SHAKEHAND_PACKET_SIZE);
  
  printf("Waiting for response from the server ...\n");

  total_received = 0;
  do {
    received = recv(_sockfd, (void*)buffer, SHAKEHAND_PACKET_SIZE, 0);
    if (received < 0) {
      handle_recv_error();
      fclose(_file_to_send.file);
      close(_sockfd);
      exit(EXIT_FAILURE);
    }
    
    total_received += received;
  } while(total_received < SHAKEHAND_PACKET_SIZE);

  error_code = deserialize_long(buffer);
  switch (error_code) {
    case SERVER_OK :
      printf("File already uploaded on the server\n");
      ret = false;
      break;

    case SERVER_UPLOAD :
      printf("Uploading file\n");
      ret = true;
      break;
      
    case SERVER_ERROR :
    default:
      printf("Server error. Aborting upload\n");
      fclose(_file_to_send.file);
      close(_sockfd);
      exit(EXIT_SUCCESS);
      break;
  }
  
  return ret;
}
