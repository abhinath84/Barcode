#include "qrcode.h"

using namespace QR;

const int16_t QRCode::NUM_ERROR_CORRECTION_CODEWORDS[4][41] = {
  // Version: (note that index 0 is for padding, and is set to an illegal value)
  //0,  1,  2,  3,  4,  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40    Error correction level
  {-1,  7, 10, 15, 20, 26,  36,  40,  48,  60,  72,  80,  96, 104, 120, 132, 144, 168, 180, 196, 224, 224, 252, 270, 300,  312,  336,  360,  390,  420,  450,  480,  510,  540,  570,  570,  600,  630,  660,  720,  750},  // Low
  {-1, 10, 16, 26, 36, 48,  64,  72,  88, 110, 130, 150, 176, 198, 216, 240, 280, 308, 338, 364, 416, 442, 476, 504, 560,  588,  644,  700,  728,  784,  812,  868,  924,  980, 1036, 1064, 1120, 1204, 1260, 1316, 1372},  // Medium
  {-1, 13, 22, 36, 52, 72,  96, 108, 132, 160, 192, 224, 260, 288, 320, 360, 408, 448, 504, 546, 600, 644, 690, 750, 810,  870,  952, 1020, 1050, 1140, 1200, 1290, 1350, 1440, 1530, 1590, 1680, 1770, 1860, 1950, 2040},  // Quartile
  {-1, 17, 28, 44, 64, 88, 112, 130, 156, 192, 224, 264, 308, 352, 384, 432, 480, 532, 588, 650, 700, 750, 816, 900, 960, 1050, 1110, 1200, 1260, 1350, 1440, 1530, 1620, 1710, 1800, 1890, 1980, 2100, 2220, 2310, 2430},  // High
};

const int8_t QRCode::NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
  // Version: (note that index 0 is for padding, and is set to an illegal value)
  //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
  {-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
  {-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
  {-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
  {-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};

QRCode::QRCode(void)
{
}


QRCode::~QRCode(void)
{
}

int QRCode::getECLFormatBits()
{
  int val = -1;

  if((m_ecl>=0) && (m_ecl < 4))
  {
    int eclformatBits[] = {1, 0, 3, 2};
    val = eclformatBits[m_ecl];;
  }

  return(val);
}

int QRCode::getTotalBits(const std::vector<QRSegment> &segs, int version) 
{
  if (version < 1 || version > 40)
    throw "Version number out of range";

  int result = 0;
  for (size_t i = 0; i < segs.size(); i++) 
  {
    int ccbits = segs[i].getCharCountIndicatorSize(version);

    // Fail if segment length value doesn't fit in the length field's bit-width
    if (segs[i].getInputSize() >= (1 << ccbits))
      return -1;
    
    result += 4 + ccbits + segs[i].getBitSize();
  }
  return result;
}

void QRCode::encodeText(const std::string &input, const ECL &ecl) 
{
  QRSegment seg;
  seg.make(input);

  std::vector<QRSegment> segs;
  segs.push_back(seg);
  return (encodeSegments(segs, ecl));
}

void QRCode::encodeBinary(const std::vector<uint8_t> &data, const ECL &ecl) 
{
  QRSegment seg;
  seg.make(data);

  std::vector<QRSegment> segs;
  segs.push_back(seg);

  return encodeSegments(segs, ecl);
}

void QRCode::encodeSegments(const std::vector<QRSegment> &segs, const ECL &ecl,
                              int minVersion, int maxVersion, int mask, bool boostEcl) 
{
  if (!(1 <= minVersion && minVersion <= maxVersion && maxVersion <= 40) ||
      (mask < -1) || (mask > 7))
    throw "Invalid value";

  // Find the minimal version number to use
  int version, dataUsedBits;
  for (version = minVersion; ; version++) 
  {
    int dataCapacityBits = getDataCodewordsCount(version, ecl) * 8;  // Number of data bits available

    dataUsedBits = getTotalBits(segs, version);
    if (dataUsedBits != -1 && dataUsedBits <= dataCapacityBits)
      break;  // This version number is found to be suitable

    if (version >= maxVersion)  // All versions in the range could not fit the given data
      throw "Data too long";
  }

  if (dataUsedBits == -1)
    throw "Assertion error";

  // Increase the error correction level while the data still fits in the current version number
  ECL newEcl = ecl;
  if (boostEcl) 
  {
    if (dataUsedBits <= getDataCodewordsCount(version, ECL_M ) * 8)  newEcl = ECL_M;
    if (dataUsedBits <= getDataCodewordsCount(version, ECL_Q ) * 8)  newEcl = ECL_Q;
    if (dataUsedBits <= getDataCodewordsCount(version, ECL_H ) * 8)  newEcl = ECL_H;
  }
	
  // Create the data bit string by concatenating all segments
  int dataCapacityBits = getDataCodewordsCount(version, newEcl) * 8;
  QRBitBuffer bits;
  for (size_t i = 0; i < segs.size(); i++) 
  {
    const QRSegment &seg(segs.at(i));
    bits.appendBits(seg.getMode(), 4);
    bits.appendBits(seg.getInputSize(), seg.getCharCountIndicatorSize(version));
    bits.appendData(seg);
  }

  // Add terminator and pad up to a byte if applicable
  bits.appendBits(0, std::min(4, dataCapacityBits - bits.getBitLength()));
  bits.appendBits(0, (8 - bits.getBitLength() % 8) % 8);

  // Pad with alternate bytes until data capacity is reached
  for (uint8_t padByte = 0xEC; bits.getBitLength() < dataCapacityBits; padByte ^= 0xEC ^ 0x11)
    bits.appendBits(padByte, 8);

  if (bits.getBitLength() % 8 != 0)
    throw "Assertion error";  

  // Create the QR Code symbol
  makeQRCode(version, newEcl, bits.getBytes(), mask);
}

int QRCode::getRawDataModulesCount(int version)
{
  if (version < 1 || version > 40)
    throw "Version number out of range";

  int result = (16 * version + 128) * version + 64;
  if (version >= 2) 
  {
    int numAlign = version / 7 + 2;
    result -= (25 * numAlign - 10) * numAlign - 55;
    if (version >= 7)
      result -= 18 * 2;  // Subtract version information
  }
  return result;
}

int QRCode::getDataCodewordsCount(int version, const ECL &ecl) 
{
  if (version < 1 || version > 40)
    throw "Version number out of range";

  return (getRawDataModulesCount(version) / 8 - NUM_ERROR_CORRECTION_CODEWORDS[ecl][version]);
}

void QRCode::makeQRCode(int version, const ECL &ecl, const std::vector<uint8_t> &dataCodewords, int mask) 
{
		// Initialize scalar fields
		this->m_version = version;
    this->m_size = (1 <= version && version <= 40 ? version * 4 + 17 : -1);  // Avoid signed overflow undefined behavior
    this->m_ecl = ecl; 
	
	// Check arguments
	if (
      (version < 1) || (version > 40) || 
      (mask < -1) || (mask > 7)
     )
    throw "Value out of range";

  std::vector<bool> row(m_size);
  for (int i = 0; i < m_size; i++) 
  {
    m_modules.push_back(row);
    m_isFunction.push_back(row);
  }
	
  // Draw function patterns, draw all codewords, do masking
  drawFunctionPatterns();
  const std::vector<uint8_t> allCodewords(appendErrorCorrection(dataCodewords));
  drawCodewords(allCodewords);
  this->mask = handleConstructorMasking(mask);
}
