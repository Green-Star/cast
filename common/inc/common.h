#pragma once

#include <stdint.h>
#include <endian.h>

#include "common_defines.h"
#include "common_types.h"

#include "handle_error.h"

static inline void serialize_long(long _long, char *_buffer);
static inline long deserialize_long(char *_buffer);
static inline char get_file_separator();

static inline void serialize_long(long _long, char *_buffer) {
  uint64_t be_long;
  int i;

  be_long = htobe64(_long);
  for(i = 7; i >= 0; i--) {
    _buffer[i] = (char)be_long;
    be_long >>= 8;
  }
}

static inline long deserialize_long(char *_buffer) {
  uint64_t be_long;
  int i;

  be_long = 0L;
  for(i = 0; i < 8; i++) {    
    be_long <<= 8;
    be_long |= _buffer[i] & 255; /* We just want to have the least byte so we nullify all others bytes before OR bitwise */ 
  }
  
  return be64toh(be_long);
}

static inline char get_file_separator() { 
#ifdef _WIN32
  return '\\';
#elif __linux__
  return '/';
#else
  #error "Unknown OS file separator !" 
#endif
}
