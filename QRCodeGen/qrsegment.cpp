#include "qrbitbuffer.h"
#include "qrsegment.h"

using namespace QR;

QRSegment::QRSegment()
  :m_mode(DM_NUL),
  m_inputSize(0),
  m_bits(),
  m_bitSize(0)
{
}

QRSegment::QRSegment(const DATA_MODE &md, int inputSize, const std::vector<uint8_t> &bits, int bitSize)
  :m_mode(md),
  m_inputSize(inputSize),
  m_bits(bits),
  m_bitSize(bitSize)
{
}

QRSegment::QRSegment(const QRSegment &other)
  :m_mode(other.m_mode),
  m_inputSize(other.m_inputSize),
  m_bits(other.m_bits),
  m_bitSize(other.m_bitSize)
{
}

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

QRSegment::~QRSegment()
{
}

int QRSegment::getInputSize() const
{
  return(m_inputSize);
}

int QRSegment::getBitSize() const
{
  return(1);
}

uint8_t QRSegment::at(int pos) const
{
  return(1);
}

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
  if(m_mode == 0)
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

void QRSegment::make(const std::string &input)
{
  // Select the most efficient segment encoding automatically
  if (input.size() > 0)
  {
    if (isDigit(input))
      makeNumeric(input);
    else if (isAlphaNumeric(input))
      makeAlphanumeric(input);
    else if (isKanji(input))
      nakeKanji(input);
    else
    {
      makeBytes(input);
    }
  }
}

bool QRSegment::setBits(const std::string &input)
{
  int size = (int) input.size();
  bool status = false;

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

bool QRSegment::setSegment(const DATA_MODE &dm, int inputSize, 
                            const std::vector<uint8_t> &bits, int bitSize)
{
  bool status = false;

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

void QRSegment::makeNumeric(const std::string &input)
{
  int size = input.size();

  if(size > 0)
  {
    QRBitBuffer bits;
    int split = 0, c_split = 0;

    for (int i = 0; i < size; ++i) 
    {
      split = split * 10 + (input[i] - '0');
      c_split++;

      if (c_split == 3) 
      {
        bits.appendBits(split, 10);
        split = 0;
        c_split = 0;
      }
    }

    if (c_split > 0)  // 1 or 2 digits remaining
      bits.appendBits(split, c_split * 3 + 1);

    setSegment(DM_NUM, size, bits.getBytes(), bits.getBitLength());
  }
}

void QRSegment::makeAlphanumeric(const std::string &input)
{
  int size = input.size();

  if(size > 0)
  {
    QRBitBuffer bits;
    int digit = 0, count = 0;

    for (int i = 0; i < size; ++i) 
    {
      digit += getAlphanumeric(input[i]);
      count++;

      // if count == 1, collect digit and multiply by 45
      if((count == 1) && (i < (size - 1)))
        digit = digit * 45;

      // if count == 2, collect digit and add with prev val
      if(count == 2)
      {
        // convert to binary (having 11-bits) and store.
        bits.appendBits(digit, 11);

        // reset digit and count.
        digit = count = 0;
      }
    }

    if (count > 0)  // 1 character remaining
      bits.appendBits(digit, 6);

    setSegment(DM_AN, size, bits.getBytes(), bits.getBitLength());
  }
}

void QRSegment::makeBytes(const std::string &input)
{
  int size = (int) input.size();

  if(size > 0)
  {
    this->m_mode = DM_8;
    this->m_inputSize = size;
    this->m_bitSize = size * 8;
    setBits(input);
  }
}

void QRSegment::nakeKanji(const std::string &input)
{

}
