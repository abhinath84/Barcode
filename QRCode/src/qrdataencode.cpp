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
  : m_input(),
  m_ecl(ECL_L),
  m_version(0),
  m_width(),
  m_encodedData()
{
}

QRDataEncode::QRDataEncode(const string &input, const ECL &ecl)
  : m_input(input),
  m_ecl(ecl),
  m_version(0),
  m_width(input.size()),
  m_encodedData()
{
}

QRDataEncode::QRDataEncode(const QRDataEncode &other)
  : m_input(other.m_input),
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

bool QRDataEncode::calculateVersion()
{
  return false;
}

//bool QRDataEncode::calculateVersion(const int CCT[][4])
//{
//  bool status = false;
//
//  for(int i = m_ecl; i <= ECL_H; ++i)
//  {
//    for(int j = 0; j < 40; ++j)
//    {
//      if(m_width <= CCT[j][i])
//      {
//        m_version = j + 1;
//        if(m_ecl != i)
//          m_ecl = (ECL)i;
//
//        status = true;
//        break;
//      }
//    }
//
//    if(status)
//      break;
//  }
//  
//  return(status);
//}

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

bool QRDataEncode::calculateVersion(int mode)
{
  bool  status = false;
  int   capacity;

  if((mode > 0) && (mode <= 4))
  {
    for(int i = m_ecl; i <= ECL_H; ++i)   // ECL
    {
      for(int j = 0; j < 40; ++j)         // version
      {
        /// i = ECL, j = version
        capacity = calculateCharacterCapacity(mode, (j + 1), (ECL)i);
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
  return false;
}

bool QRDataEncode::calculateErrorCorrectionLevel(int mode)
{
  bool status = false;
  int  capacity = 0;

  if(m_version > 0 && m_version <= 40)
  {
    for(int i = (m_ecl + 1); i <= ECL_H; ++i)
    {
      capacity = calculateCharacterCapacity(mode, m_version, (ECL)i);
      if((int)m_input.length() > capacity)
      {
        m_ecl = (ECL)(i - 1);
        status = true;
        break;
      }
    }
  }

  return(status);
}

bool QRDataEncode::addModeIndicator()
{
  return false;
}

bool QRDataEncode::addCharCountIndicator()
{
  return false;
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

bool QRDataEncode::encode()
{
  return false;
}


//!< QRNumericEncode

/// QRNumeric_CCT[version][ECL].
//const int QRNumericEncode::QRNumeric_CCT[40][4] ={
//      {41, 34, 27, 17}, {77, 63, 48, 34}, {127, 101, 77, 58}, {187, 149, 111, 82}, {255, 202, 144, 106}, {322, 255, 178, 139}, {370, 293, 207, 154}, {461, 365, 259, 202}, {552, 432, 312, 235}, {652, 513, 364, 288},
//      {772, 604, 427, 331}, {883, 691, 489, 374}, {1022, 796, 580, 427}, {1101, 871, 621, 468}, {1250, 991, 703, 530}, {1408, 1082, 775, 602}, {1548, 1212, 876, 674}, {1725, 1346, 948, 746}, {1903, 1500, 1063, 813}, {2061, 1600, 1159, 919},
//      {2232, 1708, 1224, 969}, {2409, 1872, 1358, 1056}, {2620, 2059, 1468, 1108}, {2812, 2188, 1588, 1228}, {3057, 2395, 1718, 1286}, {3283, 2544, 1804, 1425}, {3517, 2701, 1933, 1501}, {3669, 2857, 2085, 1581}, {3909, 3035, 2181, 1677}, {4158, 3289, 2358, 1782},
//      {4417, 3486, 2473, 1897}, {4686, 3693, 2670, 2022}, {4965, 3909, 2805, 2157}, {5253, 4134, 2949, 2301}, {5529, 4343, 3081, 2361}, {5836, 4588, 3244, 2524}, {6153, 4775, 3417, 2625}, {6479, 5039, 3599, 2735}, {6743, 5313, 3791, 2937}, {7089, 5596, 3993, 3057}
//    };

QRNumericEncode::QRNumericEncode()
  :QRDataEncode()
{
}

QRNumericEncode::QRNumericEncode(const string &input, const ECL &ecl)
  :QRDataEncode(input, ecl)
{
}

QRNumericEncode::QRNumericEncode(const QRNumericEncode &other)
  :QRDataEncode(other)
{
}

QRNumericEncode::~QRNumericEncode()
{
}

QRNumericEncode& QRNumericEncode::operator=(const QRNumericEncode &other)
{
  if(this != &other)
  {
    setInput(other.getInput());
    setErrorCorrectionLevel(other.getErrorCorrectionLevel());
    setVersion(other.getVersion());
    setEncodedData(other.getEncodedData());
  }

  return(*this);
}

bool QRNumericEncode::calculateVersion()
{
  return(QRDataEncode::calculateVersion(1));
}

bool QRNumericEncode::calculateErrorCorrectionLevel()
{
  return(QRDataEncode::calculateErrorCorrectionLevel(1));
}

bool QRNumericEncode::addModeIndicator()
{
  m_encodedData.push_back('0');
  m_encodedData.push_back('0');
  m_encodedData.push_back('0');
  m_encodedData.push_back('1');

  return(true);
}

bool QRNumericEncode::addCharCountIndicator()
{
  bool status = false;

  /// get length
  int len = getCCILength(m_version, 0);

  if(len > 0)
  {
    /// convert input length into bit and pad extra bit by '0'
    vector<uint8_t> cci;
    
    convertToByte(m_input.length(), cci);
    if(cci.size() > 0)
      m_encodedData.push_back(cci, len);
  }

  return(status);
}

bool QRNumericEncode::encode()
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
          /// convert splitted digit and pad extra bit by '0'
          status = convertAndAppend(split, 10);

          /// marge into m_encodedData
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

//!< QRAlphanumericEncode

/// QRNumeric_CCT[version][ECL].
//const int QRAlphanumericEncode::QRAlphanumeric_CCT[40][4] ={
//      {41, 34, 27, 17}, 
//    };

QRAlphanumericEncode::QRAlphanumericEncode()
  :QRDataEncode()
{
}

QRAlphanumericEncode::QRAlphanumericEncode(const string &input, const ECL &ecl)
  :QRDataEncode(input, ecl)
{
}

QRAlphanumericEncode::QRAlphanumericEncode(const QRAlphanumericEncode &other)
  :QRDataEncode(other)
{
}

QRAlphanumericEncode::~QRAlphanumericEncode()
{
}

QRAlphanumericEncode& QRAlphanumericEncode::operator=(const QRAlphanumericEncode &other)
{
  if(this != &other)
  {
    setInput(other.getInput());
    setErrorCorrectionLevel(other.getErrorCorrectionLevel());
    setVersion(other.getVersion());
    setEncodedData(other.getEncodedData());
  }

  return(*this);
}

bool QRAlphanumericEncode::calculateVersion()
{
  return(QRDataEncode::calculateVersion(2));
}

bool QRAlphanumericEncode::calculateErrorCorrectionLevel()
{
  return(QRDataEncode::calculateErrorCorrectionLevel(2));
}

bool QRAlphanumericEncode::addModeIndicator()
{
  m_encodedData.push_back('0');
  m_encodedData.push_back('0');
  m_encodedData.push_back('1');
  m_encodedData.push_back('0');

  return(true);
}

bool QRAlphanumericEncode::addCharCountIndicator()
{
  bool status = false;

  /// get length
  int len = getCCILength(m_version, 0);

  if(len > 0)
  {
    /// convert input length into bit and pad extra bit by '0'
    vector<uint8_t> cci;
    
    convertToByte(m_input.length(), cci);
    if(cci.size() > 0)
      m_encodedData.push_back(cci, len);
  }

  return(status);
}

bool QRAlphanumericEncode::encode()
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
          /// convert splitted digit and pad extra bit by '0'
          status = convertAndAppend(split, 10);

          /// marge into m_encodedData
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
