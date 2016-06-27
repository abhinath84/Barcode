#ifndef QRCODE_H
#define QRCODE_H

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

#include "qrutility.h"
#include "qrbitbuffer.h"
#include "qrsegment.h"
#include "qrreedsolomongenerator.h"

namespace QR
{
  class QRCode
  {
    public:
      QRCode();
      QRCode(const QRCode  &other);
      ~QRCode();

      QRCode& operator=(const QRCode &other);

      /// Member variable get method
      int getECLFormatBits();
      int getTotalBits(const std::vector<QRSegment> &segs, int version);
      int getMask() const;
      int getSize() const;

      /* 
      * Returns the color of the module (pixel) at the given coordinates, which is either 0 for white or 1 for black. The top
      * left corner has the coordinates (x=0, y=0). If the given coordinates are out of bounds, then 0 (white) is returned.
      */
      int getModule(int x, int y) const;

      /* 
      * Based on the given number of border modules to add as padding, this returns a
      * string whose contents represents an SVG XML file that depicts this QR Code symbol.
      * Note that Unix newlines (\n) are always used, regardless of the platform.
      */
      std::string toSvgString(int border) const;

      /* 
      * Returns a QR Code symbol representing the given Unicode text string at the given error correction level.
      * As a conservative upper bound, this function is guaranteed to succeed for strings that have 738 or fewer Unicode
      * code points (not UTF-16 code units). The smallest possible QR Code version is automatically chosen for the output.
      * The ECC level of the result may be higher than the ecl argument if it can be done without increasing the version.
      */
      void encode(const std::string &input, const ECL &ecl, int mask = -1);

      /* 
      * Returns a QR Code symbol representing the given binary data string at the given error correction level.
      * This function always encodes using the binary segment mode, not any text mode. The maximum number of
      * bytes allowed is 2953. The smallest possible QR Code version is automatically chosen for the output.
      * The ECC level of the result may be higher than the ecl argument if it can be done without increasing the version.
      */
      void encode(const ui8vector &data, const ECL &ecl, int mask = -1);

      /* 
      * Returns a QR Code symbol representing the specified data segments with the specified encoding parameters.
      * The smallest possible QR Code version within the specified range is automatically chosen for the output.
      * This function allows the user to create a custom sequence of segments that switches
      * between modes (such as alphanumeric and binary) to encode text more efficiently.
      * This function is considered to be lower level than simply encoding text or binary data.
      */
      void encode(const std::vector<QRSegment> &segs, const ECL &ecl, int mask = -1);

      void writeToBMP(const std::string &filename);
      void writeToPNG(const std::string &filename);
      void writeToJPEG(const std::string &filename);

    private:
      /* 
      * Returns a QR Code symbol representing the specified data segments with the specified encoding parameters.
      * The smallest possible QR Code version within the specified range is automatically chosen for the output.
      * This function allows the user to create a custom sequence of segments that switches
      * between modes (such as alphanumeric and binary) to encode text more efficiently.
      * This function is considered to be lower level than simply encoding text or binary data.
      */
      //void encodeSegments(const std::vector<QRSegment> &segs, const ECL &ecl, int mask=-1, bool boostEcl=true);  // All optional parameters

    private:
      // Returns a set of positions of the alignment patterns in ascending order. These positions are
      // used on both the x and y axes. Each value in the resulting array is in the range [0, 177).
      // This stateless pure function could be implemented as table of 40 variable-length lists of unsigned bytes.
      std::vector<int> getAlignmentPatternPositions(int version);

      // Returns the number of raw data modules (bits) available at the given version number.
      // These data modules are used for both user data codewords and error correction codewords.
      // This stateless pure function could be implemented as a 40-entry lookup table.
      int getRawDataModulesCount(int version);

      // Returns the number of 8-bit data (i.e. not error correction) codewords contained in any
      // QR Code of the given version number and error correction level, with remainder bits discarded.
      // This stateless pure function could be implemented as a (40*4)-cell lookup table.
      int getDataCodewordsCount(int version, const ECL &ecl);

      // Sets the color of a module and marks it as a function module.
      // Only used by the constructor. Coordinates must be in range.
      void setFunctionModule(int x, int y, bool isBlack);

      // Draws a 9*9 finder pattern including the border separator, with the center module at (x, y).
      void drawFinderPattern(int x, int y);

      // Draws a 5*5 alignment pattern, with the center module at (x, y).
      void drawAlignmentPattern(int x, int y);

      // Draws two copies of the format bits (with its own error correction code)
      // based on the given mask and this object's error correction level field.
      void drawFormatBits(int mask);

      // Draws two copies of the version bits (with its own error correction code),
      // based on this object's version field (which only has an effect for 7 <= version <= 40).
      void drawVersion();

      // XORs the data modules in this QR Code with the given mask pattern. Due to XOR's mathematical
      // properties, calling applyMask(m) twice with the same value is equivalent to no change at all.
      // This means it is possible to apply a mask, undo it, and try another mask. Note that a final
      // well-formed QR Code symbol needs exactly one mask applied (not zero, not two, etc.).
      void applyMask(int mask);

      // Calculates and returns the penalty score based on state of this QR Code's current modules.
      // This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
      int getPenaltyScore() const;

      void drawFunctionPatterns();

      // Returns a new byte string representing the given data with the appropriate error correction
      // codewords appended to it, based on this object's version and error correction level.
      ui8vector appendErrorCorrection(const ui8vector &data);

      // Draws the given sequence of 8-bit codewords (data and error correction) onto the entire
      // data area of this QR Code symbol. Function modules need to be marked off before this is called.
      void drawCodewords(const ui8vector &data);

      // A messy helper function for the constructors. This QR Code must be in an unmasked state when this
      // method is called. The given argument is the requested mask, which is -1 for auto or 0 to 7 for fixed.
      // This method applies and returns the actual mask chosen, from 0 to 7.
      int handleConstructorMasking(int mask);

      void makeQRCode(int version, const ECL &ecl, const ui8vector &dataCodewords, int mask);
      void makeQRCode(const QRCode &qr, int mask);

    private:
      int m_version;    ///< Define version number for This QR Code symbol, which is always between 1 and 40 (inclusive).
      int m_size;       ///< Define the width and height of this QR Code symbol, measured in modules.
                        ///< Always equal to (version * 4 + 17), in the range 21 to 177.
      ECL m_ecl;        ///< Define the error correction level used in this QR Code symbol.
      int m_mask;       ///< Define the mask used in this QR code.

      // Private grids of modules/pixels (conceptually immutable)
      std::vector<std::vector<bool>> m_modules;     ///< Define he modules of this QR Code symbol (false = white, true = black)
      std::vector<std::vector<bool>> m_isFunction;  ///< Define function modules that are not subjected to masking

    private:
      static const int MIN_VERSION;     ///< Define minimum version number for QR Code.
      static const int MAX_VERSION;     ///< Define maximum version number for QR Code.

      // For use in getPenaltyScore(), when evaluating which mask is best.
      static const int PENALTY_N1;
      static const int PENALTY_N2;
      static const int PENALTY_N3;
      static const int PENALTY_N4;

      static const int16_t ERROR_CORRECTION_CODEWORDS[4][41];
      static const int8_t ERROR_CORRECTION_BLOCKS[4][41];
  };
}

#endif    // QRCODE_H
