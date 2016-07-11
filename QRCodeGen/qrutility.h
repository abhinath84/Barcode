#ifndef QRUTILITY_H
#define QRUTILITY_H

#include <iostream>
#include <vector>

namespace QR
{
  #define INT32_MAX         2147483647i32

  /// define system datatype
  typedef unsigned char     uint8_t;
  typedef unsigned short    uint16_t;
  typedef unsigned int      uint32_t;
  typedef unsigned long int uint64_t;
  typedef signed char       int8_t;
  typedef short             int16_t;
  typedef int               int32_t;

  typedef std::vector<uint8_t>  ui8vector;

  //!  @enum  DATA_MODE
  /*!
    Data modes for encoding text. Each mode encodes the text as a string of bits (1s and 0s),
    but each mode uses a different method for converting the text into bits.
    Each method is optimized to generate the shortest possible string of bits for that data type.
    An unsigned 4-bit integer value (range 0 to 15) representing the mode indicator bits for this mode object.
  */
  typedef enum DATA_MODE
  {
    DM_NUL    = -1,   ///< Terminator (NUL character). Internal use only
    DM_NUM    = 1,    ///< Numeric mode
    DM_AN     = 2,    ///< Alphabet-numeric mode
    DM_8      = 4,    ///< 8-bit data mode
    DM_KANJI  = 8,    ///< Kanji (shift-jis) mode
    /*DM_STRUCTURE,     ///< Internal use only
    DM_ECI,           ///< ECI mode
    DM_FNC1FIRST,     ///< FNC1, first position
    DM_FNC1SECOND,    ///< FNC1, second position*/
  } DM;

  //!  @enum  ERROR_CORRECTION_LEVEL
  /*!
    Before encoding the data, select an error correction level. 
    QR codes use Reed-Solomon error correction. This process creates error correction 
    codewords (bytes) based on the encoded data. A QR code reader can use these error 
    correction bytes to determine if it did not read the data correctly, 
    and the error correction codewords can be used to correct those errors. 
    There are four levels of error correction: L, M, Q, H.
  */
  typedef enum ERROR_CORRECTION_LEVEL
  {
    ECL_L = 0,    ///< Recovers 7% of data
    ECL_M,        ///< Recovers 15% of data
    ECL_Q,        ///< Recovers 25% of data
    ECL_H         ///< Recovers 30% of data
  } ECL;


  /** @brief check whether input character is a digit or not.
  *
  *  @param[in]  c the input character to check.
  *
  *  @return bool  true   -> if its a digit.
  *                false  -> if not.
  */
  bool isDigit(unsigned char c);

  /** @brief check whether all the characters in the input string are digits or not.
  *
  *  @param[in]  input the input string to check.
  *
  *  @return bool  true   -> if its a set of digits.
  *                false  -> if not.
  */
  bool isDigit(const std::string &input);

  /** @brief get alphanumeric number of the given input character.
  *
  *  @param[in]  c the character to get alphanumeric number.
  *
  *  @return int alphanumeric number.
  */
  int getAlphanumeric(unsigned char c);

  /** @brief check whether input character is an alphanumeric symbol or not.
  *
  *  @param[in]  c the character to check.
  *
  *  @return bool  true   -> if its an alphanumeric symbol.
  *                false  -> if not.
  */
  bool isAlphaNumeric(unsigned char c);

  /** @brief check whether all the characters in the input string are alphanumeric symbol or not.
  *
  *  @param[in]  input the input string to check.
  *
  *  @return bool  true   -> if its a set of alphanumeric symbol.
  *                false  -> if not.
  */
  bool isAlphaNumeric(const std::string &input);

  /** @brief check whether all the characters in the input string are kanji (Japanese) character or not.
  *
  *  @param[in]  input the input string to check.
  *
  *  @return bool  true   -> if its a set of kanji (Japanese) character.
  *                false  -> if not.
  */
  bool isKanji(const std::string &input);
}

#endif    // QRUTILITY_H
