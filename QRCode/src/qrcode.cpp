#include "qrcode.h"

using namespace QR;

QRCode::QRCode()
  :m_input(),
   m_datamode(DM_NUL),
   m_ecl(ECL_L),
   m_version(1),
   m_width(0),
   m_encoded()
{
}

QRCode::QRCode(int version, const ECL &ecl)
  :m_input(),
   m_datamode(DM_NUL),
   m_ecl(ecl),
   m_version(version),
   m_width(0),
   m_encoded()
{
}

QRCode::QRCode(const QRCode &other)
  :m_input(other.m_input),
   m_datamode(other.m_datamode),
   m_ecl(other.m_ecl),
   m_version(other.m_version),
   m_width(other.m_width),
   m_encoded(other.m_encoded)
{
}

QRCode& QRCode::operator=(const QRCode &other)
{
  if(this != &other)
  {
    setInput(other.m_input);
    m_datamode = other.m_datamode;
    m_ecl = other.m_ecl;
    m_version = other.m_version;
    m_width = other.m_width;

    setEncodedData(other.m_encoded);
  }

  return(*this);
}

QRCode::~QRCode()
{
}

void QRCode::setInput(const string &input)
{
  if(m_input.compare(input) != 0)
  {
    m_input.clear();
    m_input.replace(m_input.begin(), m_input.end(), input);
  }
}

void QRCode::setEncodedData(const QRBitBuffer &encoded)
{
  if(encoded.size() > 0)
  {
    m_encoded = encoded;
  }
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

vector<unsigned char> QRCode::getData() const
{
  return(m_encoded.bytes());
}

/*QRDataEncode* QRCode::getDataEncode()
{
    /// Numeric Data Mode
    if(m_datamode == DM_NUM)
      return(new QRNumericEncode());
    else if(m_datamode == DM_AN)
      return(new QRAlphanumericEncode());
    //else if(m_datamode == DM_8)
    //  return(new QRByteEncode());
    //else if(m_datamode == DM_KANJI)
    //  return(new QRKanjiEncode());
    else
      return(NULL);
}*/

void QRCode::identifyDataMode(const DATA_MODE &hint)
{
  if(m_input.empty())
    m_datamode = DM_NUL;
  if(isDigit(m_input))
    m_datamode = DM_NUM;
  else if(isAlphaNumeric(m_input))
    m_datamode = DM_AN;
  else if(hint == DM_KANJI)
  {
    if(isKanji(m_input))
      m_datamode = DM_KANJI;
    else
      m_datamode = DM_8;
  }
  else
    m_datamode = DM_8;
}

bool QRCode::encodeData()
{
  bool status = false;

  if(m_datamode > DM_NUL)
  {
    QRDataEncode qrDataEncode;

    /// necessary data
    qrDataEncode.setInput(m_input);
    qrDataEncode.setMode(m_datamode + 1);
    qrDataEncode.setErrorCorrectionLevel(m_ecl);

    /// encode data
    status = qrDataEncode.encode();
    if(status)
    {
      /// set version, ECL, encoded data.
      setEncodedData(qrDataEncode.getEncodedData());
      m_version = qrDataEncode.getVersion();
      m_ecl = qrDataEncode.getErrorCorrectionLevel();
    }
  }

  return(status);
}

bool QRCode::appendErrorCorrection()
{
  bool status = false;

  return(status);
}

void QRCode::encode(const string &input, const DATA_MODE &hint)
{
  if( !input.empty() )
  {
    if((hint == DM_8) || (hint == DM_KANJI))
    {
      bool status = false;

      /// set the input.
      setInput(input);

      /// 1. Data Analysis
      ///   - get data mode according to the input string.
      identifyDataMode(hint);

      /// 2. Data Encoding
      status = encodeData();

      /// 3. Error Correction Coding
      if(status) status = appendErrorCorrection();

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
