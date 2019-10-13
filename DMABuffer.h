/* Author : Mudassar Tamboli
 * License : APACHE LICENSE
 * URL: https://github.com/mudassar-tamboli/ESP32-OV7670-WebSocket-Camera/blob/master/OV7670.h
 * DESC: Direct Memory Access 코드?????
 */



#pragma once
#include <stdlib.h>
class DMABuffer
{
  public:
  lldesc_t descriptor;
  unsigned char* buffer;
  DMABuffer(int bytes)
  {
    buffer = (unsigned char *)malloc(bytes);
    descriptor.length = bytes;
    descriptor.size = descriptor.length;
    descriptor.owner = 1;
    descriptor.sosf = 1;
    descriptor.buf = (uint8_t*) buffer;
    descriptor.offset = 0;
    descriptor.empty = 0;
    descriptor.eof = 1;
    descriptor.qe.stqe_next = 0;
  }

  void next(DMABuffer *next)
  {
    descriptor.qe.stqe_next = &(next->descriptor);
  }

  int sampleCount() const
  {
    return descriptor.length / 4;
  }

  ~DMABuffer()
  {
    if(buffer)
      delete(buffer);
  }
};
