#include <iostream>
#include "qrcode.h"

using namespace std;
using namespace QR;

int main(int argc, char **argv)
{
  QRCode qrc;

  qrc.encode("12345698");
  qrc.writeToBMP("test.bmp");

  return(0);
}
