/**
*  @file    jpeg.h
*  @brief   class to handle jpeg image file.
*
*  .
*
*  @author  Abhishek Nath
*  @date    27-May-2016
*
*/


#ifndef JPEG_H
#define JPEG_H

/// C++-related include
#include <iostream>
#include <cstdio>

#include "jpeginfo.h"

namespace JPEG
{
  class Jpeg
  {
    public:
      Jpeg();
      Jpeg(const WORD height, const WORD width);
      Jpeg(const Jpeg &other);
      ~Jpeg();

      // Assigment Operator
      Jpeg& operator=(const Jpeg &other);

    private:
      FILE   *fp;
      APPINFO m_app0;
  };
}

#endif  // end of JPEG_H