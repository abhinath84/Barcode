#include "qrdataencode.h"

using namespace QR;

//!< QRDataEncode

QRDataEncode::QRDataEncode()
  : m_input(),
  m_ecl(ECL_L),
  m_version(0),
  m_width(),
  m_bits()
{
}

QRDataEncode::QRDataEncode(const string &input, const ECL &ecl)
  : m_input(input),
  m_ecl(ecl),
  m_version(0),
  m_width(input.size()),
  m_bits()
{
}

QRDataEncode::QRDataEncode(const QRDataEncode &other)
  : m_input(other.m_input),
  m_ecl(other.m_ecl),
  m_version(other.m_version),
  m_width(other.m_width),
  m_bits(other.m_bits)
{
}

QRDataEncode& QRDataEncode::operator=(const QRDataEncode &other)
{
  if(this != &other)
  {
    setInput(other.m_input);
    setErrorCorrectionLevel(other.m_ecl);
    setVersion(other.m_version);
    setBits(other.m_bits);
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

QRBitBuffer QRDataEncode::getBits() const
{
  return m_bits;
}

void QRDataEncode::setBits(const QRBitBuffer &bits)
{
  m_bits = bits;
}

void QRDataEncode::appendBit(uint8_t val)
{
  //m_bits.append(val);
}

void QRDataEncode::appendBit(uint32_t val, int len)
{
  //m_bits.append(val, len);
}

bool QRDataEncode::calculateVersion()
{
  return false;
}

bool QRDataEncode::calculateVersion(const int CCT[][4])
{
  bool status = false;

  for(int i = m_ecl; i <= ECL_H; ++i)
  {
    for(int j = 0; j < 40; ++j)
    {
      if(m_width <= CCT[j][i])
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
  
  return(status);
}

bool QRDataEncode::calculateErrorCorrectionLevel()
{
  return false;
}

bool QRDataEncode::calculateErrorCorrectionLevel(const int CCT[][4])
{
  bool status = false;
  if(m_version > 0 && m_version <= 40)
  {
    for(int i = (m_ecl + 1); i <= ECL_H; ++i)
    {
      if(m_input.length() > CCT[m_version][i])
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

bool QRDataEncode::encode()
{
  return false;
}


//!< QRNumericEncode
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
    setBits(other.getBits());
  }

  return(*this);
}

bool QRNumericEncode::calculateVersion()
{
  return(QRDataEncode::calculateVersion(QRNumeric_CCT));
}

bool QRNumericEncode::calculateErrorCorrectionLevel()
{
  return(QRDataEncode::calculateErrorCorrectionLevel(QRNumeric_CCT));
}

bool QRNumericEncode::addModeIndicator()
{
  uint8_t *modeIndicator = NULL;

  modeIndicator = (uint8_t*)malloc(4 * sizeof(uint8_t));
  memcpy(modeIndicator, "0001", 4 * sizeof(uint8_t));

  return(true);
}

bool QRNumericEncode::addCharCountIndicator()
{
  bool status = false;
  int len = 0;
  uint8_t *charCountIndicator = NULL;

  /// get length
  if(m_version < 10)
    len = QRDataEncode_CCLT[0][0];
  else if(m_version < 27)
    len = QRDataEncode_CCLT[1][0];
  else if(m_version <= 40)
    len = QRDataEncode_CCLT[2][0];

  if(len > 0)
  {
    charCountIndicator = (uint8_t*)malloc(4 * sizeof(uint8_t));

    /// convert input length into bit and pad extra bit by '0'
    /// uint8_t *cci;
    /// cci = convertToByte(m_input.lenght());
    /// 
    status = true;
  }

  return(status);
}

bool QRNumericEncode::encode()
{
  return(false);
}
