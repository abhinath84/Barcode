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
      m_compCr        (3, 0x11, 2)
    {
    }

    SOFInfo(const WORD marker, const WORD len, const BYTE precision, const WORD height, const WORD width,
              const BYTE components, const BYTE idY, const BYTE idCb, const BYTE idCr, const BYTE sfY,
              const BYTE sfCb, const BYTE sfCr, const BYTE qtY, const BYTE qtCb, const BYTE qtCr)
      :m_marker       (marker),
      m_len           (len),
      m_precision     (precision),
      m_height        (height),
      m_width         (width),
      m_components    (components),
      m_compY         (idY, sfY, qtY),
      m_compCb        (idCb, sfCb, qtCb),
      m_compCr        (idCr, sfCr, qtCr)
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
      :m_marker(0xFFDB),
      m_len(132),
      m_scaleFactor(50),
      m_infoY(0),
      m_infoCb(1)
    {
      setQuantizationTableForY();
      setQuantizationTableForCb();
    }

    DQTInfo(const WORD marker, const WORD len, const BYTE scaleFactor,  
            const BYTE infoY, const BYTE infoCb)
      :m_marker(marker),
      m_len(len),
      m_scaleFactor(scaleFactor),
      m_infoY(infoY),
      m_infoCb(infoCb)
    {
      setQuantizationTableForY();
      setQuantizationTableForCb();
    }

    /// member variables
    WORD m_marker;            ///< Define the Quantization Table marker type, ie, SOF0 - SOF15. (2 byte)
    WORD m_len;               ///< Define Quantization Table marker length. (2 byte)
    BYTE m_scaleFactor;       ///< Define scale factor which controls the visual quality of the image
                              //   the smaller is, the better image we'll get, and the smaller
                              //   compression we'll achieve
    BYTE m_infoY;             ///< Define information for Y component.
                              //   = 0:  bit 0..3: number of QT = 0 (table for Y)
                              //   bit 4..7: precision of QT, 0 = 8 bit
    BYTE m_tableY[64];        ///< Define Quantization Table for Y component.
    BYTE m_infoCb;            ///< Define information for Cb/Cr component.
    BYTE m_tableCb[64];       ///< Define Quantization Table for Cb/Cr component.

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

  } DQTINFO;

  struct HTComponent
  {
    /// member variables
    BYTE m_index;
    BYTE m_type;          // DC = 0, AC = 1
    BYTE m_nrcodes[16];
    BYTE m_values[12];
  };

  typedef struct DHTInfo
  {
    WORD marker;  // = 0xFFC4
    WORD length;  //0x01A2

    BYTE HTYDCinfo; // bit 0..3: number of HT (0..3), for Y =0
                    //bit 4  :type of HT, 0 = DC table,1 = AC table
                    //bit 5..7: not used, must be 0
    BYTE YDC_nrcodes[16]; //at index i = nr of codes with length i
    BYTE YDC_values[12];
    BYTE HTYACinfo; // = 0x10
    BYTE YAC_nrcodes[16];
    BYTE YAC_values[162];//we'll use the standard Huffman tables
    BYTE HTCbDCinfo; // = 1
    BYTE CbDC_nrcodes[16];
    BYTE CbDC_values[12];
    BYTE HTCbACinfo; //  = 0x11
    BYTE CbAC_nrcodes[16];
    BYTE CbAC_values[162];

  } DHTINFO;

}

#endif  // end of JPEGINFO_H
