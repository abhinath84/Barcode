
#ifndef QRBITBUFFER_H
#define QRBITBUFFER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "qrutility.h"

using namespace std;

namespace QR
{
  class QRBitBuffer
  {
    public:
      QRBitBuffer(void);
      QRBitBuffer(const QRBitBuffer &other);
      ~QRBitBuffer(void);

      QRBitBuffer& operator=(const QRBitBuffer &other);

      // Methods
      void push_back(const uint8_t &val);
      void push_back(const vector<uint8_t> &val);
      void push_back(const vector<uint8_t> &val, int size);

      void push_front(const uint8_t &val);
      void push_front(const vector<uint8_t> &val);

      int size() const;
      vector<unsigned char> bytes() const;
      void clear();

    private:
      vector<uint8_t> m_bits;
  };
}

#endif    // QRBITBUFFER_H
