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

#include "qrutility.h"
#include "qrdataencode.h"

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
      unsigned char* getData() const;


      void encode(const string &input, const DATA_MODE &hint = DM_8);
      string decode(unsigned char *dat);

      void writeToBMP(const string &filename);
      void writeToPNG(const string &filename);
      void writeToJPEG(const string &filename);

    private:
      QRDataEncode* getDataEncode();
      void identifyDataMode(const string &input, const DATA_MODE &hint);
      bool encodeData(const string &input);

    private:
      DATA_MODE             m_datamode;
      ECL                   m_ecl;
      int                   m_version;
      int                   m_width;
      unsigned char         *p_data;
      vector<vector<bool> > m_data;
  };

}

#endif    // QRCODE_H
