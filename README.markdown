# Nelma

Nelma is a tiny library that provides the necessary boilerplate to easily
implement proper framing for a serial communications protocol.
It uses a [Consistent Overhead Byte Stuffing](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) (COBS)
library [by Jacques Fortier](https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing).

It provides:

* A zero-delimited packet protocol
* A way to detect packet boundaries in a stream of bytes
* A simple request-response mechanism. A response can be simply
  an acknowledgement or can have contents. Using the response mechanism
  is not mandatory.

It does not provide:

* Sequence numbering of packets
* Checksumming or any other kind of integrity check
* Any basic building blocks for full-duplex communication. It's possible
  to implement such things, but Nelma is best suited for a simple
  request-response communication, where requests are always initiated
  by the same party. For example, an Arduino taking commands from a PC.

## Nelma vs NelmaX

NelmaX is an extension to Nelma that extends the protocol with a 8-bit
command byte. This provides a very useful basic protocol for most
simple request-response use cases.

Example code for Arduino:

```c
// Arduino doesn't support adding defines outside code, so
// add something like this to the top of nelmax.h and nelma.h:
// #define NELMAX_MAX_PAYLOAD_SIZE 8
#include "nelmax.h"

static struct NelmaX NELMAX = {0};

void setup() {
  Serial.begin(115200);
}

void loop() {
  byte command;
  size_t payloadSize;

  int maxBytes = Serial.available();
  while (maxBytes--) {
    byte inputByte = Serial.read();
    if (nelmax_read(&NELMAX, inputByte, &command, &payloadSize)) {
      uint8_t *payload = nelmax_payload(&NELMAX);

      // Perform operations based on command, payload, and payloadSize
      // Call nelmax_write(&NELMAX, byte_value) or
      //      nelmax_write_array(&NELMAX, ptr, len)
      // to prepare a response

      if (command == 0x42) {
        nelmax_write(&NELMAX, 0x24);
      }

      size_t responseSize = nelmax_encode_response(&NELMAX);
      Serial.write(nelmax_encoded_packet(&NELMAX), responseSize);
    }
  }
}
```

## License and copyright

Nelma is released under The Unlicense, so it is in public domain.
The COBS library by Jacques Fortier is licensed under a
conditionless variant of the BSD license.
