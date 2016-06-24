#include <iostream>
#include <string>
#include <vector>

#include "bitmap.h"
#include "qrcode.h"

using namespace QR;

#define OUT_FILE_PIXEL_PRESCALER  8											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

/// Forward declaration
void doBasicDemo();
void printQR(const QRCode &qr);

int main(int argc, char **argv)
{
  doBasicDemo();

  return(0);
}

// Creates a single QR Code, then prints it to the console.
void doBasicDemo() 
{
  const char *text = "Creo 3.0 : Mfg Model \n ABC.mfg : 24062016::15:31";  // User-supplied text
  ECL errCorLvl = ECL_L;  // Error correction level

  // Make and print the QR Code symbol
  QRCode qr;
  qr.encodeText(text, errCorLvl);

  std::cout << qr.toSvgString(4) << std::endl;
  printQR(qr);
}

void printQR(const QRCode &qr) 
{
  int border = 4;
  int size = qr.getSize();
  // Output the bmp file
  Bitmap bmp(size * OUT_FILE_PIXEL_PRESCALER, size * OUT_FILE_PIXEL_PRESCALER);

  for (int y = -border; y < size + border; y++) 
  {
    for (int x = -border; x < size + border; x++) 
    {
      std::cout << (qr.getModule(x, y) == 1 ? "##" : "  ");
    }
    std::cout << std::endl;
  }

  for(int y = 0; y < size; y++)
  {
      for(int x = 0; x < size; x++)
      {
        if (qr.getModule(x, y) == 1)
        {
          for(int l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
          {
            for(int n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
            {
              bmp.setPixel(l + (x * OUT_FILE_PIXEL_PRESCALER), 
                                n + (y * OUT_FILE_PIXEL_PRESCALER), 
                                0xff, 0, 0);
            }
          }
        }
      }
  }

  // write to file
  bmp.writeToFile("test.bmp");
}
