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
      void writebyte(const char byte);
      void writeword(const WORD w);

      /// write markers
      void writeAPP();
      void writeSOF();
      void writeDQT();
      void writeDHT();
      void writeSOS();
      void writeComment(BYTE *comment);
      void writeBits(const bitstring &bs);

    private:
      /// marker
      APPINFO   m_app0;
      SOFINFO   m_sof0;
      DQTINFO   m_dqt;
      DHTINFO   m_dht;
      SOSINFO   m_sos0;

      RGB       *p_rgb;
      FILE      *p_file;
  };
}

#endif  // end of JPEG_H