#include "qrcode.h"

using namespace QR;

QRCode::QRCode(void)
{
}


QRCode::~QRCode(void)
{
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
