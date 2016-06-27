#include "qrreedsolomongenerator.h"

using namespace QR;

QRReedSolomonGenerator::QRReedSolomonGenerator(void)
  :m_coefficients()
{
}

QRReedSolomonGenerator::QRReedSolomonGenerator(int degree)
  :m_coefficients()
{
  if (degree < 1 || degree > 255)
    throw "Degree out of range";

  // Start with the monomial x^0
  m_coefficients.resize(degree);
  m_coefficients.at(degree - 1) = 1;

  // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{degree-1}),
  // drop the highest term, and store the rest of the coefficients in order of descending powers.
  // Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
  int root = 1;
  for (int i = 0; i < degree; i++) 
  {
    // Multiply the current product by (x - r^i)
    for (size_t j = 0; j < m_coefficients.size(); j++) 
    {
      m_coefficients[j] = multiply(m_coefficients[j], static_cast<uint8_t>(root));
      if (j + 1 < m_coefficients.size())
        m_coefficients[j] ^= m_coefficients[j + 1];
    }

    root = (root << 1) ^ ((root >> 7) * 0x11D);  // Multiply by 0x02 mod GF(2^8/0x11D)
  }
}

QRReedSolomonGenerator::QRReedSolomonGenerator(const QRReedSolomonGenerator &other)
  :m_coefficients(other.m_coefficients)
{
}

QRReedSolomonGenerator::~QRReedSolomonGenerator(void)
{
}

QRReedSolomonGenerator& QRReedSolomonGenerator::operator=(const QRReedSolomonGenerator &other)
{
  if(this != &other)
  {
    m_coefficients.clear();
    std::copy(other.m_coefficients.begin(), other.m_coefficients.end(),
                back_inserter(m_coefficients));
  }

  return(*this);
}

ui8vector QRReedSolomonGenerator::getRemainder(const ui8vector &data) const 
{
  // Compute the remainder by performing polynomial division
  ui8vector result(m_coefficients.size());
  for (size_t i = 0; i < data.size(); i++) 
  {
    uint8_t factor = data[i] ^ result.at(0);
    result.erase(result.begin());
    result.push_back(0);
    for (size_t j = 0; j < result.size(); j++)
      result.at(j) ^= multiply(m_coefficients[j], factor);
  }
  return result;
}

uint8_t QRReedSolomonGenerator::multiply(uint8_t x, uint8_t y) 
{
  // Russian peasant multiplication
  int z = 0;
  for (int i = 7; i >= 0; i--) {
    z = (z << 1) ^ ((z >> 7) * 0x11D);
    z ^= ((y >> i) & 1) * x;
  }

  if (z >> 8 != 0)
    throw "Assertion error";

  return static_cast<uint8_t>(z);
}
