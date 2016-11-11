#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

/* TODO : Put this in a common header shared by client and server */
#define SHAKEHAND_PACKET_SIZE 1024
#define FILE_LENGTH 512

struct cast_file {
  FILE *file;
  long file_length;
  char file_name[FILE_LENGTH];
};

void shakehands_client(int _sockfd, struct cast_file _file_to_send);
