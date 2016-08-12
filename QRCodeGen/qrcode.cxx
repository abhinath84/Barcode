#include "qrcode.h"
#include "bitmap.h"
#include "jpeg.h"

using namespace QR;

const int QRCode::MIN_VERSION = 1;
const int QRCode::MAX_VERSION = 40;

const int QRCode::PENALTY_N1 = 3;
const int QRCode::PENALTY_N2 = 3;
const int QRCode::PENALTY_N3 = 40;
const int QRCode::PENALTY_N4 = 10;

const int16_t QRCode::ERROR_CORRECTION_CODEWORDS[4][41] = {
  // Version: (note that index 0 is for padding, and is set to an illegal value)
  //0,  1,  2,  3,  4,  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40    Error correction level
  {-1,  7, 10, 15, 20, 26,  36,  40,  48,  60,  72,  80,  96, 104, 120, 132, 144, 168, 180, 196, 224, 224, 252, 270, 300,  312,  336,  360,  390,  420,  450,  480,  510,  540,  570,  570,  600,  630,  660,  720,  750},  // Low
  {-1, 10, 16, 26, 36, 48,  64,  72,  88, 110, 130, 150, 176, 198, 216, 240, 280, 308, 338, 364, 416, 442, 476, 504, 560,  588,  644,  700,  728,  784,  812,  868,  924,  980, 1036, 1064, 1120, 1204, 1260, 1316, 1372},  // Medium
  {-1, 13, 22, 36, 52, 72,  96, 108, 132, 160, 192, 224, 260, 288, 320, 360, 408, 448, 504, 546, 600, 644, 690, 750, 810,  870,  952, 1020, 1050, 1140, 1200, 1290, 1350, 1440, 1530, 1590, 1680, 1770, 1860, 1950, 2040},  // Quartile
  {-1, 17, 28, 44, 64, 88, 112, 130, 156, 192, 224, 264, 308, 352, 384, 432, 480, 532, 588, 650, 700, 750, 816, 900, 960, 1050, 1110, 1200, 1260, 1350, 1440, 1530, 1620, 1710, 1800, 1890, 1980, 2100, 2220, 2310, 2430},  // High
};

const int8_t QRCode::ERROR_CORRECTION_BLOCKS[4][41] = {
  // Version: (note that index 0 is for padding, and is set to an illegal value)
  //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
  {-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
  {-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
  {-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
  {-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};

QRCode::QRCode()
  :m_version(1),
  m_size(0),
  m_ecl(ECL_L),
  m_mask(-1),
  m_modules(),
  m_isFunction()
{
}

QRCode::QRCode(const QRCode  &other)
  :m_version(other.m_version),
  m_size(other.m_size),
  m_ecl(other.m_ecl),
  m_mask(other.m_mask),
  m_modules(other.m_modules),
  m_isFunction(other.m_isFunction)
{
}

QRCode::~QRCode()
{
}

QRCode& QRCode::operator=(const QRCode &other)
{
  if(this != &other)
  {
    m_version = other.m_version;
    m_size = other.m_size;
    m_ecl = other.m_ecl;
    m_mask = other.m_mask;
    m_modules = other.m_modules;
    m_isFunction = other.m_isFunction;
  }

  return(*this);
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

int QRCode::getMask() const
{
  return m_mask;
}

int QRCode::getSize() const
{
  return m_size;
}

int QRCode::getModule(int x, int y) const 
{
  if (0 <= x && x < m_size && 0 <= y && y < m_size)
    return (m_modules[y][x] ? 1 : 0);
  else
    return 0;  // Infinite white border
}

std::string QRCode::toSvgString(int border) const
{
  if (border < 0)
    throw "Border must be non-negative";

  std::ostringstream sb;
  sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
  sb << (m_size + border * 2) << " " << (m_size + border * 2) << "\">\n";
  sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\" stroke-width=\"0\"/>\n";
  sb << "\t<path d=\"";
  bool head = true;
  for (int y = -border; y < m_size + border; y++) 
  {
    for (int x = -border; x < m_size + border; x++) 
    {
      if (getModule(x, y) == 1)
      {
        if (head)
          head = false;
        else
          sb << " ";

        sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
      }
    }
  }

  sb << "\" fill=\"#000000\" stroke-width=\"0\"/>\n";
  sb << "</svg>\n";

  return sb.str();
}

void QRCode::encode(const std::string &input, const ECL &ecl, int mask) 
{
  QRSegment seg;
  seg.create(input);

  std::vector<QRSegment> segs;
  segs.push_back(seg);
  
  encode(segs, ecl, mask);
}

void QRCode::encode(const ui8vector &data, const ECL &ecl, int mask) 
{
  QRSegment seg;
  seg.create(data);

  std::vector<QRSegment> segs;
  segs.push_back(seg);

  encode(segs, ecl, mask);
}

//void QRCode::encode(const std::vector<QRSegment> &segs, const ECL &ecl, int mask)
//{
//  encodeSegments(segs, ecl, mask);
//}

//void QRCode::encodeSegments(const std::vector<QRSegment> &segs, const ECL &ecl, int mask, bool boostEcl) 
void QRCode::encode(const std::vector<QRSegment> &segs, const ECL &ecl, int mask)
{
  if (((mask < -1) && (mask > 7)))
    throw "Invalid value";

  // Find the minimal version number to use
  int version, dataUsedBits;
  for (version = MIN_VERSION; ; version++) 
  {
    int dataCapacityBits = getDataCodewordsCount(version, ecl) * 8;  // Number of data bits available

    dataUsedBits = getTotalBits(segs, version);
    if (dataUsedBits != -1 && dataUsedBits <= dataCapacityBits)
      break;  // This version number is found to be suitable

    if (version >= MAX_VERSION)  // All versions in the range could not fit the given data
      throw "Data too long";
  }

  if (dataUsedBits == -1)
    throw "Assertion error";

  // Increase the error correction level while the data still fits in the current version number
  ECL newEcl = ecl;
  //if (boostEcl) 
  //{
    if (dataUsedBits <= getDataCodewordsCount(version, ECL_M ) * 8)  newEcl = ECL_M;
    if (dataUsedBits <= getDataCodewordsCount(version, ECL_Q ) * 8)  newEcl = ECL_Q;
    if (dataUsedBits <= getDataCodewordsCount(version, ECL_H ) * 8)  newEcl = ECL_H;
  //}

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

void QRCode::writeToBMP(const std::string &filename)
{
  if(m_size > 0)
  {
    int OUT_FILE_PIXEL_PRESCALER = 8;
    // Output the bmp file
    Bitmap bmp(m_size * OUT_FILE_PIXEL_PRESCALER, m_size * OUT_FILE_PIXEL_PRESCALER);

    for(int y = 0; y < m_size; y++)
    {
      for(int x = 0; x < m_size ; x++)
      {
        if (getModule(x, y) == 1)
        {
          for(int l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
          {
            for(int n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
            {
              /// As height is a positive number, the bitmap is a bottom-up DIB 
              /// and its origin is the lower-left corner.
              /// So, we to swap row value, so that it's origin become lower-left corner.
              /// So, we swap row value with it's max value.
              /// Like: if current row is 0 and max row height is 24,
              /// then x_pos_swap = 24(max row height) - 0(current row value ) + 1 (as array start's with 0-index).

              int x_pos = l + (x * OUT_FILE_PIXEL_PRESCALER);
              int y_pos = n + (y * OUT_FILE_PIXEL_PRESCALER);
              int x_pos_swap = (m_size * OUT_FILE_PIXEL_PRESCALER) - (x_pos + 1);

              bmp.setPixel(x_pos_swap, 
                            y_pos, 
                            0xff, 0, 0);
            }
          }
        }
      }
    }

    // write to file
    bmp.writeToFile(filename.c_str());
  }
}

void QRCode::writeToPNG(const std::string &filename)
{
  
}

void QRCode::writeToJPEG(const std::string &filename)
{
  if(m_size > 0)
  {
    int OUT_FILE_PIXEL_PRESCALER = 8;

    // Output the jpeg file
    JPEG::Jpeg jpg(m_size * OUT_FILE_PIXEL_PRESCALER, m_size * OUT_FILE_PIXEL_PRESCALER);

    //jpg.setHeight(m_size * OUT_FILE_PIXEL_PRESCALER);
    //jpg.setWidth(m_size * OUT_FILE_PIXEL_PRESCALER);

    for(int y = 0; y < m_size; y++)
    {
      for(int x = 0; x < m_size ; x++)
      {
        if (getModule(x, y) == 1)
        {
          for(int l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
          {
            for(int n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
            {
              jpg.setJPEGPixel(l + (x * OUT_FILE_PIXEL_PRESCALER), 
                            n + (y * OUT_FILE_PIXEL_PRESCALER), 
                            0, 0, 0xff);
            }
          }
        }
      }
    }

    jpg.writeToFile(filename.c_str());
  }
}

std::vector<int> QRCode::getAlignmentPatternPositions(int version) 
{
  if (version < 1 || version > 40)
    throw "Version number out of range";
  else if (version == 1)
    return std::vector<int>();
  else 
  {
    int numAlign = version / 7 + 2;
    int step;
    if (version != 32)
      step = (version * 4 + numAlign * 2 + 1) / (2 * numAlign - 2) * 2;  // ceil((size - 13) / (2*numAlign - 2)) * 2
    else  // C-C-C-Combo breaker!
      step = 26;

    int size = version * 4 + 17;
    
    std::vector<int> result(numAlign);
    result[0] = 6;
    for (int i = numAlign - 1, pos = size - 7; i >= 1; i--, pos -= step)
      result[i] = pos;

    return result;
  }
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

  return (getRawDataModulesCount(version) / 8 - ERROR_CORRECTION_CODEWORDS[ecl][version]);
}

void QRCode::makeQRCode(int version, const ECL &ecl, const ui8vector &dataCodewords, int mask) 
{
  // Initialize scalar fields
  this->m_version = version;
  this->m_size = (1 <= version && version <= 40 ? version * 4 + 17 : -1);  // Avoid signed overflow undefined behavior
  this->m_ecl = ecl;
  this->m_modules.clear();
  this->m_isFunction.clear();

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

  const ui8vector allCodewords(appendErrorCorrection(dataCodewords));
  drawCodewords(allCodewords);
  this->m_mask = handleConstructorMasking(mask);
}

void QRCode::makeQRCode(const QRCode &qr, int mask)
{
  // Copy scalar fields
  this->m_version = qr.m_version;
  this->m_size = qr.m_size;
  this->m_ecl = qr.m_ecl;

  // Check arguments
  if (mask < -1 || mask > 7)
    throw "Mask value out of range";

  // Handle grid fields
  this->m_modules = qr.m_modules;
  this->m_isFunction = qr.m_isFunction;
	
  // Handle masking
  applyMask(qr.m_mask);  // Undo old mask
  this->m_mask = handleConstructorMasking(mask);
}

void QRCode::drawFunctionPatterns() 
{
  // Draw the horizontal and vertical timing patterns
  for (int i = 0; i < m_size; i++) 
  {
    setFunctionModule(6, i, i % 2 == 0);
    setFunctionModule(i, 6, i % 2 == 0);
  }
  
  // Draw 3 finder patterns (all corners except bottom right; overwrites some timing modules)
  drawFinderPattern(3, 3);
  drawFinderPattern(m_size - 4, 3);
  drawFinderPattern(3, m_size - 4);

  // Draw the numerous alignment patterns
  const std::vector<int> alignPatPos(getAlignmentPatternPositions(m_version));
  int numAlign = alignPatPos.size();
  for (int i = 0; i < numAlign; i++) 
  {
    for (int j = 0; j < numAlign; j++) 
    {
      if ((i == 0 && j == 0) || (i == 0 && j == numAlign - 1) || (i == numAlign - 1 && j == 0))
        continue;  // Skip the three finder corners
      else
        drawAlignmentPattern(alignPatPos[i], alignPatPos[j]);
    }
  }

  // Draw configuration data
  drawFormatBits(0);  // Dummy mask value; overwritten later in the constructor
  drawVersion();
}

ui8vector QRCode::appendErrorCorrection(const ui8vector &data) 
{
  if (data.size() != static_cast<unsigned int>(getDataCodewordsCount(m_version, m_ecl)))
    throw "Invalid argument";

  // Calculate parameter numbers
  int numBlocks = ERROR_CORRECTION_BLOCKS[m_ecl][m_version];
  int totalEcc = ERROR_CORRECTION_CODEWORDS[m_ecl][m_version];
  if (totalEcc % numBlocks != 0)
    throw "Assertion error";

  int blockEccLen = totalEcc / numBlocks;
  int numShortBlocks = numBlocks - getRawDataModulesCount(m_version) / 8 % numBlocks;
  int shortBlockLen = getRawDataModulesCount(m_version) / 8 / numBlocks;

  // Split data into blocks and append ECC to each block
  std::vector<ui8vector> blocks;
  const QRReedSolomonGenerator rs(blockEccLen);

  for (int i = 0, k = 0; i < numBlocks; i++) 
  {
    ui8vector dat;

    dat.insert(dat.begin(), data.begin() + k, data.begin() + (k + shortBlockLen - blockEccLen + (i < numShortBlocks ? 0 : 1)));
    k += dat.size();

    ui8vector ecc = rs.getErrorCorrection(dat);

    if (i < numShortBlocks)
      dat.push_back(0);

    dat.insert(dat.end(), ecc.begin(), ecc.end());
    blocks.push_back(dat);
  }

  // Interleave (not concatenate) the bytes from every block into a single sequence
  ui8vector result;
  for (int i = 0; static_cast<unsigned int>(i) < blocks.at(0).size(); i++)
  {
    for (int j = 0; static_cast<unsigned int>(j) < blocks.size(); j++)
    {
      // Skip the padding byte in short blocks
      if (i != shortBlockLen - blockEccLen || j >= numShortBlocks)
        result.push_back(blocks.at(j).at(i));
    }
  }
  if (result.size() != static_cast<unsigned int>(getRawDataModulesCount(m_version) / 8))
    throw "Assertion error";

  return result;
}

void QRCode::drawCodewords(const ui8vector &data) 
{
  if (data.size() != static_cast<unsigned int>(getRawDataModulesCount(m_version) / 8))
    throw "Invalid argument";

  size_t i = 0;  // Bit index into the data

  // Do the funny zigzag scan
  for (int right = m_size - 1; right >= 1; right -= 2) 
  { 
    // Index of right column in each column pair
    if (right == 6)
      right = 5;

    for (int vert = 0; vert < m_size; vert++) 
    {
      // Vertical counter
      for (int j = 0; j < 2; j++)
      {
        int x = right - j;  // Actual x coordinate
        bool upwards = ((right & 2) == 0) ^ (x < 6);
        int y = upwards ? m_size - 1 - vert : vert;  // Actual y coordinate
        if (!m_isFunction[y][x] && i < data.size() * 8) 
        {
          m_modules[y][x] = ((data[i >> 3] >> (7 - (i & 7))) & 1) != 0;
          i++;
        }
        // If there are any remainder bits (0 to 7), they are already
        // set to 0/false/white when the grid of modules was initialized
      }
    }
  }

  if (static_cast<unsigned int>(i) != data.size() * 8)
    throw "Assertion error";
}

int QRCode::handleConstructorMasking(int mask)
{
  if (mask == -1) 
  {
    // Automatically choose best mask
    int32_t minPenalty = INT32_MAX;
    for (int i = 0; i < 8; i++)
    {
      drawFormatBits(i);
      applyMask(i);

      int penalty = getPenaltyScore();
      if (penalty < minPenalty) 
      {
        mask = i;
        minPenalty = penalty;
      }
      applyMask(i);  // Undoes the mask due to XOR
    }
  }

  if (mask < 0 || mask > 7)
    throw "Assertion error";

  drawFormatBits(mask);  // Overwrite old format bits
  applyMask(mask);  // Apply the final choice of mask

  return mask;  // The caller shall assign this value to the final-declared field
}

void QRCode::setFunctionModule(int x, int y, bool isBlack) 
{
  m_modules[y][x]     = isBlack;
  m_isFunction[y][x]  = true;
}

void QRCode::drawFinderPattern(int x, int y) 
{
  for (int i = -4; i <= 4; i++) 
  {
    for (int j = -4; j <= 4; j++) 
    {
      int dist = std::max(std::abs(i), std::abs(j));  // Chebyshev/infinity norm
      int xx = x + j, yy = y + i;
      if ((0 <= xx) && (xx < m_size) && (0 <= yy) && (yy < m_size))
        setFunctionModule(xx, yy, (dist != 2 && dist != 4));
    }
  }
}

void QRCode::drawAlignmentPattern(int x, int y) 
{
  for (int i = -2; i <= 2; i++) 
  {
    for (int j = -2; j <= 2; j++)
      setFunctionModule((x + j), (y + i), std::max(std::abs(i), std::abs(j)) != 1);
  }
}

void QRCode::drawFormatBits(int mask) 
{
  // Calculate error correction code and pack bits
  int eclFormatBits = getECLFormatBits();
  int data = eclFormatBits << 3 | mask;  // errCorrLvl is uint2, mask is uint3
  int rem = data;
  for (int i = 0; i < 10; i++)
    rem = (rem << 1) ^ ((rem >> 9) * 0x537);

  data = data << 10 | rem;
  data ^= 0x5412;  // uint15
  if (data >> 15 != 0)
    throw "Assertion error";

  // Draw first copy
  for (int i = 0; i <= 5; i++)
    setFunctionModule(8, i, ((data >> i) & 1) != 0);

  setFunctionModule(8, 7, ((data >> 6) & 1) != 0);
  setFunctionModule(8, 8, ((data >> 7) & 1) != 0);
  setFunctionModule(7, 8, ((data >> 8) & 1) != 0);

  for (int i = 9; i < 15; i++)
    setFunctionModule(14 - i, 8, ((data >> i) & 1) != 0);

  // Draw second copy
  for (int i = 0; i <= 7; i++)
    setFunctionModule(m_size - 1 - i, 8, ((data >> i) & 1) != 0);

  for (int i = 8; i < 15; i++)
    setFunctionModule(8, m_size - 15 + i, ((data >> i) & 1) != 0);

  setFunctionModule(8, m_size - 8, true);
}

void QRCode::drawVersion()
{
  if (m_version < 7)
    return;

  // Calculate error correction code and pack bits
  int rem = m_version;  // version is uint6, in the range [7, 40]
  for (int i = 0; i < 12; i++)
    rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);

  int data = m_version << 12 | rem;  // uint18
  if (data >> 18 != 0)
    throw "Assertion error";

  // Draw two copies
  for (int i = 0; i < 18; i++) 
  {
    bool bit = ((data >> i) & 1) != 0;
    int a = m_size - 11 + i % 3, b = i / 3;

    setFunctionModule(a, b, bit);
    setFunctionModule(b, a, bit);
  }
}

void QRCode::applyMask(int mask) 
{
  if (mask < 0 || mask > 7)
    throw "Mask value out of range";

  for (int y = 0; y < m_size; y++) {
    for (int x = 0; x < m_size; x++) 
    {
      bool invert;
      switch (mask) 
      {
        case 0:  invert = (x + y) % 2 == 0;                    break;
        case 1:  invert = y % 2 == 0;                          break;
        case 2:  invert = x % 3 == 0;                          break;
        case 3:  invert = (x + y) % 3 == 0;                    break;
        case 4:  invert = (x / 3 + y / 2) % 2 == 0;            break;
        case 5:  invert = x * y % 2 + x * y % 3 == 0;          break;
        case 6:  invert = (x * y % 2 + x * y % 3) % 2 == 0;    break;
        case 7:  invert = ((x + y) % 2 + x * y % 3) % 2 == 0;  break;
        default:  throw "Assertion error";
      }

      m_modules[y][x] = m_modules[y][x] ^ (invert & !m_isFunction[y][x]);
    }
  }
}

int QRCode::getPenaltyScore() const
{
  int result = 0;
	
  // Adjacent modules in row having same color
  for (int y = 0; y < m_size; y++) 
  {
    bool colorX = m_modules[y][0];
    for (int x = 1, runX = 1; x < m_size; x++) 
    {
      if (m_modules[y][x] != colorX) 
      {
        colorX = m_modules[y][x];
        runX = 1;
      }
      else
      {
        runX++;
        if (runX == 5)
          result += PENALTY_N1;
        else if (runX > 5)
          result++;
      }
    }
  }

  // Adjacent modules in column having same color
  for (int x = 0; x < m_size; x++) 
  {
    bool colorY = m_modules[0][x];
    for (int y = 1, runY = 1; y < m_size; y++) 
    {
      if (m_modules[y][x] != colorY) 
      {
        colorY = m_modules[y][x];
        runY = 1;
      }
      else
      {
        runY++;
        if (runY == 5)
          result += PENALTY_N1;
        else if (runY > 5)
          result++;
      }
    }
  }

  // 2*2 blocks of modules having same color
  for (int y = 0; y < m_size - 1; y++) 
  {
    for (int x = 0; x < m_size - 1; x++) 
    {
      bool  color = m_modules[y][x];
      if (
          (color == m_modules[y][x + 1]) &&
          (color == m_modules[y + 1][x]) &&
          (color == m_modules[y + 1][x + 1])
         )
      result += PENALTY_N2;
    }
  }
	
  // Finder-like pattern in rows
  for (int y = 0; y < m_size; y++) 
  {
    for (int x = 0, bits = 0; x < m_size; x++) 
    {
      bits = ((bits << 1) & 0x7FF) | (m_modules[y][x] ? 1 : 0);
      if (x >= 10 && (bits == 0x05D || bits == 0x5D0))  // Needs 11 bits accumulated
        result += PENALTY_N3;
    }
  }

  // Finder-like pattern in columns
  for (int x = 0; x < m_size; x++)
  {
    for (int y = 0, bits = 0; y < m_size; y++) 
    {
      bits = ((bits << 1) & 0x7FF) | (m_modules[y][x] ? 1 : 0);
      if (y >= 10 && (bits == 0x05D || bits == 0x5D0))  // Needs 11 bits accumulated
        result += PENALTY_N3;
    }
  }

  // Balance of black and white modules
  int black = 0;
  for (int y = 0; y < m_size; y++) 
  {
    for (int x = 0; x < m_size; x++) 
    {
      if (m_modules[y][x])
      black++;
    }
  }

  int total = m_size * m_size;
  // Find smallest k such that (45-5k)% <= dark/total <= (55+5k)%
  for (int k = 0; black*20 < (9-k)*total || black*20 > (11+k)*total; k++)
    result += PENALTY_N4;

  return result;
}
