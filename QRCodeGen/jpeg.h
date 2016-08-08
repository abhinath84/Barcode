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


// typedef
typedef unsigned char       BYTE ;
typedef signed char         SBYTE;
typedef signed short int    SWORD;
typedef unsigned short int  WORD;
typedef unsigned long int   DWORD;
typedef signed long int     SDWORD;


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
  {
    m_marker          = 0xFFE0;             // = 0xFFE0
    m_len             = 16;                 // = 16 for usual JPEG, no thumbnail
    memcpy(m_identifier, "JFIF", 5);        // = "JFIF"
    m_majorVersion    = 1;                  // 1
    m_minorVersion    = 1;                  // 1
    m_unit            = 0;                  // 0 = no units, normal density
    m_xDensity        = 1;                  // 1
    m_yDensity        = 1;                  // 1
    m_thumbnWidth     = 0;                  // 0
    m_thumbnHeight    = 0;                  // 0
  }

  /// Argumented Constructor
  APPInfo(const WORD marker, const WORD len, const BYTE *identifier,
          const BYTE majorVersion, const BYTE minorVersion, const BYTE unit,
          const WORD xDensity, const WORD yDensity, const BYTE thumbnWidth,
          const BYTE thumbnHeight)
  {
    m_marker          = marker;
    m_len             = len;
    memcpy(m_identifier, identifier, 5);
    m_majorVersion    = majorVersion;
    m_minorVersion    = minorVersion;
    m_unit            = unit;
    m_xDensity        = xDensity;
    m_yDensity        = yDensity;
    m_thumbnWidth     = thumbnWidth;
    m_thumbnHeight    = thumbnHeight;
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
  {
    m_marker        = 0xFFC0;   // = 0xFFC0
    m_len           = 17;       // = 17 for a truecolor YCbCr JPG
    m_precision     = 8;        // Should be 8: 8 bits/sample
    m_height        = 0;
    m_width         = 0;
    m_components    = 3;        //Should be 3: We encode a truecolor JPG

    m_idY           = 1;        // = 1
    m_idCb          = 2;        // = 2
    m_idCr          = 3;        // = 3

    m_SFY           = 0x11;     // sampling factors for Y (bit 0-3 vert., 4-7 hor.)
    m_SMCb          = 0x11;
    m_SFCr          = 0x11;

    m_QTY           = 0;        // Quantization Table number for Y = 0
    m_QTCb          = 1;        // 1
    m_QTCr          = 2;        // Normally equal to QTCb = 1
  }

  /// member variables
  WORD m_marker;            ///< Define the SOF marker type, ie, SOF0 - SOF15. (2 byte)
  WORD m_len;               ///< Define Start of Frame length. (2 byte)
  BYTE m_precision ;        ///< Define precision(Bits per pixel per color component) of SOF. (1 byte) 
  WORD m_height ;           ///< Define height of the Image. (2 bytes)
  WORD m_width;             ///< Define width of the Image. (2 bytes)
  BYTE m_components;        ///< Define the number of color components. (1 byte)
                            //   Should be 3: We encode a truecolor JPG.
  BYTE m_idY;               ///< Define the ID for Y. (1 byte)
  BYTE m_idCb;              ///< Define the ID for Cb. (1 byte)
  BYTE m_idCr;              ///< Define the ID for Cr. (1 byte)
  
  BYTE m_SFY;               ///< Define the sampling factor for Y (bit 0-3 vert., 4-7 hor.). (1 byte)
  BYTE m_SMCb;              ///< Define the sampling factor for Cb (bit 0-3 vert., 4-7 hor.). (1 byte)
  BYTE m_SFCr;              ///< Define the sampling factor for Cr (bit 0-3 vert., 4-7 hor.). (1 byte)

  BYTE m_QTY;               ///< Define the Quantization Table number for Y. (1 byte)
  BYTE m_QTCb;              ///< Define the Quantization Table number for Cb. (1 byte)
  BYTE m_QTCr;              ///< Define the Quantization Table number for Cr. (1 byte)

} SOFINFO;

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
};

#endif  // end of JPEG_H