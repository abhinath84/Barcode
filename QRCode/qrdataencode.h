
#ifndef QRDATAENCODE_H
#define QRDATAENCODE_H

/// C++-related includes
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "qrutility.h"
#include "qrbitbuffer.h"

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
      QRBitBuffer getBits() const;
      void setBits(const QRBitBuffer &bits);
      void appendBit(uint8_t val);
      void appendBit(uint32_t val, int len);

    public:
      virtual bool calculateVersion();
      virtual bool calculateErrorCorrectionLevel();
      virtual bool addModeIndicator();
      virtual bool addCharCountIndicator();
      virtual bool encode();

    protected:
      bool calculateVersion(const int CCT[][4]);
      bool calculateErrorCorrectionLevel(const int CCT[][4]);

    protected:
      string        m_input;
      ECL           m_ecl;
      int           m_version;
      int           m_width;
      QRBitBuffer   m_bits;
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

  //class QRAlphanumericEncode : public QRDataEncode
  //{
  //  public:
  //    QRAlphanumericEncode();
  //    QRAlphanumericEncode(const string &input, const ECL &ecl);
  //    QRAlphanumericEncode(const QRAlphanumericEncode &other);
  //    ~QRAlphanumericEncode();

  //    QRAlphanumericEncode& operator=(const QRAlphanumericEncode &other);

  //  public:
  //    virtual bool calculateVersion();
  //    virtual bool calculateErrorCorrectionLevel();
  //    virtual bool addModeIndicator();
  //    virtual bool addCharCountIndicator();
  //    virtual bool encode();
  //};

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

  const int QRNumeric_CCT[][4] ={
                                  {41, 34, 27, 17}, {77, 63, 48, 34}, {127, 101, 77, 58}, {187, 149, 111, 82}, {255, 202, 144, 106}, {322, 255, 178, 139}, {370, 293, 207, 154}, {461, 365, 259, 202}, {552, 432, 312, 235}, {652, 513, 364, 288},
                                  {772, 604, 427, 331}, {883, 691, 489, 374}, {1022, 796, 580, 427}, {1101, 871, 621, 468}, {1250, 991, 703, 530}, {1408, 1082, 775, 602}, {1548, 1212, 876, 674}, {1725, 1346, 948, 746}, {1903, 1500, 1063, 813}, {2061, 1600, 1159, 919},
                                  {2232, 1708, 1224, 969}, {2409, 1872, 1358, 1056}, {2620, 2059, 1468, 1108}, {2812, 2188, 1588, 1228}, {3057, 2395, 1718, 1286}, {3283, 2544, 1804, 1425}, {3517, 2701, 1933, 1501}, {3669, 2857, 2085, 1581}, {3909, 3035, 2181, 1677}, {4158, 3289, 2358, 1782},
                                  {4417, 3486, 2473, 1897}, {4686, 3693, 2670, 2022}, {4965, 3909, 2805, 2157}, {5253, 4134, 2949, 2301}, {5529, 4343, 3081, 2361}, {5836, 4588, 3244, 2524}, {6153, 4775, 3417, 2625}, {6479, 5039, 3599, 2735}, {6743, 5313, 3791, 2937}, {7089, 5596, 3993, 3057}
                                };

  const int QRDataEncode_CCLT[][4] = {
                                      /// Numeric, Alphanumeric, Byte, Kanji
                                      {10, 9, 8, 8},      /// 1 - 9
                                      {12, 11, 16, 10},   /// 10 - 26
                                      {14, 13, 16, 12}    /// 27 - 40
                                    };
}

#endif    // QRDATAENCODE_H
