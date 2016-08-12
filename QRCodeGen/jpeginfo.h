/**
*  @file    jpeginfo.h
*  @brief   .
*
*  .
*
*  @author  Abhishek Nath
*  @date    09-Aug-2016
*
*/

#ifndef JPEGINFO_H
#define JPEGINFO_H

/// C++-related includes
#include <iostream>
#include <cstdio>
#include <string>


namespace JPEG
{
  /// typedef
  typedef unsigned char       BYTE ;
  typedef signed char         SBYTE;
  typedef signed short int    SWORD;
  typedef unsigned short int  WORD;
  typedef unsigned long int   DWORD;
  typedef signed long int     SDWORD;


  /// Constant variables
  const BYTE zigzag[64]={ 
                          0, 1, 5, 6,14,15,27,28,
                          2, 4, 7,13,16,26,29,42,
                          3, 8,12,17,25,30,41,43,
                          9,11,18,24,31,40,44,53,
                          10,19,23,32,39,45,52,54,
                          20,22,33,38,46,51,55,60,
                          21,34,37,47,50,56,59,61,
                          35,36,48,49,57,58,62,63 
                         };

  const BYTE std_dc_luminance_nrcodes[18]={0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0,'\0'};
  const BYTE std_dc_luminance_values[12]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  const BYTE std_dc_chrominance_nrcodes[17]={0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0};
  const BYTE std_dc_chrominance_values[12]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  const BYTE std_ac_luminance_nrcodes[17]={0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d };
  const BYTE std_ac_luminance_values[162]= {
        0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
        0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
        0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
        0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
        0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
        0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
        0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
        0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
        0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
        0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
        0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
        0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
        0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
        0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
        0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
        0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
        0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
        0xf9, 0xfa };

  const BYTE std_ac_chrominance_nrcodes[17]={0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};
  const BYTE std_ac_chrominance_values[162]={
        0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
        0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
        0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
        0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
        0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
        0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
        0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
        0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
        0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
        0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
        0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
        0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
        0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
        0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
        0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
        0xf9, 0xfa };

  //!  @struct  APPInfo
  /*!
    This structure contains information about the Application
    marker for the JPEG file. It's a basic structure which store
    information of any type of Application marker, ie: APP0 - APP15.
  */
  typedef struct APPInfo 
  {
    /// Default Constructor
    /// By default it's 'APP0' with identifier 'JFIF'.
    APPInfo()
      :m_marker         (0xFFE0),         // = 0xFFE0
      m_len             (16),             // (16 for usual JPEG, no thumbnail
      m_majorVersion    (1),              // 1
      m_minorVersion    (1),              // 1
      m_unit            (0),              // 0 (no units, normal density
      m_xDensity        (1),              // 1
      m_yDensity        (1),              // 1
      m_thumbnWidth     (0),              // 0
      m_thumbnHeight    (0)               // 0
    {
      memcpy(m_identifier, "JFIF", 5);    // = "JFIF"
    }

    /// Argumented Constructor
    APPInfo(const WORD marker, const WORD len, const BYTE *identifier,
            const BYTE majorVersion, const BYTE minorVersion, const BYTE unit,
            const WORD xDensity, const WORD yDensity, const BYTE thumbnWidth,
            const BYTE thumbnHeight)
      :m_marker          (marker),
      m_len             (len),
      m_majorVersion    (majorVersion),
      m_minorVersion    (minorVersion),
      m_unit            (unit),
      m_xDensity        (xDensity),
      m_yDensity        (yDensity),
      m_thumbnWidth     (thumbnWidth),
      m_thumbnHeight    (thumbnHeight)
    {
      memcpy(m_identifier, identifier, 5);
    }

    /// member variables
    WORD m_marker;                ///< Define the Application marker type, ie, APP0 - APP15. (2 byte)
    WORD m_len;                   ///< Define length of segment excluding APP marker. (2 byte)
    BYTE m_identifier[5];         ///< Define identifier of APP, like : 'JFIF'. It's a NULL terminated string. (5 bytes)
    BYTE m_majorVersion;          ///< Define major version of APP. (1 byte)
    BYTE m_minorVersion;          ///< Define minor version of APP. (1 byte)
    BYTE m_unit;                  ///< Define units for the X and Y densities. (1 byte)
                                  //   units = 0:  no units, X and Y specify the pixel aspect ratio
                                  //   units = 1:  X and Y are dots per inch
                                  //   units = 2:  X and Y are dots per cm
    WORD m_xDensity;              ///< Define horizontal pixel density. (2 bytes)
    WORD m_yDensity;              ///< Define vertical pixel density. (2 bytes)
    BYTE m_thumbnWidth;           ///< Define thumbnail horizontal pixel count. (1 byte)
    BYTE m_thumbnHeight;          ///< Define thumbnail vertical pixel count. (1 byte)

  } APPINFO;



  //!  @struct  SOFComponent
  /*!
    This structure contains information about each color component of 
    the Start of Frame marker for the JPEG file.
    Default color component set for Y.
  */
  struct SOFComponent
  {
    /// Default Constructor
    /// By default it's set component for Y.
    SOFComponent()
      :m_id                 (1),
      m_samplingFactor      (0x11),
      m_quantizationTable   (0)
    {
    }

    /// Argumented Constructor
    SOFComponent(const BYTE id, const BYTE sf, const BYTE qt)
      :m_id                 (id),
      m_samplingFactor      (sf),
      m_quantizationTable   (qt)
    {
    }

    /// member variables
    BYTE m_id;
    BYTE m_samplingFactor;
    BYTE m_quantizationTable;
  };



  //!  @struct  SOFInfo
  /*!
    This structure contains information about the Start of Frame
    marker for the JPEG file. It's a basic structure which store
    information of any type of SOF marker, ie: SOF0 - SOF15.
    Default sampling factors are 1,1 for every image component: No downsampling.
  */
  typedef struct  SOFInfo 
  {
    /// Default Constructor
    /// By default it's 'SOF0'.
    SOFInfo()
      :m_marker       (0xFFC0),           // = 0xFFC0
      m_len           (17),                    // = 17 for a truecolor YCbCr JPG
      m_precision     (8),                // Should be 8: 8 bits/sample
      m_height        (0),
      m_width         (0),
      m_components    (3),                //Should be 3: We encode a truecolor JPG
      m_compY         (1, 0x11, 0),       // sampling factors for Y (bit 0-3 vert., 4-7 hor.)
      m_compCb        (2, 0x11, 1),
      m_compCr        (3, 0x11, 1)
    {
    }

    SOFInfo(const WORD marker, const WORD len, const BYTE precision, const WORD height, const WORD width,
              const BYTE components, const SOFComponent compY, const SOFComponent compCb, const SOFComponent compCr)
      :m_marker       (marker),
      m_len           (len),
      m_precision     (precision),
      m_height        (height),
      m_width         (width),
      m_components    (components),
      m_compY         (compY),
      m_compCb        (compCb),
      m_compCr        (compCr)
    {
    }

    /// member variables
    WORD m_marker;            ///< Define the SOF marker type, ie, SOF0 - SOF15. (2 byte)
    WORD m_len;               ///< Define Start of Frame marker length. (2 byte)
    BYTE m_precision ;        ///< Define precision(Bits per pixel per color component) of SOF. (1 byte) 
    WORD m_height ;           ///< Define height of the Image. (2 bytes)
    WORD m_width;             ///< Define width of the Image. (2 bytes)
    BYTE m_components;        ///< Define the number of color components. (1 byte)
                              //   Should be 3: We encode a truecolor JPG.
    SOFComponent m_compY;     ///< Define elements for Y Component. (3 bytes)
    SOFComponent m_compCb;    ///< Define elements for Cb Component. (3 bytes)
    SOFComponent m_compCr;    ///< Define elements for Cr Component. (3 bytes)

  } SOFINFO;


  //!  @struct  DQTInfo
  /*!
    This structure contains information about the Quantization Table
    marker for the JPEG file.
    m_tableY from DQTinfo should be equal to a scaled and zizag reordered version
    of the table which can be found in std_luminance_qt
    m_tableCb , similar = std_chrominance_qt.
  */
  typedef struct DQTInfo
  {
    DQTInfo()
      :m_marker       (0xFFDB),
      m_len           (132),
      m_scaleFactor   (50),
      m_infoY         (0),
      m_infoCb        (1)
    {
      setQuantizationTableForY();
      setQuantizationTableForCb();
      prepareQuantizationTable();
    }

    DQTInfo(const WORD marker, const WORD len, const BYTE scaleFactor,  
            const BYTE infoY, const BYTE infoCb)
      :m_marker       (marker),
      m_len           (len),
      m_scaleFactor   (scaleFactor),
      m_infoY         (infoY),
      m_infoCb        (infoCb)
    {
      setQuantizationTableForY();
      setQuantizationTableForCb();
      prepareQuantizationTable();
    }

    /// member variables
    WORD  m_marker;             ///< Define the Quantization Table marker type, ie, SOF0 - SOF15. (2 byte)
    WORD  m_len;                ///< Define Quantization Table marker length. (2 byte)
    BYTE  m_scaleFactor;        ///< Define scale factor which controls the visual quality of the image
                                //   the smaller is, the better image we'll get, and the smaller
                                //   compression we'll achieve
    BYTE  m_infoY;              ///< Define information for Y component.
                                //   = 0:  bit 0..3: number of QT = 0 (table for Y)
                                //   bit 4..7: precision of QT, 0 = 8 bit
    BYTE  m_tableY[64];         ///< Define Quantization Table for Y component.
    float m_fdctTableY[64];     ///< Define forward DCT table for Y component.
    BYTE  m_infoCb;             ///< Define information for Cb/Cr component.
    BYTE  m_tableCb[64];        ///< Define Quantization Table for Cb/Cr component.
    float m_fdctTableCb[64];     ///< Define forward DCT table for Y component.

    public:
      void setQuantizationTableForY()
      {
        //  These are the sample quantization tables given in JPEG spec section K.1.
        //  The spec says that the values given produce "good" quality, and
        //  when divided by 2, "very good" quality
        BYTE std_luminance_qt[64] = {
                                      16,  11,  10,  16,  24,  40,  51,  61,
                                      12,  12,  14,  19,  26,  58,  60,  55,
                                      14,  13,  16,  24,  40,  57,  69,  56,
                                      14,  17,  22,  29,  51,  87,  80,  62,
                                      18,  22,  37,  56,  68, 109, 103,  77,
                                      24,  35,  55,  64,  81, 104, 113,  92,
                                      49,  64,  78,  87, 103, 121, 120, 101,
                                      72,  92,  95,  98, 112, 100, 103,  99
                                    };

        setQuantizationTable(std_luminance_qt, m_scaleFactor, m_tableY);
      }

      void setQuantizationTableForCb()
      {
        //  These are the sample quantization tables given in JPEG spec section K.1.
        //  The spec says that the values given produce "good" quality, and
        //  when divided by 2, "very good" quality
        BYTE std_chrominance_qt[64] = {
                                        17,  18,  24,  47,  99,  99,  99,  99,
                                        18,  21,  26,  66,  99,  99,  99,  99,
                                        24,  26,  56,  99,  99,  99,  99,  99,
                                        47,  66,  99,  99,  99,  99,  99,  99,
                                        99,  99,  99,  99,  99,  99,  99,  99,
                                        99,  99,  99,  99,  99,  99,  99,  99,
                                        99,  99,  99,  99,  99,  99,  99,  99,
                                        99,  99,  99,  99,  99,  99,  99,  99
                                      };

        setQuantizationTable(std_chrominance_qt, m_scaleFactor, m_tableCb);
      }

    private:
      void setQuantizationTable(BYTE *basic_table, BYTE scale_factor, BYTE *newtable)
      {
        BYTE i;
        long temp;
        for (i = 0; i < 64; i++) 
        {
          temp = ((long) basic_table[i] * scale_factor + 50L) / 100L;

          //limit the values to the valid range
          if (temp <= 0L) temp = 1L;
          if (temp > 255L) temp = 255L; //limit to baseline range if requested

          newtable[zigzag[i]] = (BYTE) temp;
        }
      }

      // Using a bit modified form of the FDCT routine from IJG's C source:
      // Forward DCT routine idea taken from Independent JPEG Group's C source for
      // JPEG encoders/decoders

      // For float AA&N IDCT method, divisors are equal to quantization
      //   coefficients scaled by scalefactor[row]*scalefactor[col], where
      //   scalefactor[0] = 1
      //   scalefactor[k] = cos(k*PI/16) * sqrt(2)    for k=1..7
      //   We apply a further scale factor of 8.
      //   What's actually stored is 1/divisor so that the inner loop can
      //   use a multiplication rather than a division.
      void prepareQuantizationTable()
      {
        int i = 0;

        double aanScaleFactor[8] = {
                                    1.0, 1.387039845, 1.306562965, 1.175875602,
                                    1.0, 0.785694958, 0.541196100, 0.275899379
                                   };

        for (int row = 0; row < 8; row++)
        {
          for (int col = 0; col < 8; col++)
          {
            m_fdctTableY[i] = (float) (1.0 / ((double) m_tableY[zigzag[i]] *
                                aanScaleFactor[row] * aanScaleFactor[col] * 8.0));

            m_fdctTableCb[i] = (float) (1.0 / ((double) m_tableCb[zigzag[i]] *
                              aanScaleFactor[row] * aanScaleFactor[col] * 8.0));

            i++;
          }
        }
      }

  } DQTINFO;


  //!  @struct  DHTComponent
  /*!
    This structure contains information about the Huffman Table
     components for the JPEG file.
  */
  struct DHTComponent
  {
    DHTComponent()
      :m_index      (0),
      m_type        (-1),
      m_nrcodeSize  (0),
      m_valuesSize  (0)
    {
    }

    DHTComponent(const BYTE index, const BYTE type, const BYTE *nrcodes, 
                const int nrSize, const BYTE *values, const int valSize)
      :m_index      (index),
      m_type        (type),
      m_nrcodeSize  (nrSize),
      m_valuesSize  (valSize)
    {
      if((m_nrcodeSize > 0) && (m_valuesSize > 0))
      {
        /// Allocate memory
        m_nrcodes = new BYTE[m_nrcodeSize];
        m_values = new BYTE[m_valuesSize];

        for (int i = 0; i < m_nrcodeSize; i++)
          m_nrcodes[i]=nrcodes[i+1];

        for (int i = 0; i < m_valuesSize; i++)
          m_values[i]=values[i];
      }
    }

    /// member variables
    BYTE  m_index;
    BYTE  m_type;          // DC = 0, AC = 1
    BYTE  *m_nrcodes;
    int   m_nrcodeSize;
    BYTE  *m_values;
    int   m_valuesSize;
  };


  //!  @struct  DHTInfo
  /*!
    This structure contains information about the Huffman Table
     marker for the JPEG file.
  */
  typedef struct DHTInfo
  {
    DHTInfo()
      :m_marker   (0xFFC4),
      m_len       (0x01A2),
      m_YDC       (0, 0, std_dc_luminance_nrcodes, 16, std_dc_luminance_values, 12),
      m_YAC       (0x10, 1, std_ac_luminance_nrcodes, 16, std_ac_luminance_values, 162),
      m_CbDC      (1, 0, std_dc_chrominance_nrcodes, 16, std_dc_chrominance_values, 12),
      m_CbAC      (0x11, 1, std_ac_chrominance_nrcodes, 16, std_ac_chrominance_values, 162)
    {
    }

    DHTInfo(const WORD maker, const WORD len, 
            const DHTComponent yDC, const DHTComponent yAC,
            const DHTComponent CbDC, const DHTComponent CbAC)
      :m_marker   (maker),
      m_len       (len),
      m_YDC       (yDC),
      m_YAC       (yAC),
      m_CbDC      (CbDC),
      m_CbAC      (CbAC)
    {
    }

    /// member variables
    WORD m_marker;          ///< Define the Hoffman Table marker type, ie, SOF0 - SOF15. (2 byte)
    WORD m_len;             ///< Define Quantization Table marker length. (2 byte)

    DHTComponent m_YDC;      ///< Define HT component for luminance color component of type DC
    DHTComponent m_YAC;      ///< Define HT component for luminance color component of type AC
    DHTComponent m_CbDC;     ///< Define HT component for chrominance color component of type DC
    DHTComponent m_CbAC;     ///< Define HT component for chrominance color component of type AC

  } DHTINFO;


  //!  @struct  SOSInfo
  /*!
    This structure contains information about the Start of Scan
     marker for the JPEG file.
  */
  typedef struct SOSInfo
  {
    SOSInfo()
      :m_marker       (0xFFDA),
      m_len           (12),
      m_components    (3),
      m_idY           (1),
      m_HTY           (0),
      m_idCb          (2),
      m_HTCb          (0x11),
      m_idCr          (3),
      m_HTCr          (0x11),
      m_Ss            (0),
      m_Se            (0x3F),
      m_Bf            (0)
    {
    }

    SOSInfo(const WORD marker, const WORD len, const BYTE components,
            const BYTE idY, const BYTE htY, const BYTE idCb, const BYTE htCb,
            const BYTE idCr, const BYTE htCr, const BYTE ss, const BYTE se, const BYTE bf)
      :m_marker       (marker),
      m_len           (len),
      m_components    (components),
      m_idY           (idY),
      m_HTY           (htY),
      m_idCb          (idCb),
      m_HTCb          (htCb),
      m_idCr          (idCr),
      m_HTCr          (htCr),
      m_Ss            (ss),
      m_Se            (se),
      m_Bf            (bf)
    {
    }

    ///member variables
    WORD m_marker;  // = 0xFFDA
    WORD m_len; // = 12
    BYTE m_components; // Should be 3: truecolor JPG
    BYTE m_idY; //1
    BYTE m_HTY; //0 // bits 0..3: AC table (0..3)
			    // bits 4..7: DC table (0..3)
    BYTE m_idCb; //2
    BYTE m_HTCb; //0x11
    BYTE m_idCr; //3
    BYTE m_HTCr; //0x11
    BYTE m_Ss;
    BYTE m_Se;
    BYTE m_Bf; // not interesting, they should be 0,63,0

  } SOSINFO;

  //!  @struct  RGBApixel
  /*!
    This structure contains information about the color for each pixel of bitmap file.
    It store the red, green, blue, alpha color information.
  */
  typedef struct RGBAInfo
  {
    /// Default Constructor
    RGBAInfo()
      :red (0xff),
      green (0xff),
      blue (0xff),
      alpha (0xff)
    {
    }

    /// Argumented Constructor
    RGBAInfo(unsigned char r, unsigned char g, unsigned char b)
      :red (r),
      green (g),
      blue (b),
      alpha (0)
    {
    }

    /// Argumented Constructor
    RGBAInfo(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
      :red (r),
      green (g),
      blue (b),
      alpha (a)
    {
    }

    BYTE red;                ///< The value of red color (Range: 0 to 255)
    BYTE green;              ///< The value of green color (Range: 0 to 255)
    BYTE blue;               ///< The value of blue color (Range: 0 to 255)
    BYTE alpha;              ///< The value of alpha (Range: 0 to 255)

  }RGB;

  typedef struct
  { 
    BYTE length;
    WORD value;
  } bitstring;


  //!  @struct  HT
  /*!
    .
  */
  struct HT
  {
    public:
      HT()
      {
      }

      void computeHuffmanTable(const BYTE *dc_nrcodes, const BYTE *dc_values,
                                const BYTE *ac_nrcodes, const BYTE *ac_values)
      {
        computeHuffmanTable(dc_nrcodes, dc_values, m_dc);
        computeHuffmanTable(ac_nrcodes, ac_values, m_ac);
      }

      /// member variables
      bitstring m_dc[12];
      bitstring m_ac[256];

    private:
      void computeHuffmanTable(const BYTE *nrcodes, const BYTE *values, bitstring *HT)
      {
        BYTE pos_in_table = 0;
        WORD codevalue = 0;

        for (int k = 1; k <= 16; k++)
        {
          for (int j = 1; j <= nrcodes[k]; j++) 
          {
            HT[values[pos_in_table]].value=codevalue;
            HT[values[pos_in_table]].length=k;
            pos_in_table++;
            codevalue++;
          }

          codevalue*=2;
        }
      }
  };



  //!  @struct  YCbCr
  /*!
    Precalculated tables for a faster YCbCr->RGB transformation
    We use a SDWORD table because we'll scale values by 2^16 and 
    work with integers.
  */
  struct YCbCr
  {
    YCbCr()
    {
      for (int R = 0; R <= 255; R++)
      {
        m_YR[R]=(SDWORD)(65536 * 0.299+0.5)*R;
        m_CbR[R]=(SDWORD)(65536 * -0.16874+0.5)*R;
        m_CrR[R]=(SDWORD)(32768) * R;
      }

      for (int G = 0; G <= 255; G++)
      {
        m_YG[G]=(SDWORD)(65536 * 0.587+0.5)*G;
        m_CbG[G]=(SDWORD)(65536 * -0.33126+0.5)*G;
        m_CrG[G]=(SDWORD)(65536 * -0.41869+0.5)*G;
      }

      for (int B = 0; B <= 255; B++)
      {
        m_YB[B]=(SDWORD)(65536 * 0.114+0.5)*B;
        m_CbB[B]=(SDWORD)(32768) * B;
        m_CrB[B]=(SDWORD)(65536 * -0.08131+0.5)*B;
      }
    }

    SBYTE  Y(const BYTE R, const BYTE G, const BYTE B)
    {
      return((SBYTE)( (m_YR[R] + m_YG[G] + m_YB[B])>>16 ) - 128);
    }

    SBYTE Cb(const BYTE R, const BYTE G, const BYTE B)
    {
      return((SBYTE)( (m_CbR[R] + m_CbG[G] + m_CbB[B])>>16 ) );
    }

    SBYTE Cr(const BYTE R, const BYTE G, const BYTE B)
    {
      return((SBYTE)( (m_CrR[R] + m_CrG[G] + m_CrB[B])>>16 ) );
    }

    /// member variables
    SDWORD m_YR[256], m_YG[256], m_YB[256];
    SDWORD m_CbR[256], m_CbG[256], m_CbB[256];
    SDWORD m_CrR[256], m_CrG[256], m_CrB[256];
  };
}

#endif  // end of JPEGINFO_H
