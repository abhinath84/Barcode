#ifndef QRSEGMENT_H
#define QRSEGMENT_H

#include <vector>
#include "qrutility.h"

namespace QR
{
  class QRSegment
  {
    public:
      QRSegment(void);
      ~QRSegment(void);

      int getBitSize() const;
      uint8_t at(int pos) const;

    private:
      /* The mode indicator for this segment. */
      DATA_MODE m_mode;

      /* The length of this segment's unencoded data, measured in characters. Always zero or positive. */
      const int m_inputSize;

      /* The bits of this segment packed into a byte array in big endian. */
      const std::vector<uint8_t> m_bits;

      /* The length of this segment's encoded data, measured in bits. Satisfies ceil(bitLength / 8) = data.size(). */
      const int m_bitSize;
  };
}

#endif    // QRSEGMENT_H
