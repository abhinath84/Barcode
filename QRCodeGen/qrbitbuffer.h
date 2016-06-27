#ifndef QRBITBUFFER_H
#define QRBITBUFFER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "qrutility.h"
#include "qrsegment.h"

namespace QR
{
  class QRBitBuffer
  {
    public:
      // Creates an empty bit buffer (length 0).
      QRBitBuffer();
      QRBitBuffer(const QRBitBuffer &other);
      ~QRBitBuffer(void);

      QRBitBuffer& operator=(const QRBitBuffer &other);

      // Methods
      // Returns the number of bits in the buffer, which is a non-negative value.
      int getBitLength() const;

      // Returns a copy of all bytes, padding up to the nearest byte.
      ui8vector getBytes() const;

      // Appends the given number of bits of the given value to this sequence.
      // If 0 <= len <= 31, then this requires 0 <= val < 2^len.
      void appendBits(uint32_t val, int len);

      // Appends the data of the given segment to this bit buffer.
      void appendData(const QRSegment &seg);

    private:
      ui8vector  m_bits;
      int        m_len;
  };
}

#endif    // QRBITBUFFER_H
