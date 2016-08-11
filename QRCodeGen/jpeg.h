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

      void setHeight(const WORD height);
      void setWidth(const WORD width);

      void setJPEGPixel(int row, int col, int red, int green, int blue);
      void writeToFile(const char *filename);

    private:
      void initComponents();
      void initHoffmanTable();
      void initCategoryAndBit();

      void encodePixels();
      void loadDUFromRGBPixels(const WORD xpos, const WORD ypos);
      void calculateFDCTAndQuantization(const SBYTE *data, const float *fdtbl, SWORD *outdata);

      void writeDU(const SBYTE *ComponentDU, const float *fdtbl, SWORD *DC, const HT &ht);
      void writebyte(const char byte);
      void writeword(const WORD w);

      /// write markers
      void writeAPP();
      void writeSOF();
      void writeDQT();
      void writeDHT();
      void writeHTComponent(const DHTComponent &htc);
      void writeSOS();
      void writeComment(BYTE *comment);
      void writeBits(const bitstring &bs);

      /// similar to void process_DU(SBYTE *ComponentDU,float *fdtbl,SWORD *DC,
		  ///                             bitstring *HTDC,bitstring *HTAC)
      void writeDataUnit(const HT &ht);

    private:
      /// marker
      APPINFO   m_app0;
      SOFINFO   m_sof0;
      DQTINFO   m_dqt;
      DHTINFO   m_dht;
      SOSINFO   m_sos0;

      HT        m_htY;
      HT        m_htCb;
      YCbCr     m_ycbcr;

      // This is the Data Unit of Y after YCbCr->RGB transformation
      SBYTE     m_Y[64];
      SBYTE     m_Cb[64];
      SBYTE     m_Cr[64];
      BYTE      m_bytenew; // The byte that will be written in the JPG file
      SBYTE     m_bytepos; //bit position in the byte we write (bytenew)

      FILE      *p_file;
      RGB       *p_rgb;
      BYTE      *p_categoryAlloc;
      BYTE      *p_category;        //Here we'll keep the category of the numbers in range: -32767..32767
      bitstring *p_bitAlloc;
      bitstring *p_bit;             // their bitcoded representation
  };
}

#endif  // end of JPEG_H