#include "qrutility.h"

namespace QR
{
  bool isDigit(unsigned char c)
  {
    bool status = false;

    if((unsigned char)((signed char)(c) - '0') < 10)
      status = true;

    return(status);
  }

  bool isDigit(const std::string &input)
  {
    bool status = false;
    unsigned char c;

    for(int i = 0; i < (int)input.size(); ++i)
    {
      status = true;

      c = input[i];
      if(isDigit(c) == false)
      {
        status = false;
        break;
      }
    }

    return(status);
  }

  int getAlphanumeric(unsigned char c)
  {
    /// Alphabet-numeric data
    const signed char alphaNumeric_table[128] =
    {
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      36, -1, -1, -1, 37, 38, -1, -1, -1, -1, 39, 40, -1, 41, 42, 43,
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 44, -1, -1, -1, -1, -1,
      -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
      25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };

    int val = ((c & 0x80) ? -1 : alphaNumeric_table[(int)c]);

    return(val);
  }

  bool isAlphaNumeric(unsigned char c)
  {
    bool status = false;

    if(getAlphanumeric(c) >= 0)
      status = true;

    return(status);
  }

  bool isAlphaNumeric(const std::string &input)
  {
    bool status = false;
    unsigned char c;

    for(int i = 0; i < (int)input.size(); ++i)
    {
      status = true;
      c = input[i];

      if(isAlphaNumeric(c) == false)
      {
        status = false;
        break;
      }
    }

    return(status);
  }

  /** @brief check whether all the characters in the input string are kanji (Japanese) character or not.
  *
  *  @param[in]  input the input string to check.
  *
  *  @return bool  true   -> if its a set of kanji (Japanese) character.
  *                false  -> if not.
  */
  bool isKanji(const std::string &input)
  {
    bool status = false;
    unsigned char c, d;
    unsigned int word;

    c = input[0];
    d = input[1];

    /// check for valid input
    if(d != '\0')
    {
      /// Japanese characters take 2-bytes
      word = ((unsigned int)c << 8) | d;
      if((word >= 0x8140 && word <= 0x9ffc) || (word >= 0xe040 && word <= 0xebbf))
        status = true;
    }

    return(status);
  }
}
