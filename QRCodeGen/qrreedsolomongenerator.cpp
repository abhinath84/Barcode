#include "qrreedsolomongenerator.h"

using namespace QR;

/// Default Constructor
QRReedSolomonGenerator::QRReedSolomonGenerator(void)
  :m_coefficients(),
  m_degree(0)
{
}

/// Parametric Constructor
QRReedSolomonGenerator::QRReedSolomonGenerator(int degree)
  :m_coefficients(),
  m_degree(degree)
{
  computePolynomial();
}

/// Copy COnstructor
QRReedSolomonGenerator::QRReedSolomonGenerator(const QRReedSolomonGenerator &other)
  :m_coefficients(other.m_coefficients),
  m_degree(other.m_degree)
{
}

/// Destructor
QRReedSolomonGenerator::~QRReedSolomonGenerator(void)
{
}

/// Assignment Operator
QRReedSolomonGenerator& QRReedSolomonGenerator::operator=(const QRReedSolomonGenerator &other)
{
  if(this != &other)
  {
    m_degree = other.m_degree;

    m_coefficients.clear();
    std::copy(other.m_coefficients.begin(), other.m_coefficients.end(),
                back_inserter(m_coefficients));
  }

  return(*this);
}

/** @brief compute and get Reed-Solomon error correction codewords.
*
*  Computes and returns the Reed-Solomon error correction codewords for the given sequence of data codewords.
*  The returned object is always a new byte array. This method does not alter this object's state (because it is immutable).
*
*  @param[in]   data the codewords for which compute RS error correction codewords.
*
*  @return ui8vector the RS error correction codewords.
*/
ui8vector QRReedSolomonGenerator::getErrorCorrection(const ui8vector &data) const 
{
  // Compute the remainder by performing polynomial division
  ui8vector result(m_coefficients.size());

  for (size_t i = 0; i < data.size(); i++) 
  {
    uint8_t factor = data[i] ^ result[0];
    result.erase(result.begin());
    result.push_back(0);
    for (size_t j = 0; j < result.size(); j++)
    {
      result[j] ^= multiply(m_coefficients[j], factor);
    }
  }
  return result;
}

/** @brief set degree for QRReedSolomonGenerator class.
*
*  Set degree for QRReedSolomonGenerator class and calculate coefficients of the divisor polynomial
*  and store them in m_coefficients variable for further use.
*  degree value must be within 1 to 255.
*
*  @param[in]   deg the degree to set.
*
*  @return nothing.
*/
void QRReedSolomonGenerator::setDegree(int deg)
{
  /// check for valid input
  if ((m_degree >= 1) && (m_degree <= 255))
  {
    m_degree = deg;
    computePolynomial();
  }
}

void QRReedSolomonGenerator::computePolynomial()
{
  if (m_degree < 1 || m_degree > 255)
    throw "Degree out of range";

  // Start with the monomial x^0
  m_coefficients.clear();
  m_coefficients.resize(m_degree);
  m_coefficients.at(m_degree - 1) = 1;

  // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{m_degree-1}),
  // drop the highest term, and store the rest of the coefficients in order of descending powers.
  // Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
  int root = 1;
  for (int i = 0; i < m_degree; i++) 
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

/** @brief calculate the product of 2 inputs (modulo GF(2^8/0x11D)).
*
*  Returns the product of the two given field elements modulo GF(2^8/0x11D). The arguments and result
*  are unsigned 8-bit integers. This could be implemented as a lookup table of 256*256 entries of uint8.
*
*  @param[in]   deg the degree to set.
*
*  @return nothing.
*/
uint8_t QRReedSolomonGenerator::multiply(uint8_t x, uint8_t y) const
{
  // Russian peasant multiplication
  int z = 0;
  for (int i = 7; i >= 0; i--)
  {
    z = (z << 1) ^ ((z >> 7) * 0x11D);
    z ^= ((y >> i) & 1) * x;
  }

  if (z >> 8 != 0)
    throw "Assertion error";

  return static_cast<uint8_t>(z);
}
