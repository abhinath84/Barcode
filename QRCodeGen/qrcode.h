#ifndef QRCODE_H
#define QRCODE_H

#include <algorithm>
#include <vector>

#include "qrsegment.h"

namespace QR
{
  class QRCode
  {
    public:
      QRCode();
      ~QRCode();

      int getTotalBits(const std::vector<QRSegment> &segs, int version);
  };
}

#endif    // QRCODE_H
