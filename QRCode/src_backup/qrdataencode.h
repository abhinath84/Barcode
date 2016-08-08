
#ifndef QRDATAENCODE_H
#define QRDATAENCODE_H

/// C++-related includes
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>

#include "qrutility.h"
#include "qrbitbuffer.h"

using namespace std;

namespace QR
{
  class QRDataEncode
  {
    public:
      QRDataEncode();
      QRDataEncode(const string &input, const ECL &ecl);
      QRDataEncode(const QRDataEncode &other);
      virtual ~QRDataEncode() = 0;

      QRDataEncode& operator=(const QRDataEncode &other);

      /// Methods
      string getInput() const;
      void setInput(const string &input);
      ECL getErrorCorrectionLevel() const;
      void setErrorCorrectionLevel(const ECL &ecl);
      int getVersion() const;
      void setVersion(int version);
      int getWidth() const;
      void setWidth(int width);
      void appendBit(uint8_t val);
      void appendBit(uint32_t val, int len);
      QRBitBuffer getEncodedData()const;
      void setEncodedData(const vector<uint8_t> &data);
      void setEncodedData(const QRBitBuffer &data);


    public:
      virtual bool calculateVersion();
      virtual bool calculateErrorCorrectionLevel();
      virtual bool addModeIndicator();
      virtual bool addCharCountIndicator();
      virtual bool encode();

    protected:
      bool calculateVersion(int mode);
      bool calculateErrorCorrectionLevel(int mode);
      int getCCILength(int version, int datamode);
      bool appendPadBits();
      bool convertAndAppend(int val, int size);
      int getNumRawDataModules();
      int getNumDataCodewords();
      int calculateCharacterCapacity(int mode, int version, const ECL &ecl);

    protected:
      string        m_input;
      ECL           m_ecl;
      int           m_version;
      int           m_width;
      QRBitBuffer   m_encodedData;

    private:
      static const int16_t NUM_ERROR_CORRECTION_CODEWORDS[4][41];
      static const int CAPACITY_TABLE[][4];
  };

  class QRNumericEncode : public QRDataEncode
  {
    public:
      QRNumericEncode();
      QRNumericEncode(const string &input, const ECL &ecl);
      QRNumericEncode(const QRNumericEncode &other);
      ~QRNumericEncode();

      QRNumericEncode& operator=(const QRNumericEncode &other);

    public:
      virtual bool calculateVersion();
      virtual bool calculateErrorCorrectionLevel();
      virtual bool addModeIndicator();
      virtual bool addCharCountIndicator();
      virtual bool encode();
  };

  class QRAlphanumericEncode : public QRDataEncode
  {
    public:
      QRAlphanumericEncode();
      QRAlphanumericEncode(const string &input, const ECL &ecl);
      QRAlphanumericEncode(const QRAlphanumericEncode &other);
      ~QRAlphanumericEncode();

      QRAlphanumericEncode& operator=(const QRAlphanumericEncode &other);

    public:
      virtual bool calculateVersion();
      virtual bool calculateErrorCorrectionLevel();
      virtual bool addModeIndicator();
      virtual bool addCharCountIndicator();
      virtual bool encode();
  };

  //class QRByteEncode : public QRDataEncode
  //{
  //  public:
  //    QRByteEncode();
  //    QRByteEncode(const string &input, const ECL &ecl);
  //    QRByteEncode(const QRByteEncode &other);
  //    ~QRByteEncode();

  //    QRByteEncode& operator=(const QRByteEncode &other);

  //  public:
  //    virtual bool calculateVersion();
  //    virtual bool calculateErrorCorrectionLevel();
  //    virtual bool addModeIndicator();
  //    virtual bool addCharCountIndicator();
  //    virtual bool encode();
  //};

  //class QRKanjiEncode : public QRDataEncode
  //{
  //  public:
  //    QRKanjiEncode();
  //    QRKanjiEncode(const string &input, const ECL &ecl);
  //    QRKanjiEncode(const QRKanjiEncode &other);
  //    ~QRKanjiEncode();

  //    QRKanjiEncode& operator=(const QRKanjiEncode &other);

  //  public:
  //    virtual bool calculateVersion();
  //    virtual bool calculateErrorCorrectionLevel();
  //    virtual bool addModeIndicator();
  //    virtual bool addCharCountIndicator();
  //    virtual bool encode();
  //};
}

#endif    // QRDATAENCODE_H
