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
      /// Default Constructor
      QRSegment();
      QRSegment(const DATA_MODE &md, int inputSize, const ui8vector &bits, int bitSize);
      QRSegment(const QRSegment &other);
      ~QRSegment();

      QRSegment& operator=(const QRSegment &other);

      /// get methods for member variables
      DATA_MODE getMode() const;
      int getCharCountIndicatorSize(int version) const;
      int getInputSize() const;
      int getBitSize() const;
      uint8_t at(int pos) const;

      void create(const std::string &input);
      void create(const ui8vector &data);

    private:
      bool setBits(const std::string &input);
      bool setSegment(const DATA_MODE &dm, int inputSize, const ui8vector &bits, int bitSize);

      void createNumeric(const std::string &input);
      void createAlphanumeric(const std::string &input);
      void createBytes(const std::string &input);
      void createBytes(const ui8vector &data);
      void createKanji(const std::string &input);

    private:
      DATA_MODE m_mode;   ///< Define the mode indicator for this segment.
      int m_inputSize;    ///< Define the size of input character data. Always zero or positive.
      ui8vector m_bits;   ///< Define the bits of this segment packed into a byte array in big endian.
      int m_bitSize;      ///< Define the size of this segment's encoded data, measured in bits. 
                          ///< Satisfies ceil(m_bitsize / 8) = m_bits.size().
  };
}

#endif    // QRSEGMENT_H
