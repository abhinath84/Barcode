#include <iostream>
#include "qrcode.h"

using namespace std;
using namespace QR;

int main(int argc, char **argv)
{
  QRCode qrc;

  qrc.encode("Creo 3.0 - mfg model");
  qrc.writeToBMP("test.bmp");

  return(0);
}
