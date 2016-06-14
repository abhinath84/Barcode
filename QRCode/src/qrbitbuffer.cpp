#include "qrbitbuffer.h"

using namespace QR;

QRBitBuffer::QRBitBuffer()
  :m_bits()
{
}

QRBitBuffer::QRBitBuffer(const QRBitBuffer &other)
  :m_bits(other.m_bits)
{
}

QRBitBuffer::~QRBitBuffer()
{
}

QRBitBuffer& QRBitBuffer::operator=(const QRBitBuffer &other)
{
  if(this != &other)
  {
    m_bits.clear();
    copy(other.m_bits.begin(), other.m_bits.end(), back_inserter(m_bits));
  }

  return(*this);
}

void QRBitBuffer::push_back(const uint8_t &val)
{
  m_bits.push_back(val);
}

void QRBitBuffer::push_back(const vector<uint8_t> &val)
{
  int size = val.size();

  for(int i = 0; i < size; ++i)
    m_bits.push_back(val[i]);
}

void QRBitBuffer::push_back(const vector<uint8_t> &val, int size)
{
  int s_val = val.size();

  if((size > 0) && (s_val <= size))
  {
    int diff = size - s_val;
    for(int i = 0; i < size; ++i)
    {
      if(i < diff)
        m_bits.push_back('0');
      else
        m_bits.push_back(val[i - diff]);
    }
  }
}

void QRBitBuffer::push_front(const uint8_t &val)
{
  vector<unsigned char> tmp;
  int size = m_bits.size();
  
  tmp.push_back(val);
  for(int i = 0; i < size; ++i)
    tmp.push_back(m_bits[i]);

  m_bits.clear();
  copy(tmp.begin(), tmp.end(), back_inserter(m_bits));
}

void QRBitBuffer::push_front(const vector<uint8_t> &val)
{
  vector<unsigned char> tmp;
  int size = 0;
  
  size = val.size();
  for(int i = 0; i < size; ++i)
    tmp.push_back(val[i]);

  size = m_bits.size();
  for(int i = 0; i < size; ++i)
    tmp.push_back(m_bits[i]);

  m_bits.clear();
  copy(tmp.begin(), tmp.end(), back_inserter(m_bits));
}

int QRBitBuffer::size() const
{
  return((int)m_bits.size());
}

vector<unsigned char> QRBitBuffer::bytes() const
{
  return(m_bits);
}

void QRBitBuffer::clear()
{
  m_bits.clear();
}