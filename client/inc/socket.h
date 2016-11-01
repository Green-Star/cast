#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#ifdef __linux__
#include <errno.h>
#endif

#define TCP 1
#define UDP 2 

int get_socket(char *_address, int _port, int _protocol);
int close_socket(int _sockfd);
