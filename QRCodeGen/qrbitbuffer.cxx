#include "qrbitbuffer.h"

using namespace QR;

QRBitBuffer::QRBitBuffer()
  : m_bits(),
  m_len(0)
{
}

QRBitBuffer::QRBitBuffer(const QRBitBuffer &other)
  :m_bits(other.m_bits),
  m_len(other.m_len)
{
}

QRBitBuffer& QRBitBuffer::operator=(const QRBitBuffer &other)
{
  if(this != &other)
  {
    // copy element by element
    std::copy(other.m_bits.begin(), other.m_bits.end(),
                back_inserter(m_bits));
  }

  return(*this);
}

QRBitBuffer::~QRBitBuffer()
{
}

int QRBitBuffer::getBitLength() const
{
  return(m_len);
}

ui8vector QRBitBuffer::getBytes() const 
{
  return m_bits;
}

void QRBitBuffer::appendBits(uint32_t val, int len) 
{
  if (len < 0 || len > 32 || (len < 32 && (val >> len) != 0))
    throw "Value out of range";

  size_t newBitLen = m_len + len;
  while (m_bits.size() * 8 < newBitLen)
    m_bits.push_back(0);

  for (int i = len - 1; i >= 0; i--, m_len++)  // Append bit by bit
  {
    unsigned pos = m_len >> 3;              // to manage 8-bit format for each entry
    unsigned val_left = val >> i;           // ??
    unsigned and_one = val_left & 1;        // ??
    unsigned bit_and = m_len & 7;           // manage value b/w 0 - 7, whatever is the value of m_len
    unsigned after_bit_and = 7 - bit_and;   // ??
    unsigned char right_shift = and_one << after_bit_and;   // ??
    unsigned char prev_val = m_bits.at(pos);                // ??
    unsigned char final_val = prev_val | right_shift;       // ??

    m_bits.at(pos) |= ((val >> i) & 1) << (7 - (m_len & 7));
  }
}

void QRBitBuffer::appendData(const QRSegment &seg) 
{
  size_t newBitLen = m_len + seg.getBitSize();
  while (m_bits.size() * 8 < newBitLen)
    m_bits.push_back(0);

  for (int i = 0; i < seg.getBitSize(); i++, m_len++) 
  {
    // Append bit by bit
    int bit = (seg.at(i >> 3) >> (7 - (i & 7))) & 1;
    m_bits.at(m_len >> 3) |= bit << (7 - (m_len & 7));
  }
}
