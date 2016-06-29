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
      uint8_t at(int pos) const;


 //<!
 //<! Create Segment according to the input string type, like: numeric, alphanumeric, kanji, byte (UTF-8)
 //<!

      /** @brief encode input string and create segment for the input string/data.
      *
      *  Encode input string/data according to their DATA_MODE.
      *  Then fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] input the input string to encode.
      *
      *  @return nothing.
      */
      void create(const std::string &input);

      /** @brief encode input string and create segment for the input string/data.
      *
      *  Encode input string/data according to their DATA_MODE.
      *  Then fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] data the data in bit format to encode.
      *
      *  @return nothing.
      */
      void create(const std::vector<uint8_t> &data);

    private:
      /** @brief set input string in bit format in m_bits member variable.
      *
      *  set input string in bit format in m_bits.
      *  If m_bits contain any value previously, then that values will be erased
      *  and m_bits will be filled by new bits.
      *
      *  @param[in] input the input string to set in bit format.
      *
      *  @return bool true  -> If set successfully
      *               false -> If not.
      */
      bool setBits(const std::string &input);

      /** @brief set all the member variables with corresponding input.
      *
      *  @param[in] dm the DATA_MODE for this segment.
      *  @param[in] inputSize the size of input string/data for this segment.
      *  @param[in] bits the encoded data for this segment.
      *  @param[in] bitSize the size of encoded data for this segment.
      *
      *  @return bool true  -> If set successfully
      *               false -> If not.
      */
      bool setSegment(const DATA_MODE &dm, int inputSize, const ui8vector &bits, int bitSize);

      /** @brief encode input string in numeric mode (DM_NUM).
      *
      *  Check input string is digits or not. If it's digit, 
      *  then encode input string in numeric mode.
      *  Afterwards fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] input the input string to encode.
      *
      *  @return nothing.
      */
      void createNumeric(const std::string &input);

      /** @brief encode input string in alphanumeric mode (DM_AN).
      *
      *  Check input string is alphanumeric or not. The characters allowed are:
      *  0 to 9, A to Z (uppercase only), space, dollar, percent, asterisk, plus, hyphen, period, slash, colon.
      *  If so, then encode input string in alphanumeric mode.
      *  Afterwards fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] input the input string to encode.
      *
      *  @return nothing.
      */
      void createAlphanumeric(const std::string &input);

      /** @brief encode input string in binary mode (DM_8).
      *
      *  Check input string is in binary form or not (UTF-8).
      *  If so, then encode input string in binary mode.
      *  Afterwards fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] input the input string to encode.
      *
      *  @return nothing.
      */
      void createBytes(const std::string &input);

      /** @brief encode input string in binary mode (DM_8).
      *
      *  Check input string is in binary form or not (UTF-8).
      *  If so, then encode input string in binary mode.
      *  Afterwards fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] data the data in bit format to encode.
      *
      *  @return nothing.
      */
      void createBytes(const ui8vector &data);

      /** @brief encode input string in kanji mode (DM_KANJI).
      *
      *  Check input string is in kanji form or not (Japanese characters).
      *  If so, then encode input string in kanji mode.
      *  Afterwards fill all the member variables of QRSegment class for the
      *  input string/data.
      *
      *  @param[in] input the input string to encode.
      *
      *  @return nothing.
      */
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
