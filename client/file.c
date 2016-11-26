#include "file.h"


bool get_file_name(char *_path, char *_file_name, int *_file_name_length) {
  int offset, i, length;
  char separator;
  bool separator_found;

  separator = get_file_separator();

  offset = 0;
  separator_found = false;
  
  for(i = 0; _path[i]; i++) {
    if (_path[i] == separator) {
      offset = i;
      separator_found = true;
    }
  }
  i++; /* We want the NULL-terminating byte to be part of the file name */

  /* If we found a separator, we add 1 to the offset, to place it on the file's first character */
  if (separator_found) {
    offset++;
  }
  /* If we haven't found any separator in the path, the first file name character is the path's first character */
  else {
    offset = 0;
  }

  length = i - offset;
  
  if (length > FILE_LENGTH) {
    fprintf(stderr, "File name too long !");
    return false;
  }

  strncpy(_file_name, _path + offset, length);
  *_file_name_length = length;
  
  return true;
}


bool get_file_length(char *_path, long *_file_length) {
  struct stat info;

  if (stat(_path, &info) < 0) {
    perror("stat");
    return false;
  }

  if (S_ISREG(info.st_mode)) {
    *_file_length = (long)info.st_size;
    return true;
  }

  fprintf(stderr, "%s is not a regular file !\n", _path);
  return false;
}
