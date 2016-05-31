#include <iostream>
#include "qrcode.h"

using namespace std;
using namespace QR;

int main(int argc, char **argv)
{
  QRCode qrc;

  qrc.encode("Creo3.0 - Mfg Model");
  qrc.writeToBMP("test.bmp");

  return(0);
}
