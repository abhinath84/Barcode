#ifndef QRSEGMENT_H
#define QRSEGMENT_H

#include <algorithm>
#include <vector>
#include <iterator>

#include "qrutility.h"

namespace QR
{
  class QRSegment
  {
    public:
      QRSegment();
      QRSegment(const DATA_MODE &md, int inputSize, const std::vector<uint8_t> &bits, int bitSize);
      QRSegment(const QRSegment &other);
      ~QRSegment();

      QRSegment& operator=(const QRSegment &other);

      DATA_MODE getMode() const;
      int getCharCountIndicatorSize(int version) const;
      int getInputSize() const;
      int getBitSize() const;
      uint8_t at(int pos) const;

      void make(const std::string &input);
      void make(const std::vector<uint8_t> &data);

    private:
      bool setBits(const std::string &input);
      bool setSegment(const DATA_MODE &dm, int inputSize, const std::vector<uint8_t> &bits, int bitSize);

      void makeNumeric(const std::string &input);
      void makeAlphanumeric(const std::string &input);
      void makeBytes(const std::string &input);
      void makeBytes(const std::vector<uint8_t> &data);
      void nakeKanji(const std::string &input);

    private:
      /* The mode indicator for this segment. */
      DATA_MODE m_mode;

      /* The length of this segment's unencoded data, measured in characters. Always zero or positive. */
      int m_inputSize;

      /* The bits of this segment packed into a byte array in big endian. */
      std::vector<uint8_t> m_bits;

      /* The length of this segment's encoded data, measured in bits. Satisfies ceil(bitLength / 8) = data.size(). */
      int m_bitSize;
  };
}

#endif    // QRSEGMENT_H
