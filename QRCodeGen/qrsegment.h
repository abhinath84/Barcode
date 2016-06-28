/**
*  @file    qrsegment.h
*  @brief   class to handle data segment for QR Code.
*
*  QRSegment class encode the input string according to the QR Code
*  specification and provides functionality to use encoded data(in bytes format)
*  for further manipulation.
*  It also store size of input data, size of encoded bytes which will be useful
*  for futher calculation for QR Code.
*  Object of this class will use as a input for QRCode class (internally or externally).
*
*  @author  Abhishek Nath
*  @date    20-June-2016
*
*/


#ifndef QRSEGMENT_H
#define QRSEGMENT_H

#include <algorithm>
#include <vector>
#include <iterator>

#include "qrutility.h"


namespace QR
{
  //!  @class  QRSegment
  /*!
    Represents a character string to be encoded in a QR Code symbol. Each segment has
    a mode, and a sequence of characters that is already encoded as a sequence of bits.
    This segment class imposes no length restrictions, but QR Codes have restrictions.
    Even in the most favorable conditions, a QR Code can only hold 7089 characters of data.
    Any segment longer than this is meaningless for the purpose of generating QR Codes.
  */
  class QRSegment
  {
    public:
      /// Default Constructor
      QRSegment();

      /// Parametric Constructor
      QRSegment(const DATA_MODE &md, int inputSize, const ui8vector &bits, int bitSize);

      /// Copy Constructor
      QRSegment(const QRSegment &other);

      /// Destructor
      ~QRSegment();

      /// Assignment Operator
      QRSegment& operator=(const QRSegment &other);

 //<!
 //<! get methods for member variables
 //<!

      /** @brief get size of Character Count Indicator.
      *
      *  Get size of Character Count Indicator according to the
      *  DATA_MODE of this segment and passing version number.
      *
      *  @param[in]   version the number of version.
      *
      *  @return size of Character Count Indicator.
      */
      int getCharCountIndicatorSize(int version) const;

      /** @brief get size of Input string/data.
      *
      *  @param[in] nothing.
      *
      *  @return int size of input string/data.
      */
      int getInputSize() const;

      /** @brief get size of encoded data(in bits format) of this segment.
      *
      *  @param[in] nothing.
      *
      *  @return int size of encoded data.
      */
      int getBitSize() const;

      /** @brief get DATA_MODE of this segment.
      *
      *  @param[in] nothing.
      *
      *  @return DATA_MODE
      */
      DATA_MODE getMode() const;

      /** @brief get element from encoded data bits for passing position.
      *
      *  get element for the passing position from encoded data bits.
      *  pos value must be >=0 and < m_bits.size(). Otherwise it would return -1.
      *
      *  @param[in] pos from which position retrive the element in encoded data bits.
      *
      *  @return uint8_t
      */
      uint8_t   at(int pos) const;

      /// Create Segment according to the input type,like: numeric, alphanumeric, kanji, byte (UTF-8)
      void create(const std::string &input);
      void create(const std::vector<uint8_t> &data);

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
