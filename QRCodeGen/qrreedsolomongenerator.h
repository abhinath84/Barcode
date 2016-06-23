/* 
* Computes the Reed-Solomon error correction codewords for a sequence of data codewords
* at a given degree. Objects are immutable, and the state only depends on the degree.
* This class exists because the divisor polynomial does not need to be recalculated for every input.
*/

#ifndef QRREEDSOLOMONGENERATOR_H
#define QRREEDSOLOMONGENERATOR_H

#include <algorithm>
#include <iterator>
#include <vector>

#include "qrutility.h"

namespace QR
{
  class QRReedSolomonGenerator
  {
    public:
      QRReedSolomonGenerator();
      /* 
      * Creates a Reed-Solomon ECC generator for the given degree. This could be implemented
      * as a lookup table over all possible parameter values, instead of as an algorithm.
      */
      QRReedSolomonGenerator(int degree);
      QRReedSolomonGenerator(const QRReedSolomonGenerator &other);
      ~QRReedSolomonGenerator();

      QRReedSolomonGenerator& operator=(const QRReedSolomonGenerator &other);

      /* 
      * Computes and returns the Reed-Solomon error correction codewords for the given sequence of data codewords.
      * The returned object is always a new byte array. This method does not alter this object's state (because it is immutable).
      */
      std::vector<uint8_t> getRemainder(const std::vector<uint8_t> &data) const;

    private:
      // Returns the product of the two given field elements modulo GF(2^8/0x11D). The arguments and result
      // are unsigned 8-bit integers. This could be implemented as a lookup table of 256*256 entries of uint8.
      static uint8_t multiply(uint8_t x, uint8_t y);

    private:
      // Coefficients of the divisor polynomial, stored from highest to lowest power, excluding the leading term which
      // is always 1. For example the polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8, 93}.
      std::vector<uint8_t> m_coefficients;
  };
}

#endif    // QRREEDSOLOMONGENERATOR_H
