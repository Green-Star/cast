#include "write_pipe.h"

static ssize_t send_pipe(int _pipe_fd, char const * const _buf, size_t const _length);
static void cat_string(char * const buffer, char const * const s, int *offset);
static void cat_number(char * const buffer, int const number, int *offset);
static void cat_double(char * const buffer, double const number, int *offset);
static void cat_float(char * const buffer, float const number, int *offset);

static char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

ssize_t write_pipe(const int _pipe_fd, char const * const _fmt, ...) {
  char buffer[PIPE_BUF];
  char const *s;
  bool special_char;
  int offset;

  va_list ap;
  int d;
  char c;
  char *sub_string;

  s = _fmt;
  offset = 0;
  special_char = false;

  memset((void*)buffer, 0, PIPE_BUF);
  
  va_start(ap, _fmt);

  while(*s) {
    if (special_char) {
      switch (*s) {
        case '%':
	    buffer[offset] = '%';
	    offset++;
	break;

        case 's':
	  sub_string = va_arg(ap, char*);
	  cat_string(buffer, sub_string, &offset);
	break;

        case 'c':
	  c = (char)va_arg(ap, int);
	  buffer[offset] = c;
	  offset++;
	break;

        case 'd':
	  d = va_arg(ap, int);
	  cat_number(buffer, d, &offset);
	break;
	
        default:
	  buffer[offset] = *s;
	  offset++;
	break;
      }
      
      special_char = false;
    }
    else {
      if (*s == '%') {
	special_char = true;
      }
      else {
	buffer[offset] = *s;
	offset++;
      }
    }
    
    s++;
  }

  va_end(ap);

  return send_pipe(_pipe_fd, buffer, offset);
}


static ssize_t send_pipe(int _pipe_fd, char const * const _buf, size_t const _length) {
  return write(_pipe_fd, _buf, _length);
}

static void cat_string(char * const _buffer, char const * const _string, int *_offset) {
  char const *s = _string;

  while (*s) {
    _buffer[*_offset] = *s;
    (*_offset)++;
    s++;
  }
}

static void cat_number(char * const _buffer, int const _number, int *_offset) {
  char tmp[1024];
  int i;
  int number;
  int x;

  i = 0;
  number = _number;

  if (number == 0) {
    _buffer[*_offset] = 0;
    (*_offset)++;
    return;
  }

  if (number < 0) {
    _buffer[*_offset] = '-';
    (*_offset)++;
    number *= -1;
  }
  
  while (number > 0) {
    x = number % 10;
    number /= 10;
    tmp[i] = numbers[x];
    i++;
  }

  for (i -= 1; i >= 0; i--) {
    _buffer[*_offset] = tmp[i];
    (*_offset)++;
  }
}

static void cat_double(char * const buffer, double const number, int *offset);
static void cat_float(char * const buffer, float const number, int *offset);

ssize_t read_pipe(int const _pipefd, char * const _buffer) {
  char buf[PIPE_BUF];
  ssize_t ret;

  memset(buf, 0, PIPE_BUF * sizeof(char));

  ret = read(_pipefd, buf, PIPE_BUF);
  strcpy(_buffer, buf);
  return ret;
}
