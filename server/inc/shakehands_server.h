#pragma once

#include <sys/types.h>
#include <sys/socket.h>

#define SHAKEHAND_PACKET_SIZE 1024

void shakehands_server(int sockfd);
