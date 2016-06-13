#include "qrdataencode.h"

using namespace QR;

//!< QRDataEncode

const int16_t QRDataEncode::NUM_ERROR_CORRECTION_CODEWORDS[4][41] = 
{
  // Version: (note that index 0 is for padding, and is set to an illegal value)
  //0,  1,  2,  3,  4,  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40    Error correction level
  {-1,  7, 10, 15, 20, 26,  36,  40,  48,  60,  72,  80,  96, 104, 120, 132, 144, 168, 180, 196, 224, 224, 252, 270, 300,  312,  336,  360,  390,  420,  450,  480,  510,  540,  570,  570,  600,  630,  660,  720,  750},  // Low
  {-1, 10, 16, 26, 36, 48,  64,  72,  88, 110, 130, 150, 176, 198, 216, 240, 280, 308, 338, 364, 416, 442, 476, 504, 560,  588,  644,  700,  728,  784,  812,  868,  924,  980, 1036, 1064, 1120, 1204, 1260, 1316, 1372},  // Medium
  {-1, 13, 22, 36, 52, 72,  96, 108, 132, 160, 192, 224, 260, 288, 320, 360, 408, 448, 504, 546, 600, 644, 690, 750, 810,  870,  952, 1020, 1050, 1140, 1200, 1290, 1350, 1440, 1530, 1590, 1680, 1770, 1860, 1950, 2040},  // Quartile
  {-1, 17, 28, 44, 64, 88, 112, 130, 156, 192, 224, 264, 308, 352, 384, 432, 480, 532, 588, 650, 700, 750, 816, 900, 960, 1050, 1110, 1200, 1260, 1350, 1440, 1530, 1620, 1710, 1800, 1890, 1980, 2100, 2220, 2310, 2430},  // High
};

const int QRDataEncode::CAPACITY_TABLE[][4] = {
  {  19,   16,   13,	9},{  34,   28,   22,   16},{  55,   44,   34,   26},{  80,   64,   48,   36},
  { 108,   86,   62,   46},{ 136,  108,   76,   60},{ 156,  124,   88,   66},{ 194,  154,  110,   86},
  { 232,  182,  132,  100},{ 274,  216,  154,  122},{ 324,  254,  180,  140},{ 370,  290,  206,  158},
  { 428,  334,  244,  180},{ 461,  365,  261,  197},{ 523,  415,  295,  223},{ 589,  453,  325,  253},
  { 647,  507,  367,  283},{ 721,  563,  397,  313},{ 795,  627,  445,  341},{ 861,  669,  485,  385},
  { 932,  714,  512,  406},{1006,  782,  568,  442},{1094,  860,  614,  464},{1174,  914,  664,  514},
  {1276, 1000,  718,  538},{1370, 1062,  754,  596},{1468, 1128,  808,  628},{1531, 1193,  871,  661},
  {1631, 1267,  911,  701},{1735, 1373,  985,  745},{1843, 1455, 1033,  793},{1955, 1541, 1115,  845},
  {2071, 1631, 1171,  901},{2191, 1725, 1231,  961},{2306, 1812, 1286,  986},{2434, 1914, 1354, 1054},
  {2566, 1992, 1426, 1096},{2702, 2102, 1502, 1142},{2812, 2216, 1582, 1222},{2956, 2334, 1666, 1276}
};

QRDataEncode::QRDataEncode()
  : m_mode(0),
  m_input(),
  m_ecl(ECL_L),
  m_version(0),
  m_width(),
  m_encodedData()
{
}

QRDataEncode::QRDataEncode(const string &input, const ECL &ecl)
  : m_mode(0),
  m_input(input),
  m_ecl(ecl),
  m_version(0),
  m_width(input.size()),
  m_encodedData()
{
}

QRDataEncode::QRDataEncode(const QRDataEncode &other)
  : m_mode(other.m_mode),
  m_input(other.m_input),
  m_ecl(other.m_ecl),
  m_version(other.m_version),
  m_width(other.m_width),
  m_encodedData(other.m_encodedData)
{
}

QRDataEncode& QRDataEncode::operator=(const QRDataEncode &other)
{
  if(this != &other)
  {
    setMode(other.m_mode);
    setInput(other.m_input);
    setErrorCorrectionLevel(other.m_ecl);
    setVersion(other.m_version);
    setEncodedData(other.m_encodedData);
  }

  return(*this);
}

QRDataEncode::~QRDataEncode()
{
}

int QRDataEncode::getMode() const
{
  return(m_mode);
}

void QRDataEncode::setMode(int mode)
{
  if((mode > 0) && (mode < 4))
    m_mode = mode;
}

string QRDataEncode::getInput() const
{
  return m_input;
}

void QRDataEncode::setInput(const string &input)
{
  if(m_input.compare(input) != 0)
  {
    m_input.clear();
    m_input.replace(m_input.begin(), m_input.end(), input);
    m_width = input.length();
  }
}

ECL QRDataEncode::getErrorCorrectionLevel() const
{
  return m_ecl;
}

void QRDataEncode::setErrorCorrectionLevel(const ECL &ecl)
{
  if(((ecl >= ECL_L) && (ecl <= ECL_H)) && (m_ecl != ecl))
    m_ecl = ecl;
}

int QRDataEncode::getVersion() const
{
  return m_version;
}

void QRDataEncode::setVersion(int version)
{
  if(((version > 0) && (version <= 40)) && (m_version != version))
    m_version = version;
}

int QRDataEncode::getWidth() const
{
  return m_width;
}

void QRDataEncode::setWidth(int width)
{
  m_width = width;
}

void QRDataEncode::appendBit(uint8_t val)
{
  //m_bits.append(val);
}

void QRDataEncode::appendBit(uint32_t val, int len)
{
  //m_bits.append(val, len);
}

bool QRDataEncode::appendPadBits()
{
  int dataCapacityBits = getNumDataCodewords() * 8;

  /// append at max '0000' bits if current encoded data length is less than dataCapacityBits
  int minimum = min(4, (int)(dataCapacityBits - m_encodedData.size()));
  for(int i = 1; i < minimum; ++i)
    m_encodedData.push_back('0');

  /// Add More 0s to Make the Length a Multiple of 8
  int byte = (8 - (m_encodedData.size() % 8)) % 8;
  for(int i = 0; i < byte; ++i)
    m_encodedData.push_back('0');

  /// Add Pad Bytes if the String is Still too Short
  int diff = dataCapacityBits - m_encodedData.size();
  uint8_t padded[] = {'1','1','1','0','1','1','0','0', '0','0','0','1','0','0','0','1'};
  int c_pad = 0;
  for(int i = 0 ; i < diff; ++i)
  {
    m_encodedData.push_back(padded[c_pad]);
    c_pad++;
    c_pad = (c_pad == 16) ? 0 : c_pad;
  }

  return(true);
}

bool QRDataEncode::convertAndAppend(int val, int size)
{
  bool status = false;
  vector<uint8_t> bits;
    
  convertToByte(val, bits);
  if(bits.size() > 0)
    m_encodedData.push_back(bits, 10);

  return(status);
}

int QRDataEncode::getNumRawDataModules()
{
  int result = (16 * m_version + 128) * m_version + 64;

  if (m_version >= 2) 
  {
    int numAlign = m_version / 7 + 2;
    result -= (25 * numAlign - 10) * numAlign - 55;
    if (m_version >= 7)
      result -= 18 * 2;  // Subtract version information
  }

  return result;
}


int QRDataEncode::getNumDataCodewords() 
{
  return getNumRawDataModules() / 8 - NUM_ERROR_CORRECTION_CODEWORDS[m_ecl][m_version];
}

QRBitBuffer QRDataEncode::getEncodedData()const
{
  return(m_encodedData);
}

void QRDataEncode::setEncodedData(const vector<uint8_t> &data)
{
  m_encodedData.clear();
  m_encodedData.push_back(data);
}

void QRDataEncode::setEncodedData(const QRBitBuffer &data)
{
  m_encodedData = data;
}

int QRDataEncode::calculateCharacterCapacity(int mode, int version, const ECL &ecl)
{
  int   bits, digits, modebits, capacity = 0;

  if(
      ((mode > 0) && (mode <= 4)) &&
      ((version > 0) && (version <= 40)) &&
      ((ecl >= 0) && (mode < 4))
    )
  {
    bits = CAPACITY_TABLE[version - 1][ecl] * 8;
    // the mode indicator
    bits = bits - 4;
    digits = getCCILength(version, ecl);

    modebits = bits - digits;
    if (mode == 1)                        // numeric
      capacity = (int)floor(modebits * 3 / 10);
    else if (mode == 2)                   // alphanumeric
      capacity = (int)floor(modebits * 2 / 11);
    else if (mode == 3)                   // binary
      capacity = (int)floor(modebits * 1 / 8);
    else
      capacity = (int)floor(modebits * 1 / 13);
  }

  return(capacity);
}

bool QRDataEncode::calculateVersion()
{
  bool  status = false;
  int   capacity;

  if((m_mode > 0) && (m_mode <= 4))
  {
    for(int i = m_ecl; i <= ECL_H; ++i)   // ECL
    {
      for(int j = 0; j < 40; ++j)         // version
      {
        /// i = ECL, j = version
        capacity = calculateCharacterCapacity(m_mode, (j + 1), (ECL)i);
        if (capacity >= m_width)
        {
          m_version = j + 1;
          if(m_ecl != i)
            m_ecl = (ECL)i;

          status = true;
          break;
        }
      }

      if(status)
        break;
    }
  }

  return(status);
}

bool QRDataEncode::calculateErrorCorrectionLevel()
{
  bool status = false;
  int  capacity = 0;

  if(m_version > 0 && m_version <= 40)
  {
    for(int i = (m_ecl + 1); i <= ECL_H; ++i)
    {
      capacity = calculateCharacterCapacity(m_mode, m_version, (ECL)i);
      if((int)m_input.length() > capacity)
      {
        m_ecl = (ECL)(i - 1);
        status = true;
        break;
      }
    }

    status = true;
  }

  return(status);
}

bool QRDataEncode::addModeIndicator()
{
  if(m_mode == 1)
  {
    m_encodedData.push_back('0');
    m_encodedData.push_back('0');
    m_encodedData.push_back('0');
    m_encodedData.push_back('1');
  }
  else if(m_mode == 2)
  {
    m_encodedData.push_back('0');
    m_encodedData.push_back('0');
    m_encodedData.push_back('1');
    m_encodedData.push_back('0');
  }

  return(true);
}

bool QRDataEncode::addCharCountIndicator()
{
  bool status = false;

  /// get length
  int len = getCCILength(m_version, (m_mode - 1));

  if(len > 0)
  {
    /// convert input length into bit and pad extra bit by '0'
    vector<uint8_t> cci;

    convertToByte(m_input.length(), cci);
    if(cci.size() > 0)
      m_encodedData.push_back(cci, len);

    status = true;
  }

  return(status);
}

int QRDataEncode::getCCILength(int version, int datamode)
{
  int len = 0;
  const int QRDataEncode_CCLT[][4] = {
                                        /// Numeric, Alphanumeric, Byte, Kanji
                                        {10, 9, 8, 8},      /// 1 - 9
                                        {12, 11, 16, 10},   /// 10 - 26
                                        {14, 13, 16, 12}    /// 27 - 40
                                      };

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

bool QRDataEncode::encodeData()
{
  bool status = false;
  int split = 0, c_split = 0;
  int len = m_input.length();

  if(len > 0)
  {
    status = true;

    for(int i = 0; i < len; ++i)
    {
      if(status)
      {
        /// split number into group of 3-digits
        split = split * 10 + (m_input[i] - '0');
        c_split++;

        if(c_split == 3)
        {
          /// convert split digit and pad extra bit by '0'
          status = convertAndAppend(split, 10);

          /// merge into m_encodedData
          split = c_split = 0;
        }
      }
    }

    if((status) && (c_split > 0))
    {
      if(c_split == 1)
        status = convertAndAppend(split, 4);
      else
        status = convertAndAppend(split, 7);
    }

    /// Break up into 8-bit codewords and add padded bytes if needed
    if(status)
      status = appendPadBits();
  }

  return(status);
}

bool QRDataEncode::encode()
{
  bool status = false;

  /// 2.1 Choose the Error Correction Level (Pass as input)
  ///     and check data mode also.
  if(
      (m_ecl >= ECL_L) && (m_ecl <= ECL_H) &&
      ((m_mode > 0) && (m_mode <= 4))
    )
  {
    /// 2.2 Smallest version of the data
    status = calculateVersion();
    if(status) status = calculateErrorCorrectionLevel();

    /// 2.3 Add the Mode Indicator
    if(status) status = addModeIndicator();

    /// 2.4 Add the Character Count Indicator
    if(status) status = addCharCountIndicator();

    /// 2.5 Encode input data using selected data mode
    /// 2.6 Break up into 8-bit codewords and add padded bytes if needed
    if(status) status = encodeData();
  }

  return(status);
}
