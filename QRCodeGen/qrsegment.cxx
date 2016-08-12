#include "qrbitbuffer.h"
#include "qrsegment.h"

using namespace QR;

/// Default Constructor
QRSegment::QRSegment()
  :m_mode(DM_NUL),
  m_inputSize(0),
  m_bits(),
  m_bitSize(0)
{
}

/// Parametric Constructor
QRSegment::QRSegment(const DATA_MODE &md, int inputSize, const ui8vector &bits, int bitSize)
  :m_mode(md),
  m_inputSize(inputSize),
  m_bits(bits),
  m_bitSize(bitSize)
{
}

/// Copy Constructor
QRSegment::QRSegment(const QRSegment &other)
  :m_mode(other.m_mode),
  m_inputSize(other.m_inputSize),
  m_bits(other.m_bits),
  m_bitSize(other.m_bitSize)
{
}

/// Assignment Operator
QRSegment& QRSegment::operator=(const QRSegment &other)
{
  if(this != &other)
  {
    m_mode = other.m_mode;
    m_inputSize = other.m_inputSize;
    m_bitSize = other.m_bitSize;

    std::copy(other.m_bits.begin(), other.m_bits.end(),
      back_inserter(m_bits));
  }

  return(*this);
}

/// Destructor
QRSegment::~QRSegment()
{
}

/** @brief get DATA_MODE of this segment.
*
*  @param[in] nothing.
*
*  @return DATA_MODE
*/
DATA_MODE QRSegment::getMode() const
{
  return(m_mode);
}

/** @brief get size of Input string/data.
*
*  @param[in] nothing.
*
*  @return int size of input string/data.
*/
int QRSegment::getInputSize() const
{
  return(m_inputSize);
}

/** @brief get size of encoded data(in bits format) of this segment.
*
*  @param[in] nothing.
*
*  @return int size of encoded data.
*/
int QRSegment::getBitSize() const
{
  return(m_bitSize);
}

/** @brief get element from encoded data bits for passing position.
*
*  get element for the passing position from encoded data bits.
*  pos value must be >=0 and < m_bits.size(). Otherwise it would return -1.
*
*  @param[in] pos from which position retrive the element in encoded data bits.
*
*  @return uint8_t
*/
uint8_t QRSegment::at(int pos) const
{
  if((pos >=0) && (pos < (int)m_bits.size()))
    return(m_bits[pos]);
  else
    return (-1);
}

/** @brief get size of Character Count Indicator.
*
*  Get size of Character Count Indicator according to the
*  DATA_MODE of this segment and passing version number.
*
*  @param[in]   version the number of version.
*
*  @return size of Character Count Indicator.
*/
int QRSegment::getCharCountIndicatorSize(int version) const
{
  int len = 0;
  int datamode = -1;

  const int QRDataEncode_CCLT[][4] = {
                                        /// Numeric, Alphanumeric, Byte, Kanji
                                        {10, 9, 8, 8},      /// 1 - 9
                                        {12, 11, 16, 10},   /// 10 - 26
                                        {14, 13, 16, 12}    /// 27 - 40
                                      };

  /// get array position according to the DATA_MODE
  if(m_mode == 1)
    datamode = 0;
  else if(m_mode == 2)
    datamode = 1;
  else if(m_mode == 4)
    datamode = 2;
  else if(m_mode == 8)
    datamode = 3;

  if(((datamode >= 0) && (datamode < 4)) &&
      ((version > 0) && (version <= 40)))
  {
    /// get length
    if(version < 10)
      len = QRDataEncode_CCLT[0][datamode];
    else if(version < 27)
      len = QRDataEncode_CCLT[1][datamode];
    else if(version <= 40)
      len = QRDataEncode_CCLT[2][datamode];
  }

  return(len);
}

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
void QRSegment::create(const std::string &input)
{
  /// check for valid input
  if (input.size() > 0)
  {
    // Select the most efficient segment encoding automatically
    if (isDigit(input))
      createNumeric(input);
    else if (isAlphaNumeric(input))
      createAlphanumeric(input);
    /*else if (isKanji(input))
      createKanji(input);*/
    else
      createBytes(input);
  }
}

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
void QRSegment::create(const ui8vector &data)
{
  /// check for valid input
  if (data.size() > 0)
  {
    createBytes(data);
  }
}

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
bool QRSegment::setBits(const std::string &input)
{
  int size = (int) input.size();
  bool status = false;

  /// check for valid input
  if(size > 0)
  {
    // reset
    m_bits.clear();
    for (int i = 0; i < size; ++i)
      m_bits.push_back(static_cast<uint8_t>(input[i]));

    status = true;
  }

  return(status);
}

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
bool QRSegment::setSegment(const DATA_MODE &dm, int inputSize, 
                            const ui8vector &bits, int bitSize)
{
  bool status = false;

  /// check for valid inputs
  if((dm >= 0) &&
      (inputSize > 0) &&
      (bits.size() > 0) &&
      (bitSize > 0)
    )
  {
    this->m_mode = dm;
    this->m_inputSize = inputSize;
    this->m_bitSize = bitSize;

    this->m_bits.clear();
    std::copy(bits.begin(), bits.end(),
              back_inserter(this->m_bits));

    status = true;
  }

  return(status);
}

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
void QRSegment::createNumeric(const std::string &input)
{
  int size = input.size();

  /// check for valid input string
  if(size > 0)
  {
    QRBitBuffer bits;
    int split = 0, c_split = 0;

    //<!
    /* Encode procedure for numeric mode:
    *  1. Break String up into groups of Three number.
    *     Like: if input = 1234567
    *     then break them into = 123 456 7.
    *  2. Now convert that numbers into an 10bit binary data, 
    *     padding on the left with 0s if necessary.
    *  4. If number(s) is left (means number having 1 or 2 digits)
    *     then convert it into:
    *     for 1-digit = 4-bit binary data
    *     for 2-digits = 7-bit binary data.
    */
    //<!

    /// loop over entire input string
    for (int i = 0; i < size; ++i) 
    {
      /// split the character of input string into 3 digit number.
      split = split * 10 + (input[i] - '0');
      c_split++;

      /// convert numbers into an 10-bit binary data, 
      /// padding on the left with 0s if necessary.
      if (c_split == 3) 
      {
        bits.appendBits(split, 10);
        split = 0;
        c_split = 0;
      }
    }

    /// convert left numbers into:
    /// for 1-digit = 4-bits and for 2-digit = 7-bits. 
    /// padding on the left with 0s if necessary.
    if (c_split > 0)  // 1 or 2 digits remaining
      bits.appendBits(split, c_split * 3 + 1);

    /// Set all the member variables of this segment.
    setSegment(DM_NUM, size, bits.getBytes(), bits.getBitLength());
  }
}

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
void QRSegment::createAlphanumeric(const std::string &input)
{
  int size = input.size();

  /// check for valid input
  if(size > 0)
  {
    QRBitBuffer bits;
    int digit = 0, count = 0;

    //<!
    /* Encode procedure for alphanumeric mode:
    *  1. each alphanumeric character is represented by a number according to the
    *     alphanumeric table.
    *     convert each character of input string to alphanumeric number.
    *  2. For each pair of characters, get the alphanumeric number representation
    *     of the first character and multiply it by 45. 
    *     Then add that number to the alphanumeric number representation of 
    *     the second character.
    *  3. Now convert that number into an 11-bit binary data, 
    *     padding on the left with 0s if necessary.
    *  4. If encoding an odd number of characters, take the numeric representation of the 
    *     final character and convert it into a 6-bit binary data.
    */
    //<!

    ///loop over entire input string
    for (int i = 0; i < size; ++i) 
    {
      /// get alphanumeric value for each character of input string
      digit += getAlphanumeric(input[i]);
      count++;

      // if count == 1, then multiply digit by 45.
      if((count == 1) && (i < (size - 1)))
        digit = digit * 45;

      // if count == 2, create a binary number for each pair
      if(count == 2)
      {
        /// convert number(formed using character pair) into an 11-bit binary data, 
        /// padding on the left with 0s if necessary.
        bits.appendBits(digit, 11);

        // reset digit and count.
        digit = count = 0;
      }
    }

    /// If encoding an odd number of characters, 
    /// take the numeric representation of the final character and 
    /// convert it into a 6-bit binary data.
    if (count > 0)
      bits.appendBits(digit, 6);

    /// Set all the member variables of this segment.
    setSegment(DM_AN, size, bits.getBytes(), bits.getBitLength());
  }
}

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
void QRSegment::createBytes(const std::string &input)
{
  int size = (int) input.size();

  /// check for valid input
  if(size > 0)
  {
    this->m_mode = DM_8;
    this->m_inputSize = size;
    this->m_bitSize = size * 8;
    setBits(input);
  }
}

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
void QRSegment::createBytes(const ui8vector &data)
{
  int size = (int) data.size();

  /// check for valid input
  if(size > 0)
  {
    this->m_mode = DM_8;
    this->m_inputSize = size;
    this->m_bitSize = size * 8;

    // reset
    m_bits.clear();
    std::copy(data.begin(), data.end(),
                back_inserter(m_bits));
  }
}

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
void QRSegment::createKanji(const std::string &input)
{
  /*const char *p;
  int ret;
  int run;

  p = input.c_str();
  while(Split_identifyMode(p, hint) == QR_MODE_KANJI) 
  {
    p += 2;
  }

  run = p - string;
  ret = QRinput_append(input, QR_MODE_KANJI, run, (unsigned char *)string);
  if(ret < 0) return -1;

  return run;*/
}
