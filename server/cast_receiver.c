#include "cast_receiver.h"

void *cast_receiver(void *_arg) {
  struct cast_receiver *upload;
  ssize_t bytes_read;
  size_t total_bytes_read, bytes_to_write, current_bytes_written;
  char *buffer;
  int buffer_length;
  size_t buffer_size;
  socklen_t buffer_length_size;

  upload = (struct cast_receiver *)_arg;
  total_bytes_read = 0;
  buffer = NULL;
  buffer_length_size = sizeof(buffer_length);
  if (getsockopt(upload->sockfd, SOL_SOCKET, SO_RCVBUF, (void*)&buffer_length, &buffer_length_size) < 0) {
    handle_getsockopt_error();
    return (void*)EXIT_FAILURE;
  }

  /* The returned value is doubled by the kernel */
  buffer_length /= 2;

  buffer_size = buffer_length * sizeof(char);
  buffer = (char*)malloc(buffer_size);
  if (buffer == NULL) {
    fprintf(stderr, "malloc failed !\n");
    return (void*)EXIT_FAILURE;
  }

  upload->file.file = fdopen(upload->filefd, "wb");
  if (upload->file.file == NULL) {
    handle_fdopen_error();
    free(buffer);
    return (void*)EXIT_FAILURE;
  }

  printf("Starting receiving of %s\n", upload->file.file_name);
  
  do {
    bytes_read = recv(upload->sockfd, (void*)buffer, buffer_size, 0);

    if (bytes_read < 0) {
      print_error("\rError in recv");
      fprintf(stderr, "Aborting transfer\n");
      free(buffer);
      fclose(upload->file.file);
      /* TODO : Remove corrupt file ? */
      return (void*)EXIT_FAILURE;
    }

    bytes_to_write = bytes_read;
    do {
      current_bytes_written = fwrite((void*)buffer, bytes_to_write, (size_t)1, upload->file.file);

      if (current_bytes_written == 0) {
	print_error("\rError in fwrite");
	fprintf(stderr, "Aborting transfer\n");
	free(buffer);
	fclose(upload->file.file);
	/* TODO : Remove corrupt file ? */
	return (void*)EXIT_FAILURE;
      }

      bytes_to_write -= current_bytes_written;
    } while(bytes_to_write > 0);
    total_bytes_read += bytes_read;

    printf("\rBytes written : %ld/%ld (%ld%%)", total_bytes_read, upload->file.file_length, (total_bytes_read / upload->file.file_length));
    
  } while(total_bytes_read < (size_t)upload->file.file_length);

  free(buffer);
  fclose(upload->file.file);
  
  return (void*)EXIT_SUCCESS;
}
