#pragma once

struct cast_receiver {
  struct cast_file file;
  int filefd;
  int sockfd;
  int *upload_percentage;
};
