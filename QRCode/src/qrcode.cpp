#include "qrcode.h"

using namespace QR;

QRCode::QRCode()
  :m_datamode(DM_NUL),
   m_ecl(),
   m_version(),
   m_width(0),
   p_data(NULL)
{
}

QRCode::QRCode(int version, const ECL &ecl)
  :m_datamode(DM_NUL),
   m_ecl(ecl),
   m_version(version),
   m_width(0),
   p_data(NULL)
{
}

QRCode::QRCode(const QRCode &other)
:m_datamode(other.m_datamode),
 m_ecl(other.m_ecl),
 m_version(other.m_version),
 m_width(other.m_width),
 p_data(NULL)
{
  if((other.p_data != NULL) && (other.m_width > 0))
  {
    p_data = (unsigned char*)malloc(other.m_width);

    memcpy(p_data, other.p_data, other.m_width);
  }
}

QRCode& QRCode::operator=(const QRCode &other)
{
  if(this != &other)
  {
    m_datamode = other.m_datamode;
    m_ecl = other.m_ecl;
    m_version = other.m_version;
    m_width = other.m_width;

    if((other.p_data != NULL) && (other.m_width > 0))
    {
      if(p_data == NULL)
        p_data = (unsigned char*)malloc(other.m_width);

      memcpy(p_data, other.p_data, other.m_width);
    }
  }

  return(*this);
}

QRCode::~QRCode()
{
  if(p_data != NULL)
    free(p_data);
}

void QRCode::setDataMode(const DATA_MODE &dm)
{
  if((dm >= DM_NUL) && (dm <= DM_FNC1SECOND))
    m_datamode = dm;
}

void QRCode::setErrorCorrectionLevel(const ECL &ecl)
{
  if((ecl >= ECL_L) && (ecl <= ECL_H))
    m_ecl = ecl;
}

void QRCode::setVersion(int version)
{
  if((version > 0) && (version <= 40))
    m_version = version;
}

DATA_MODE QRCode::getDataMode() const
{
  return(m_datamode);
}

ECL QRCode::getErrorCorrectionLevel() const
{
  return(m_ecl);
}

int QRCode::getVersion()const
{
  return(m_version);
}

int QRCode::getWidth()const
{
  return(m_width);
}

unsigned char* QRCode::getData() const
{
  unsigned char *data = NULL;

  if(m_width > 0)
  {
    data = (unsigned char *)malloc(m_width);
    memcpy(data, p_data, m_width);
  }

  return(data);
}

void QRCode::identifyDataMode(const string &input, const DATA_MODE &hint)
{
  if(input.empty())
    m_datamode = DM_NUL;
  if(isDigit(input))
    m_datamode = DM_NUM;
  else if(isAlphaNumeric(input))
    m_datamode = DM_AN;
  else if(hint == DM_KANJI)
  {
    if(isKanji(input))
      m_datamode = DM_KANJI;
    else
      m_datamode = DM_8;
  }
  else
    m_datamode = DM_8;
}

void QRCode::encode(const string &input, const DATA_MODE &hint)
{
  if( !input.empty() )
  {
    if((hint == DM_8) || (hint == DM_KANJI))
    {
      /// 1. Data Analysis
      ///   - get data mode according to the input string.
      identifyDataMode(input, hint);

      /// 2. Data Encoding
      /// 3. Error Correction Coding
      /// 4. Structure Final Message
      /// 5. Module Placement in Matrix
      /// 6. Data Masking
      /// 7. Format and Version Information
    }
  }
}

string QRCode::decode(unsigned char *data)
{
  string output = "";

  return(output);
}

void QRCode::writeToBMP(const string &filename)
{
}

void QRCode::writeToPNG(const string &filename)
{
}

void QRCode::writeToJPEG(const string &filename)
{
}
