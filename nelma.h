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

#ifndef NELMA_H
#define NELMA_H

#ifndef NELMA_MAX_PACKET_SIZE
#ifdef NELMAX_MAX_PAYLOAD_SIZE
#define NELMA_MAX_PACKET_SIZE (NELMAX_MAX_PAYLOAD_SIZE + 1)
#else
#error "Please define either NELMA_MAX_PACKET_SIZE or NELMAX_MAX_PAYLOAD_SIZE"
#endif
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NELMA_ENC_PACKET_SIZE (NELMA_MAX_PACKET_SIZE + (NELMA_MAX_PACKET_SIZE / 254) + ((NELMA_MAX_PACKET_SIZE % 254) ? 1 : 0))
#define NELMA_BUFFER_SIZE (NELMA_ENC_PACKET_SIZE + 1)

struct Nelma {
  uint8_t encoded_packet[NELMA_BUFFER_SIZE];
  uint8_t packet[NELMA_MAX_PACKET_SIZE];

  size_t request_size;
  size_t response_size;
};

bool nelma_read(struct Nelma *nelma, uint8_t input_byte, size_t *out_packet_size);
bool nelma_write(struct Nelma *nelma, uint8_t data);
bool nelma_write_array(struct Nelma *nelma, uint8_t *data, size_t len);
size_t nelma_encode_response(struct Nelma *nelma);

#endif // NELMA_H
