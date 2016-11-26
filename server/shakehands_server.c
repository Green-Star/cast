#include "shakehands_server.h"

bool shakehands_server(int sockfd, struct cast_file_server *_info) {
  char buffer[SHAKEHAND_PACKET_SIZE];
  char path[PATH_LENGTH];
  int ret, ret_total;
  char protocol;
  char force_upload;
  long title_length;
  char *title;
  long file_length;
  bool file_found;
  bool result;
  long error_code;
  int fd;
  char file_separator[2];
  /* TODO : For now ... */
  char *default_directory = ".";
  
  ret_total = 0;
  file_separator[0] = get_file_separator();
  file_separator[1] = 0;

  /* Shakehanding is initiated by the client */
  do {
    ret = recv(sockfd, (void*)buffer, SHAKEHAND_PACKET_SIZE, 0);
    ret_total += ret;
  } while(ret_total < SHAKEHAND_PACKET_SIZE);

  protocol = buffer[0];
  force_upload = buffer[1];
  title_length = deserialize_long(buffer + 2);
  title = &buffer[10];
  file_length = deserialize_long(buffer + 10 + title_length);

  strcpy(path, default_directory);
  strcat(path, file_separator);
  strcat(path, title);

  _info->file.file_length = file_length;
  strcpy(_info->file.file_name, title);
  _info->file.file_name_length = title_length;
  
  /* Search file with the given title and the given length */
  if (force_upload == 0) {
    file_found = search_file(path, file_length);
  }

  memset((void*)buffer, 0, sizeof(buffer));
  
  /* Send SERVER_UPLOAD (we need to send the file) */
  if (force_upload || file_found == false) {
    _info->upload = true;

    /* First we create the file */
    fd = open(path, O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    _info->fd = fd;
    
    /* If an error occured, we inform the client by sending SERVER_ERROR */
    if (fd < 0) {
      perror("open");
      
      result = false;
      error_code = SERVER_ERROR;
    }
    /* If everything was fine, we are ready to recieve data from the client */
    else {
      result = true;
      error_code = SERVER_UPLOAD;
    }
  }
  /* Send SERVER_OK (don't need to send the file) */
  else {
    _info->upload = false;
    result = true;
    error_code = SERVER_OK;
  }

  /* Send response to the client */
  ret_total = 0;
  serialize_long(error_code, buffer);
  do {
    ret = send(sockfd, (void*)buffer, SHAKEHAND_PACKET_SIZE, 0);
    ret_total += ret;
  } while(ret_total < SHAKEHAND_PACKET_SIZE);
  
  /* After this, start streaming and/or recveiver part */
  return result;
}


bool search_file(char *_path, long _file_length) {
  struct stat info;

  if (stat(_path, &info) != 0) {
    return false;
  }

  /* If we found a file with the same title and the same length, we assume we have found the _title file */
  if (info.st_size == (off_t)_file_length) {
    return true;
  }

  return false;
}
