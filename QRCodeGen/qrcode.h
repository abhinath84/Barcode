#ifndef QRCODE_H
#define QRCODE_H

#include <algorithm>
#include <vector>

#include "qrbitbuffer.h"
#include "qrsegment.h"

namespace QR
{
  class QRCode
  {
    public:
      QRCode();
      QRCode(const QRCode  &other);
      ~QRCode();

      QRCode& operator=(const QRCode &other);

      int getECLFormatBits();
      int getTotalBits(const std::vector<QRSegment> &segs, int version);

      /* 
      * Returns a QR Code symbol representing the given Unicode text string at the given error correction level.
      * As a conservative upper bound, this function is guaranteed to succeed for strings that have 738 or fewer Unicode
      * code points (not UTF-16 code units). The smallest possible QR Code version is automatically chosen for the output.
      * The ECC level of the result may be higher than the ecl argument if it can be done without increasing the version.
      */
      void encodeText(const std::string &input, const ECL &ecl);

      /* 
      * Returns a QR Code symbol representing the given binary data string at the given error correction level.
      * This function always encodes using the binary segment mode, not any text mode. The maximum number of
      * bytes allowed is 2953. The smallest possible QR Code version is automatically chosen for the output.
      * The ECC level of the result may be higher than the ecl argument if it can be done without increasing the version.
      */
      void encodeBinary(const std::vector<uint8_t> &data, const ECL &ecl);

      /* 
      * Returns a QR Code symbol representing the specified data segments with the specified encoding parameters.
      * The smallest possible QR Code version within the specified range is automatically chosen for the output.
      * This function allows the user to create a custom sequence of segments that switches
      * between modes (such as alphanumeric and binary) to encode text more efficiently.
      * This function is considered to be lower level than simply encoding text or binary data.
      */
      void encodeSegments(const std::vector<QRSegment> &segs, const ECL &ecl,
                            int minVersion=1, int maxVersion=40, int mask=-1, bool boostEcl=true);  // All optional parameters

    private:
      // Returns the number of raw data modules (bits) available at the given version number.
      // These data modules are used for both user data codewords and error correction codewords.
      // This stateless pure function could be implemented as a 40-entry lookup table.
      int getRawDataModulesCount(int version);

      // Returns the number of 8-bit data (i.e. not error correction) codewords contained in any
      // QR Code of the given version number and error correction level, with remainder bits discarded.
      // This stateless pure function could be implemented as a (40*4)-cell lookup table.
      int getDataCodewordsCount(int version, const ECL &ecl);

      void makeQRCode(int version, const ECL &ecl, const std::vector<uint8_t> &dataCodewords, int mask);

    private:
      /* This QR Code symbol's version number, which is always between 1 and 40 (inclusive). */
      int m_version;
	
      /* The width and height of this QR Code symbol, measured in modules.
	      * Always equal to version &times; 4 + 17, in the range 21 to 177. */
      int m_size;
	
      /* The error correction level used in this QR Code symbol. */
      ECL m_ecl;

      int mask;

      // Private grids of modules/pixels (conceptually immutable)
      std::vector<std::vector<bool>> m_modules;     // The modules of this QR Code symbol (false = white, true = black)
      std::vector<std::vector<bool>> m_isFunction;  // Indicates function modules that are not subjected to masking

    private:
      static const int16_t NUM_ERROR_CORRECTION_CODEWORDS[4][41];
      static const int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41];
  };
}

#endif    // QRCODE_H
