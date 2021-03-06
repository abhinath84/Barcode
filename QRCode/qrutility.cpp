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

  bool isDigit(const string &input)
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

  bool isAlphaNumeric(const string &input)
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

  bool isKanji(const string &input)
  {
    bool status = false;
    unsigned char c, d;
    unsigned int word;

    c = input[0];
    d = input[1];

    if(d != '\0')
    {
      word = ((unsigned int)c << 8) | d;
      if((word >= 0x8140 && word <= 0x9ffc) || (word >= 0xe040 && word <= 0xebbf))
        status = true;
    }

    return(status);
  }

  int convertToByte(int val, uint8_t *bits)
  {
    int count = 0;

    count = (int)((val < 16) ? sizeof(int) : floor(val/16) * sizeof(int));
    if(bits != NULL)
    {
      free(bits);
      bits = NULL;
    }
    bits = (uint8_t*)malloc(count);

    for(int i=0; i < count; ++i)
    {
        const unsigned shift_amount = count-i-1;
        const unsigned this_bit = (val>>shift_amount)&1;
        *bits = this_bit;
        ++bits;
    }

    return(count);
  }

  void convertToByte(int val, vector<unsigned char> &bits)
  {
    /// base cases
    if(val == 0)
    {
      bits.push_back('0');
      return;
    }
    else if(val == 1)
    {
      bits.push_back('1');
      return;
    }
    else
    {
      convertToByte((val / 2), bits);
      convertToByte((val % 2), bits);
    }
  }
}
