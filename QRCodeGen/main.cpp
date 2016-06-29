#include <iostream>
#include <string>
#include <vector>

#include "bitmap.h"
#include "qrcode.h"

using namespace QR;

#define OUT_FILE_PIXEL_PRESCALER  8               // Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

/// Forward declaration
void doBasicDemo();
void doVarietyDemo();
void doSegmentDemo();

void printQR(const QRCode &qr);

int main(int argc, char **argv)
{
  //doBasicDemo();
  doVarietyDemo();
  //doSegmentDemo();

  return(0);
}

// Creates a single QR Code, then prints it to the console.
void doBasicDemo() 
{
  const char *text = "123"; //"Creo 3.0 : Mfg Model \n ABC.mfg : 24062016::15:31";  // User-supplied text
  ECL errCorLvl = ECL_L;  // Error correction level

  // Make and print the QR Code symbol
  QRCode qr;
  qr.encode(text, errCorLvl);
  qr.writeToBMP("doBasicDemo_qr0.bmp");

  //std::cout << qr.toSvgString(4) << std::endl;
  //printQR(qr);
}

void doVarietyDemo()
{
  QRCode qr0;
  //qr0.encode("https://www.nayuki.io/", ECL_H, 3);
  //qr0.encode("314159265358979323846264338327950288419716939937510",
  //                ECL_M);
  //qr0.encode("DOLLAR-AMOUNT:$39.87 PERCENTAGE:100.00% OPERATIONS:+-*/", 
  //                ECL_H);
  qr0.encode("\xE3\x81\x93\xE3\x82\x93\xE3\x81\xAB\xE3\x81\xA1wa\xE3\x80\x81\xE4\xB8\x96\xE7\x95\x8C\xEF\xBC\x81\x20\xCE\xB1\xCE\xB2\xCE\xB3\xCE\xB4",
                  ECL_Q);
  qr0.writeToBMP("doVarietyDemo-qr0.bmp");
  //qr0.encode(
  //  "Alice was beginning to get very tired of sitting by her sister on the bank, "
  //  "and of having nothing to do: once or twice she had peeped into the book her sister was reading, "
  //  "but it had no pictures or conversations in it, 'and what is the use of a book,' thought Alice "
  //  "'without pictures or conversations?' So she was considering in her own mind (as well as she could, "
  //  "for the hot day made her feel very sleepy and stupid), whether the pleasure of making a "
  //  "daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly "
  //  "a White Rabbit with pink eyes ran close by her.", ECL_H);
  //printQR(qr0);
}

void doSegmentDemo()
{
  std::string silver0 = "THE SQUARE ROOT OF 2 IS 1.";
  std::string silver1 = "41421356237309504880168872420969807856967187537694807317667973799";

  QRCode qr0;
  qr0.encode(std::string(silver0) + silver1, ECL_L);
  qr0.writeToBMP("qr0.bmp");

  std::vector<QRSegment> segs;

  QRSegment seg0, seg1;
  seg0.create(silver0);
  seg1.create(silver1);

  segs.push_back(seg0);
  segs.push_back(seg1);

  QRCode qr1;
  qr1.encode(segs, ECL_L);
  qr1.writeToBMP("qr1.bmp");

  std::string golden0 = "Golden ratio \xCF\x86 = 1.";
  std::string golden1 = "6180339887498948482045868343656381177203091798057628621354486227052604628189024497072072041893911374";
  std::string golden2 = "......";

  segs.clear();
  std::vector<uint8_t> bytes;
  for (int i = 0; i < (int)golden0.size(); ++i)
    bytes.push_back(static_cast<uint8_t>(golden0[i]));

  QRSegment seg2, seg3, seg4;
  seg2.create(bytes);
  seg3.create(golden1);
  seg4.create(golden2);

  segs.push_back(seg2);
  segs.push_back(seg3);
  segs.push_back(seg4);

  QRCode qr2;
  qr2.encode(segs, ECL_L);
  qr2.writeToBMP("qr2.bmp");
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
