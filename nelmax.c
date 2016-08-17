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

#include "nelmax.h"

bool nelmax_read(struct NelmaX *nelmax, uint8_t input_byte, uint8_t *out_command, size_t *out_payload_size) {
  size_t packet_size;
  if (nelma_read(&nelmax->nelma, input_byte, &packet_size) && packet_size > 0) {
    size_t payload_size = packet_size - 1;
    *out_command = nelmax->current_command = nelmax->nelma.packet[payload_size];
    *out_payload_size = payload_size;
    return true;
  }
  return false;
}

bool nelmax_write(struct NelmaX *nelmax, uint8_t data) {
  if (nelmax->nelma.response_size < NELMAX_MAX_PAYLOAD_SIZE) {
    nelma_write(&nelmax->nelma, data);
    return true;
  }
  return false;
}

bool nelmax_write_array(struct NelmaX *nelmax, uint8_t *data, size_t len) {
  if (nelmax->nelma.response_size + len < NELMAX_MAX_PAYLOAD_SIZE) {
    nelma_write_array(&nelmax->nelma, data, len);
    return true;
  }
  return false;
}

size_t nelmax_encode_response(struct NelmaX *nelmax) {
  nelma_write(&nelmax->nelma, nelmax->current_command);
  return nelma_encode_response(&nelmax->nelma);
}

const uint8_t * nelmax_payload(struct NelmaX *nelmax) {
  return nelmax->nelma.packet;
}
const uint8_t * nelmax_encoded_packet(struct NelmaX *nelmax) {
  return nelmax->nelma.encoded_packet;
}
