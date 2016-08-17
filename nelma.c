// Nelma v0.1-pre

// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org>

#include <string.h>

#include "cobs.h"
#include "nelma.h"

bool nelma_read(struct Nelma *nelma, uint8_t input_byte, size_t *out_packet_size) {
  if (input_byte != 0x00) {
    if (nelma->request_size >= NELMA_ENC_PACKET_SIZE) {
      nelma->request_size = 0;
    } else {
      nelma->encoded_packet[nelma->request_size++] = input_byte;
    }
  } else if (nelma->request_size > 0) {
    size_t packet_size = cobs_decode(nelma->encoded_packet, nelma->request_size, nelma->packet);
    nelma->request_size = nelma->response_size = 0;
    *out_packet_size = packet_size;
    return true;
  }
  return false;
}

bool nelma_write(struct Nelma *nelma, uint8_t data) {
  if (nelma->response_size < NELMA_MAX_PACKET_SIZE) {
    nelma->packet[nelma->response_size++] = data;
    return true;
  }
  return false;
}

bool nelma_write_array(struct Nelma *nelma, uint8_t *data, size_t len) {
  if (nelma->response_size + len < NELMA_MAX_PACKET_SIZE) {
    memcpy(nelma->packet + nelma->response_size, data, len);
    nelma->response_size += len;
    return true;
  }
  return false;
}

size_t nelma_encode_response(struct Nelma *nelma) {
  size_t encoded_size = cobs_encode(nelma->packet, nelma->response_size, nelma->encoded_packet);
  nelma->encoded_packet[encoded_size] = 0x00;
  return encoded_size + 1;
}
