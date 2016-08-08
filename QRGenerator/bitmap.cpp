#include "bitmap.h"


Bitmap::Bitmap()
  :m_bitmapFileHeader(),
  m_bitmapInfoHeader(),
  p_pixelArray(NULL),
  p_file(NULL)
{
}

Bitmap::Bitmap(LONG imageWidth, LONG imageHeight)
  :m_bitmapFileHeader(),
  m_bitmapInfoHeader(),
  p_pixelArray(NULL),
  p_file(NULL)
{
  setWidth(imageWidth);
  setHeight(imageHeight);
  m_bitmapFileHeader.m_size += calculatePixelArraySize();
}

Bitmap::Bitmap(LONG imageWidth, LONG imageHeight, WORD bitCount)
  :m_bitmapFileHeader(),
  m_bitmapInfoHeader(),
  p_pixelArray(NULL),
  p_file(NULL)
{
  setWidth(imageWidth);
  setHeight(imageHeight);
  m_bitmapFileHeader.m_size += calculatePixelArraySize();
}

Bitmap::Bitmap(const Bitmap &other)
  :m_bitmapFileHeader(other.m_bitmapFileHeader),
  m_bitmapInfoHeader(other.m_bitmapInfoHeader),
  p_pixelArray(other.p_pixelArray),
  p_file(other.p_file)
{
}

Bitmap& Bitmap::operator=(const Bitmap &other)
{
  if(this != &other)
  {
    m_bitmapFileHeader = other.m_bitmapFileHeader;
    m_bitmapInfoHeader = other.m_bitmapInfoHeader;
    p_pixelArray = other.p_pixelArray;
    p_file = other.p_file;
  }

  return(*this);
}

Bitmap::~Bitmap()
{
  if(p_file != NULL)
  {
    fclose(p_file);
    p_file = NULL;
  }

  if(p_pixelArray != NULL)
    free(p_pixelArray);
}

void Bitmap::setPixelArray(unsigned char *pixelArray)
{
  DWORD pixelArraySize = calculatePixelArraySize();
  if(pixelArraySize > 0)
  {
    if(p_pixelArray == NULL)
      p_pixelArray = (unsigned char*)malloc(pixelArraySize);

    //memset(p_pixelArray, 0xff, m_pixelArraySize);
    memcpy(p_pixelArray, pixelArray, pixelArraySize);
  }
}

void Bitmap::setWidth(LONG width)
{
  this->m_bitmapInfoHeader.m_width = width;
}

void Bitmap::setHeight(LONG height)
{
  this->m_bitmapInfoHeader.m_height = -height;
}

void Bitmap::setSize(LONG width, LONG height)
{
  setWidth(width);
  setHeight(height);
}

void Bitmap::setBitCount(WORD bitCount)
{
  this->m_bitmapInfoHeader.m_bitCount = bitCount;
}

void Bitmap::writeToFile(char *filename)
{
  DWORD pixelArraySize = calculatePixelArraySize();

  if (!(fopen_s(&p_file, filename, "wb")) && (pixelArraySize > 0))
  {
    // set proper size of the bitmap file including pixel array size.
    if(m_bitmapFileHeader.m_size == (14 + 40))
      m_bitmapFileHeader.m_size += pixelArraySize;

    fwrite(&m_bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, p_file);
    fwrite(&m_bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, p_file);
    //if( convertRGBAToFileFormat() )
      fwrite(p_pixelArray, sizeof(unsigned char), pixelArraySize, p_file);

    fclose(p_file);
  }
}

void Bitmap::readFromFile(char *filename)
{

}

DWORD Bitmap::calculatePixelArraySize() const
{
  DWORD pixelArraySize = 0;

  if((abs(m_bitmapInfoHeader.m_width) > 0) && 
      (abs(m_bitmapInfoHeader.m_height) > 0) && 
      (m_bitmapInfoHeader.m_bitCount > 0))
  {
    // get Row size
    DWORD rowSize = (DWORD)(floor((((abs(m_bitmapInfoHeader.m_bitCount) * abs(m_bitmapInfoHeader.m_width)) + 31)/32)) * 4);

    // get Pixel Array Size
    pixelArraySize = rowSize * abs(m_bitmapInfoHeader.m_height);
  }

  return(pixelArraySize);
}

void Bitmap::setPixel(int row, int col, const RGBApixel &rgbaPixel)
{
    setPixelLow(row, col, rgbaPixel.red, rgbaPixel.green, rgbaPixel.blue, rgbaPixel.alpha);
}

void Bitmap::setPixel(int row, int col, int red, int green, int blue)
{
  setPixelLow(row, col, red, green, blue, 0);
}

void Bitmap::setPixel(int row, int col, int red, int green, int blue, int alpha)
{
  setPixelLow(row, col, red, green, blue, alpha);
}

void Bitmap::setPixelLow(int row, int col, int red, int green, int blue, int alpha)
{
  int       c = getCurrentPos(row, col);

  if(c != -1)
  {
    DWORD     pixelArraySize = calculatePixelArraySize();
    if(p_pixelArray == NULL)
    {
        p_pixelArray = (unsigned char*)malloc(pixelArraySize);
        memset(p_pixelArray, 0xff, pixelArraySize);
    }

    p_pixelArray[ c + 0 ] = (unsigned char)red;
    p_pixelArray[ c + 1 ] = (unsigned char)green;
    p_pixelArray[ c + 2 ] = (unsigned char)blue;
  }
}

RGBApixel Bitmap::getPixel(int row, int col) const
{
    RGBApixel rgbaPixel;

    int c = getCurrentPos(row, col);
    if(c != -1)
    {
        rgbaPixel.red = p_pixelArray[c + 0];
        rgbaPixel.green = p_pixelArray[c + 1];
        rgbaPixel.blue = p_pixelArray[c + 2];
    }

    return(rgbaPixel);
}

int Bitmap::getCurrentPos(int row, int col) const
{
    int pos = -1;

    if(((row >= 0) && (row < abs(m_bitmapInfoHeader.m_width))) && 
        ((col >= 0) && (col < abs(m_bitmapInfoHeader.m_height))))
    {
        if((abs(m_bitmapInfoHeader.m_width) > 0) && (abs(m_bitmapInfoHeader.m_height) > 0))
            pos = (row * abs(m_bitmapInfoHeader.m_height) * 3) + (col * 3);
    }

    return(pos);
}
