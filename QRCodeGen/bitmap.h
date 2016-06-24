/**
*  @file    bitmap.h
*  @brief   class to handle bitmap file.
*
*  Bitamp class provides methods for loading and saving raster images.
*  An Bitmap object encapsulates a bitmap or a metafile and
*  provides member functions to manipulate the bitmap.
*
*  @author  Abhishek Nath
*  @date    27-May-2016
*
*/


#ifndef BITMAP_H
#define BITMAP_H

/// c++-related includes
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>


using namespace std;

// typedef
typedef unsigned short  WORD;
typedef unsigned int    DWORD;
typedef signed int      LONG;

// define
#define BI_RGB          0L          /// An uncomprassed format for bitmap


/// To pack a class is to place its members directly after each other in memory,
/// which can mean that some or all members can be aligned on a boundary smaller
/// than the default alignment the target architecture.
#pragma pack(push, 2)


//!  @struct  BITMAPFILEHEADER
/*!
  This structure contains information about the type, size,
  and layout of a file that containing a device-independent bitmap (DIB).
*/
typedef struct BitmapFileHeader
{
  /// Default Constructor
  BitmapFileHeader()
  {
    this->m_type  = 0x4d42;         // "BM"
    this->m_size = (14 + 40);       //sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    this->m_reserved1 = 0;
    this->m_reserved2 = 0;
    this->m_offBits = (14 + 40);    //sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
  }

  /// Argumented Constructor
  BitmapFileHeader(DWORD size, WORD res1, WORD res2)
  {
    this->m_type  = 0x4d42;         // "BM"
    this->m_size = size;
    this->m_reserved1 = res1;
    this->m_reserved2 = res2;
    this->m_offBits = (14 + 40);    //sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
  }

  /// member variables.
  WORD    m_type;                   ///< The file type; must be 'BM'.
  DWORD   m_size;                   ///< The size, in bytes, of the bitmap file. It must be :
                                    ///< = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(PixelArray)
  WORD    m_reserved1;              ///< Reserved; must be zero.
  WORD    m_reserved2;              ///< Reserved; must be zero.
  DWORD   m_offBits;                ///< The offset, in bytes, from the beginning of the BITMAPFILEHEADER
                                    ///< structure to the bitmap bits. It must be = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEA
}BITMAPFILEHEADER;


//!  @struct  BITMAPINFOHEADER
/*!
  This structure contains information about the dimensions and color format of a DIB.
  Store the detailed information about BITMAP Image and define the pixel format.
  In memory this structure immediately follows the BITMAP File Header (BITMAPFILEHEADER).
*/
typedef struct BitmapInfoHeader
{
  /// Default Constructor
  BitmapInfoHeader()
  {
    this->m_size = 40;                  //sizeof(BitmapInfoHeader);
    this->m_width = 0;
    this->m_height = -0;
    this->m_planes = 1;
    this->m_bitCount = 24;
    this->m_compression = BI_RGB;
    this->m_sizeImage = 0;
    this->m_xPelsPerMeter = 0;
    this->m_yPelsPerMeter = 0;
    this->m_clrUsed = 0;
    this->m_clrImportant = 0;
  }

  /// Argumented Constructor
  BitmapInfoHeader(LONG width, LONG height, WORD bitCount,
                    DWORD sizeImage, LONG xPelsPerMeter,
                    LONG yPelsPerMeter, DWORD clrUsed, DWORD clrImportant)
  {
    this->m_size = 40;                  //sizeof(BitmapInfoHeader);
    this->m_width = width;
    this->m_height = -height;
    this->m_planes = 1;
    this->m_bitCount = bitCount;
    this->m_compression = BI_RGB;
    this->m_sizeImage = sizeImage;
    this->m_xPelsPerMeter = xPelsPerMeter;
    this->m_yPelsPerMeter = yPelsPerMeter;
    this->m_clrUsed = clrUsed;
    this->m_clrImportant = clrImportant;
  }

  // member variables
  DWORD      m_size;                ///< The number of bytes required by the structure.
  LONG       m_width;               ///< The width of the bitmap, in pixels.
  LONG       m_height;              ///< The height of the bitmap, in pixels. If biHeight is positive, the bitmap
                                    ///< is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative,
                                    ///< the bitmap is a top-down DIB and its origin is the upper-left corner.
                                    ///< If biHeight is negative, indicating a top-down DIB, biCompression must be either
                                    ///< BI_RGB or BI_BITFIELDS. Top-down DIBs cannot be compressed. If biCompression is BI_JPEG or BI_PNG,
                                    ///< the biHeight member specifies the height of the decompressed JPEG or PNG image file, respectively.
  WORD       m_planes;              ///< The number of planes for the target device. This value must be set to 1.
  WORD       m_bitCount;            ///< The number of bits-per-pixel. Normally this value should be 24.
  DWORD      m_compression;         ///< The type of compression for a compressed bottom-up bitmap (top-down DIBs cannot be compressed). Normally it's "BI_RGB".
  DWORD      m_sizeImage;           ///< The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
  LONG       m_xPelsPerMeter;       ///< The horizontal resolution, in pixels-per-meter, of the target device for the bitmap.
  LONG       m_yPelsPerMeter;       ///< The vertical resolution, in pixels-per-meter, of the target device for the bitmap.
  DWORD      m_clrUsed;             ///< The number of color indexes in the color table that are actually used by the bitmap.
                                    ///< If this value is zero, the bitmap uses the maximum number of colors corresponding to
                                    ///< the value of the biBitCount member for the compression mode specified by biCompression.
  DWORD      m_clrImportant;        ///< The number of color indexes that are required for displaying the bitmap. If this value is zero, all colors are required.
}BITMAPINFOHEADER;

#pragma pack(pop)


//!  @struct  RGBApixel
/*!
  This structure contains information about the color for each pixel of bitmap file.
  It store the red, green, blue, alpha color information.
*/
struct RGBApixel
{
  /// Default Constructor
  RGBApixel()
  {
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
  }

  /// Argumented Constructor
  RGBApixel(unsigned char r, unsigned char g, unsigned char b)
  {
    red = r;
    green = g;
    blue = b;
    alpha = 0;
  }

  /// Argumented Constructor
  RGBApixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
  {
    red = r;
    green = g;
    blue = b;
    alpha = a;
  }

  unsigned char red;                ///< The value of red color (Range: 0 to 255)
  unsigned char green;              ///< The value of green color (Range: 0 to 255)
  unsigned char blue;               ///< The value of blue color (Range: 0 to 255)
  unsigned char alpha;              ///< The value of alpha (Range: 0 to 255)
};


//!  @class  Bitmap
/*!
  Bitamp class provides methods for loading and saving raster images.
  An Bitmap object encapsulates a bitmap or a metafile and
  provides member functions to manipulate the bitmap.
*/
class Bitmap
{
public:
  /// Default Constructor
  Bitmap();

  /// Parametric Constructor
  /// Set width and height of the bitmap image. also set
  /// bit per pixel for the bitmap.
  /// Calculate the size of pixel array and append it
  /// to bitmap info header size.
  /// Set default value for rest of the member variables.
  Bitmap(LONG imageWidth, LONG imageHeight);

  /// Parametric Constructor
  /// Set width and height of the bitmap image. also set
  /// bit per pixel for the bitmap.
  /// Calculate the size of pixel array and append it
  /// to bitmap info header size.
  /// Set default value for rest of the member variables.
  Bitmap(LONG imageWidth, LONG imageHeight, WORD bitCount);

  /// Copy Constructor
  Bitmap(const Bitmap &other);

  /// Destructor
  ~Bitmap(void);

  /// Assignment Operator
  Bitmap& operator=(const Bitmap &other);

  /** @brief set pixel array which contain the color information for each pixel.
  *
  *  Set pixel array which contain the color information for each pixel.
  *  Its allocate memory for pixel array according to the width and height
  *  of bitmap image and store color info for each pixel.
  *
  *  @param[in]  pixelArray the handle of the pixel array.
  *
  *  @return nothing.
  */
  void setPixelArray(unsigned char *pixelArray);

  /** @brief set pixel array which contain the color information for each pixel.
  *
  *  Set pixel array which contain the color information for each pixel.
  *  Its allocate memory for pixel array according to the width and height
  *  of bitmap image and store color info for each pixel.
  *
  *  @param[in]  pixelArray the handle of the pixel array.
  *
  *  @return nothing.
  */
  void setPixel(int row, int col, const RGBApixel &rgbaPixel);
  void setPixel(int row, int col, int red, int green, int blue);
  void setPixel(int row, int col, int red, int green, int blue, int alpha);
  RGBApixel getPixel(int row, int col) const;

  void setWidth(LONG width);
  void setHeight(LONG height);
  void setSize(LONG width, LONG height);
  void setBitCount(WORD bitCount);

  void writeToFile(char *filename);
  void readFromFile(char *filename);

private:
  DWORD calculatePixelArraySize() const;
  //bool  convertRGBAToFileFormat();
  int   getCurrentPos(int row, int col) const;
  void  setPixelLow(int row, int col, int red, int green, int blue, int alpha);

private:
  BITMAPFILEHEADER  m_bitmapFileHeader;
  BITMAPINFOHEADER  m_bitmapInfoHeader;
  unsigned char*    p_pixelArray;
  fstream           fs;
};

#endif      // BITMAP_H

