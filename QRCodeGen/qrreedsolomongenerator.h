/**
*  @file    qrreedsolomongenerator.h
*  @brief   class to Compute Reed-Solomon error correction codewards for QR Code.
*
*  Computes the Reed-Solomon error correction codewords for a sequence of data codewords
*  at a given degree. Objects are immutable, and the state only depends on the degree.
*  This class exists because the divisor polynomial does not need to be recalculated for every input.
*
*  @author  Abhishek Nath
*  @date    20-June-2016
*
*/


#ifndef QRREEDSOLOMONGENERATOR_H
#define QRREEDSOLOMONGENERATOR_H

#include <algorithm>
#include <iterator>
#include <vector>

#include "qrutility.h"

namespace QR
{
  //!  @class  QRReedSolomonGenerator
  /*!
    Computes the Reed-Solomon error correction codewords for a sequence of data codewords
   at a given degree. Objects are immutable, and the state only depends on the degree.
   This class exists because the divisor polynomial does not need to be recalculated for every input.
  */
  class QRReedSolomonGenerator
  {
    public:
      /// Default COnstructor
      QRReedSolomonGenerator();

      /// Parametric COnstructor
      /// Creates a Reed-Solomon ECC generator for the given degree. This could be implemented
      /// as a lookup table over all possible parameter values, instead of as an algorithm.
      QRReedSolomonGenerator(int degree);

      /// Copy Constructor
      QRReedSolomonGenerator(const QRReedSolomonGenerator &other);

      /// Destructor
      ~QRReedSolomonGenerator();

      /// Assignment Operator
      QRReedSolomonGenerator& operator=(const QRReedSolomonGenerator &other);

      /** @brief compute and get Reed-Solomon error correction codewords.
      *
      *  Computes and returns the Reed-Solomon error correction codewords for the given sequence of data codewords.
      *  The returned object is always a new byte array. This method does not alter this object's state (because it is immutable).
      *
      *  @param[in]   data the codewords for which compute RS error correction codewords.
      *
      *  @return ui8vector the RS error correction codewords.
      */
      ui8vector getErrorCorrection(const ui8vector &data) const;

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
      void setDegree(int deg);

    private:
      void computePolynomial();

    /** @brief calculate the product of 2 inputs (modulo GF(2^8/0x11D)).
    *
    *  Returns the product of the two given field elements modulo GF(2^8/0x11D). The arguments and result
    *  are unsigned 8-bit integers. This could be implemented as a lookup table of 256*256 entries of uint8.
    *
    *  @param[in]   deg the degree to set.
    *
    *  @return nothing.
    */
      uint8_t multiply(uint8_t x, uint8_t y) const;

    private:
      int       m_degree;         ///< Define degree for the Reed-Solomon error correction.
      ui8vector m_coefficients;   ///< Define coefficients of the divisor polynomial, stored from highest to lowest power, 
                                  ///< excluding the leading term which is always 1. 
                                  ///< For example the polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8, 93}.
  };
}

#endif    // QRREEDSOLOMONGENERATOR_H
