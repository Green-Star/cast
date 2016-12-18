#include "cast_file.h"

bool cast_file(int _sockfd, struct cast_file _file) {
  char *buffer;
  int buffer_length;
  size_t bytes_read, current_bytes_sent, total_bytes_sent;
  ssize_t bytes_sent;
  size_t buffer_size;
  socklen_t buffer_length_size;

  total_bytes_sent = 0;
  buffer = NULL;
  buffer_length_size = sizeof(buffer_length);
  if (getsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, (void*)&buffer_length, &buffer_length_size) < 0) {
    handle_getsockopt_error();
    return false;
  }

  /* The returned value is doubled by the kernel */
  buffer_length /= 2;

  buffer_size = buffer_length * sizeof(char);
  buffer = (char*)malloc(buffer_size);
  if (buffer == NULL) {
    fprintf(stderr, "malloc failed !\n");
    return false;
  }

  printf("Starting uploading of %s\n", _file.file_name);

  do {
    printf("************************* LOOP ***************************\n");
    /* Read file data */
    bytes_read = fread((void*)buffer, (size_t)1, buffer_size, _file.file);
    if (ferror(_file.file) != 0) {
      print_error("fread");
      fprintf(stderr, "Aborting upload\n");
      free(buffer);
      return false;
    }

    sleep(1);
    
    current_bytes_sent = 0;

  printf("buffer_size : %ld\n", buffer_size);
    printf("bytes_read : [%ld]\n", bytes_read);

    /* Send it to the server */
    /* We send all the data read before fetching new ones */
    do {
      bytes_sent = send(_sockfd, (void*)buffer, bytes_read, 0);

      printf("bytes_sent : %ld\n", bytes_sent);
      
      if (bytes_sent < 0) {
	print_error("\rError in send");
	fprintf(stderr, "Aborting upload\n");
	free(buffer);
	return false;
      }
      
      current_bytes_sent += bytes_sent;

      printf("current_bytes_sent : %ld (bytes_read : %ld)\n", current_bytes_sent, bytes_read);
    } while(current_bytes_sent < bytes_read);

    total_bytes_sent += current_bytes_sent;

    printf("total_bytes_sent : %ld (file_length : %ld)\n", total_bytes_sent, _file.file_length);

    printf("\rBytes sent : %ld/%ld (%ld%%)", total_bytes_sent, _file.file_length, (total_bytes_sent / _file.file_length));
    printf("******************************* END_LOOP *************************\n");
  } while(total_bytes_sent < (size_t)_file.file_length);
  
  free(buffer);
  
  return true;
}
