#ifndef QRUTILITY_H
#define QRUTILITY_H

namespace QR
{
  /// define system datatype
  typedef unsigned char     uint8_t;
  typedef unsigned short    uint16_t;
  typedef unsigned int      uint32_t;
  typedef unsigned long int uint64_t;
  typedef signed char       int8_t;
  typedef short             int16_t;

  //!  @enum  DATA_MODE
  /*!
    Data modes for encoding text.Each mode encodes the text as a string of bits (1s and 0s),
    but each mode uses a different method for converting the text into bits.
    Each method is optimized to generate the shortest possible string of bits for that data type.
  */
  typedef enum DATA_MODE
  {
    DM_NUL = -1,  ///< Terminator (NUL character). Internal use only
    DM_NUM = 0,   ///< Numeric mode
    DM_AN,        ///< Alphabet-numeric mode
    DM_8,         ///< 8-bit data mode
    DM_KANJI,     ///< Kanji (shift-jis) mode
    DM_STRUCTURE, ///< Internal use only
    DM_ECI,       ///< ECI mode
    DM_FNC1FIRST,  ///< FNC1, first position
    DM_FNC1SECOND, ///< FNC1, second position
  } DM;
}

#endif    // QRUTILITY_H
