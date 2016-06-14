/**
*  @file    qrcode.h
*  @brief   .
*
*  .
*
*  @author  Abhishek Nath
*  @date    30-May-2016
*
*/

#ifndef QRCODE_H
#define QRCODE_H

/// C++-related includes
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

#include "qrutility.h"
#include "qrdataencode.h"
#include "qrbitbuffer.h"

using namespace std;

namespace QR
{
  class QRCode
  {
    public:
      QRCode();
      QRCode(int version, const ECL &ecl);
      QRCode(const QRCode &other);
      ~QRCode();

      QRCode& operator=(const QRCode &other);

      void setDataMode(const DATA_MODE &dm);
      void setErrorCorrectionLevel(const ECL &ecl);
      void setVersion(int version);
      //void setWidth(int width);
      //void setData(unsigned char *data);

      DATA_MODE getDataMode() const;
      ECL getErrorCorrectionLevel() const;
      int getVersion()const;
      int getWidth()const;
      vector<unsigned char> getData() const;


      void encode(const string &input, const DATA_MODE &hint = DM_8);
      string decode(unsigned char *dat);

      void writeToBMP(const string &filename);
      void writeToPNG(const string &filename);
      void writeToJPEG(const string &filename);

    private:
      void identifyDataMode(const DATA_MODE &hint);
      bool encodeData();
      bool appendErrorCorrection();
      void setEncodedData(const QRBitBuffer &encoded);
      void setInput(const string &input);

    private:
      string                m_input;
      DATA_MODE             m_datamode;
      ECL                   m_ecl;
      int                   m_version;
      int                   m_width;
      QRBitBuffer           m_encoded;
      vector<vector<bool> > m_data;
  };

}

#endif    // QRCODE_H
