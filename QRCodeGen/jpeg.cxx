#include "jpeg.h"

using namespace JPEG;

Jpeg::Jpeg()
  :m_app0(),
  m_sof0(),
  m_dqt(),
  m_dht(),
  m_sos0(),
  m_bytenew(0),
  m_bytepos(7),
  p_file(NULL),
  p_rgb(NULL),
  p_categoryAlloc(NULL),
  p_category(NULL),
  p_bitAlloc(NULL),
  p_bit(NULL)
{
}

Jpeg::Jpeg(const WORD height, const WORD width)
  :m_app0(),
  m_sof0(),
  m_dqt(),
  m_dht(),
  m_sos0(),
  m_bytenew(0),
  m_bytepos(7),
  p_file(NULL),
  p_rgb(NULL),
  p_categoryAlloc(NULL),
  p_category(NULL),
  p_bitAlloc(NULL),
  p_bit(NULL)
{
  m_sof0.m_height = height;
  m_sof0.m_width = width;
}

Jpeg::Jpeg(const Jpeg &other)
  :m_app0(other.m_app0),
  m_sof0(other.m_sof0),
  m_dqt(other.m_dqt),
  m_dht(other.m_dht),
  m_sos0(other.m_sos0),
  m_bytenew(other.m_bytenew),
  m_bytepos(other.m_bytepos),
  p_file(other.p_file),
  p_rgb(other.p_rgb),
  p_categoryAlloc(other.p_categoryAlloc),
  p_category(other.p_category),
  p_bitAlloc(other.p_bitAlloc),
  p_bit(other.p_bit)
{
}

Jpeg::~Jpeg()
{
  if(p_file != NULL)
    fclose(p_file);

  if(p_rgb != NULL)
  {
    delete(p_rgb);
    p_rgb = NULL;
  }

  if(p_categoryAlloc != NULL)
    free(p_categoryAlloc);

  if(p_bitAlloc != NULL)
    free(p_bitAlloc);
}

// Assigment Operator
Jpeg& Jpeg::operator=(const Jpeg &other)
{
  if(this != &other)
  {
    m_app0 = other.m_app0;
    m_sof0 = other.m_sof0;
    m_bytenew = other.m_bytenew;
    m_bytepos = m_bytepos;
    m_dqt = other.m_dqt;
    m_dht = other.m_dht;
    m_sos0 = other.m_sos0;
  }

  return(*this);
}

void Jpeg::setHeight(const WORD height)
{
  m_sof0.m_height = height;
}

void Jpeg::setWidth(const WORD width)
{
  m_sof0.m_width = width;
}

void Jpeg::setJPEGPixel(int row, int col, int red, int green, int blue)
{
  if((m_sof0.m_height > 0) && (m_sof0.m_width > 0))
  {
    if(p_rgb == NULL)
      p_rgb = new RGB[3 * m_sof0.m_height * m_sof0.m_width];

    int i = row * m_sof0.m_height + col;
    p_rgb[i].red = (BYTE)red;
    p_rgb[i].green = (BYTE)green;
    p_rgb[i].blue = (BYTE)blue;
  }
}

void Jpeg::writeToFile(const char *filename)
{
  bitstring fillbits; //filling bitstring for the bit alignment of the EOI marker

  if((m_sof0.m_height > 0) && (m_sof0.m_width > 0))
  {
    fopen_s(&p_file, filename,"wb");
    if(p_file != NULL)
    {

      initComponents();

      /// 
      writeword(0xFFD8); //SOI
      
      /// write markers
      writeAPP();
      writeDQT();
      writeSOF();
      writeDHT();
      writeSOS();

      m_bytenew=0;
      m_bytepos=7;

      /// encode all image pixels
      encodePixels();

      //Do the bit alignment of the EOI marker
      if (m_bytepos>=0) 
      {
        fillbits.length = m_bytepos+1;
        fillbits.value=(1<<(m_bytepos+1))-1;
        writeBits(fillbits);
      }

      writeword(0xFFD9); //EOI

      fclose(p_file);
    }
  }
}

void Jpeg::initComponents()
{
  initHoffmanTable();
  initCategoryAndBit();
}

void Jpeg::initHoffmanTable()
{
 m_htY.computeHuffmanTable(std_dc_luminance_nrcodes, std_dc_luminance_values,
                            std_ac_luminance_nrcodes, std_ac_luminance_values);
 m_htCb.computeHuffmanTable(std_dc_chrominance_nrcodes,std_dc_chrominance_values,
                            std_ac_chrominance_nrcodes,std_ac_chrominance_values);
}

void Jpeg::initCategoryAndBit()
{
  SDWORD nr;
  SDWORD nrlower,nrupper;
  BYTE cat;

  p_categoryAlloc = (BYTE *)malloc(65535 * sizeof(BYTE));
  if (p_categoryAlloc != NULL)
  {
    //exitmessage("Not enough memory.");
    p_category = p_categoryAlloc + 32767; //allow negative subscripts

    p_bitAlloc=(bitstring *)malloc(65535*sizeof(bitstring));
    if (p_bitAlloc != NULL)
    {
      //exitmessage("Not enough memory.");
      p_bit = p_bitAlloc + 32767;

      nrlower=1;
      nrupper=2;
      for (cat = 1; cat <= 15; cat++)
      {
        //Positive numbers
        for (nr = nrlower; nr < nrupper; nr++)
        { 
          p_category[nr]=cat;
          p_bit[nr].length=cat;
          p_bit[nr].value=(WORD)nr;
        }

        //Negative numbers
        for (nr = -(nrupper-1); nr <= -nrlower; nr++)
        {
          p_category[nr]=cat;
          p_bit[nr].length=cat;
          p_bit[nr].value=(WORD)(nrupper-1+nr);
        }

        nrlower<<=1;
        nrupper<<=1;
      }
    }
  }
}

void Jpeg::encodePixels()
{
  SWORD DCY=0,DCCb=0,DCCr=0; //DC coefficients used for differential encoding
  WORD xpos,ypos;
  for (ypos = 0; ypos < m_sof0.m_height; ypos += 8)
    for (xpos = 0; xpos < m_sof0.m_width; xpos += 8)
    {
      loadDUFromRGBPixels(xpos,ypos);

      writeDU(m_Y, m_dqt.m_fdctTableY, &DCY, m_htY);
      writeDU(m_Cb, m_dqt.m_fdctTableCb, &DCCb, m_htCb);
      writeDU(m_Cr, m_dqt.m_fdctTableCb, &DCCr, m_htCb);
    }
}

void Jpeg::loadDUFromRGBPixels(const WORD xpos, const WORD ypos)
{
  BYTE x,y;
  BYTE pos=0;
  DWORD location;
  BYTE R,G,B;

  location = ypos * m_sof0.m_width + xpos;
  for (y=0;y<8;y++)
  {
    for (x=0;x<8;x++)
    {
      R = p_rgb[location].red;
      G = p_rgb[location].green;
      B = p_rgb[location].blue;

      m_Y[pos]  = m_ycbcr.Y(R,G,B);
      m_Cb[pos] = m_ycbcr.Cb(R,G,B);
      m_Cr[pos] = m_ycbcr.Cr(R,G,B);

      location++;
      pos++;
    }

    location += m_sof0.m_width - 8;
  }
}

void Jpeg::writeDU(const SBYTE *ComponentDU, const float *fdtbl, SWORD *DC, const HT &ht)
{
  BYTE i;
  BYTE startpos;
  BYTE end0pos;
  BYTE nrzeroes;
  BYTE nrmarker;

  SWORD DU_DCT[64];   // Current DU (after DCT and quantization) which we'll zigzag
  SWORD DU[64];       //zigzag reordered DU which will be Huffman coded
  SWORD Diff;

  bitstring EOB = ht.m_ac[0x00];
  bitstring M16zeroes = ht.m_ac[0xF0];

  calculateFDCTAndQuantization(ComponentDU, fdtbl, DU_DCT);

  //zigzag reorder
  for (i=0;i<=63;i++)
    DU[zigzag[i]] = DU_DCT[i];

  Diff=DU[0]-*DC;
  *DC=DU[0];

  //Encode DC
  if(Diff==0)
    writeBits(ht.m_dc[0]); //Diff might be 0
  else
  {
    writeBits(ht.m_dc[p_category[Diff]]);
    writeBits(p_bit[Diff]);
  }

  //Encode ACs
  for (end0pos=63; (end0pos>0)&&(DU[end0pos]==0); end0pos--) ;

  //end0pos = first element in reverse order !=0
  if (end0pos==0)
  {
    writeBits(EOB);
    return;
  }

  i=1;
  while (i<=end0pos)
  {
    startpos=i;
    for (; (DU[i]==0)&&(i<=end0pos);i++) ;

    nrzeroes=i-startpos;
    if (nrzeroes>=16) 
    {
      for (nrmarker=1;nrmarker<=nrzeroes/16;nrmarker++)
        writeBits(M16zeroes);

      nrzeroes=nrzeroes%16;
    }

    writeBits(ht.m_ac[nrzeroes*16 + p_category[DU[i]]]);
    writeBits(p_bit[DU[i]]);

    i++;
  }

  if (end0pos!=63) writeBits(EOB);
}

void Jpeg::calculateFDCTAndQuantization(const SBYTE *data, const float *fdtbl, SWORD *outdata)
{
  float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
  float tmp10, tmp11, tmp12, tmp13;
  float z1, z2, z3, z4, z5, z11, z13;
  float *dataptr;
  float datafloat[64];
  float temp;
  SBYTE ctr;
  BYTE i;

  for (i=0; i<64; i++)
    datafloat[i]=data[i];

  // Pass 1: process rows.
  dataptr = datafloat;
  for (ctr = 7; ctr >= 0; ctr--) 
  {
    tmp0 = dataptr[0] + dataptr[7];
    tmp7 = dataptr[0] - dataptr[7];
    tmp1 = dataptr[1] + dataptr[6];
    tmp6 = dataptr[1] - dataptr[6];
    tmp2 = dataptr[2] + dataptr[5];
    tmp5 = dataptr[2] - dataptr[5];
    tmp3 = dataptr[3] + dataptr[4];
    tmp4 = dataptr[3] - dataptr[4];

    // Even part
    tmp10 = tmp0 + tmp3;	// phase 2
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;

    dataptr[0] = tmp10 + tmp11; // phase 3
    dataptr[4] = tmp10 - tmp11;

    z1 = (tmp12 + tmp13) * ((float) 0.707106781); // c4
    dataptr[2] = tmp13 + z1;	// phase 5
    dataptr[6] = tmp13 - z1;


    // Odd part
    tmp10 = tmp4 + tmp5;	// phase 2
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    // The rotator is modified from fig 4-8 to avoid extra negations
    z5 = (tmp10 - tmp12) * ((float) 0.382683433); // c6
    z2 = ((float) 0.541196100) * tmp10 + z5; // c2-c6
    z4 = ((float) 1.306562965) * tmp12 + z5; // c2+c6
    z3 = tmp11 * ((float) 0.707106781); // c4

    z11 = tmp7 + z3;		// phase 5
    z13 = tmp7 - z3;

    dataptr[5] = z13 + z2;	// phase 6
    dataptr[3] = z13 - z2;
    dataptr[1] = z11 + z4;
    dataptr[7] = z11 - z4;

    dataptr += 8;		//advance pointer to next row
  }

  // Pass 2: process columns

  dataptr = datafloat;
  for (ctr = 7; ctr >= 0; ctr--) 
  {
    tmp0 = dataptr[0] + dataptr[56];
    tmp7 = dataptr[0] - dataptr[56];
    tmp1 = dataptr[8] + dataptr[48];
    tmp6 = dataptr[8] - dataptr[48];
    tmp2 = dataptr[16] + dataptr[40];
    tmp5 = dataptr[16] - dataptr[40];
    tmp3 = dataptr[24] + dataptr[32];
    tmp4 = dataptr[24] - dataptr[32];

    //Even part/

    tmp10 = tmp0 + tmp3;	//phase 2
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;

    dataptr[0] = tmp10 + tmp11; // phase 3
    dataptr[32] = tmp10 - tmp11;

    z1 = (tmp12 + tmp13) * ((float) 0.707106781); // c4
    dataptr[16] = tmp13 + z1; // phase 5
    dataptr[48] = tmp13 - z1;

    // Odd part

    tmp10 = tmp4 + tmp5;	// phase 2
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    // The rotator is modified from fig 4-8 to avoid extra negations.
    z5 = (tmp10 - tmp12) * ((float) 0.382683433); // c6
    z2 = ((float) 0.541196100) * tmp10 + z5; // c2-c6
    z4 = ((float) 1.306562965) * tmp12 + z5; // c2+c6
    z3 = tmp11 * ((float) 0.707106781); // c4

    z11 = tmp7 + z3;		// phase 5
    z13 = tmp7 - z3;

    dataptr[40] = z13 + z2; // phase 6
    dataptr[24] = z13 - z2;
    dataptr[8] = z11 + z4;
    dataptr[56] = z11 - z4;

    dataptr++;			// advance pointer to next column
  }

  // Quantize/descale the coefficients, and store into output array
  for (i = 0; i < 64; i++) 
  {
    // Apply the quantization and scaling factor
    temp = datafloat[i] * fdtbl[i];

    //Round to nearest integer.
    //Since C does not specify the direction of rounding for negative
    //quotients, we have to force the dividend positive for portability.
    //The maximum coefficient size is +-16K (for 12-bit data), so this
    //code should work for either 16-bit or 32-bit ints.

    outdata[i] = (SWORD) ((SWORD)(temp + 16384.5) - 16384);
  }
}

/// Private methods
void Jpeg::writebyte(const char b)
{
  if(p_file != NULL)
    fputc(b, p_file);
}

void Jpeg::writeword(const WORD w)
{
  writebyte(w / 256);
  writebyte(w % 256);
}

void Jpeg::writeAPP()
{
  writeword(m_app0.m_marker);
  writeword(m_app0.m_len);

  writebyte('J');writebyte('F');writebyte('I');writebyte('F');writebyte(0);

  writebyte(m_app0.m_majorVersion);
  writebyte(m_app0.m_minorVersion);

  writebyte(m_app0.m_unit);

  writeword(m_app0.m_xDensity);
  writeword(m_app0.m_yDensity);

  writebyte(m_app0.m_thumbnWidth);
  writebyte(m_app0.m_thumbnHeight);
}

void Jpeg::writeSOF()
{
  writeword(m_sof0.m_marker);
  writeword(m_sof0.m_len);
  writebyte(m_sof0.m_precision);
  writeword(m_sof0.m_height);
  writeword(m_sof0.m_width);
  writebyte(m_sof0.m_components);

  writebyte(m_sof0.m_compY.m_id);
  writebyte(m_sof0.m_compY.m_samplingFactor);
  writebyte(m_sof0.m_compY.m_quantizationTable);

  writebyte(m_sof0.m_compCb.m_id);
  writebyte(m_sof0.m_compCb.m_samplingFactor);
  writebyte(m_sof0.m_compCb.m_quantizationTable);

  writebyte(m_sof0.m_compCr.m_id);
  writebyte(m_sof0.m_compCr.m_samplingFactor);
  writebyte(m_sof0.m_compCr.m_quantizationTable);
}

void Jpeg::writeDQT()
{
  writeword(m_dqt.m_marker);
  writeword(m_dqt.m_len);

  writebyte(m_dqt.m_infoY);
  for (int i = 0; i < 64; i++)
    writebyte(m_dqt.m_tableY[i]);

  writebyte(m_dqt.m_infoCb);
  for (int i = 0; i < 64; i++)
    writebyte(m_dqt.m_tableCb[i]);
}

void Jpeg::writeDHT()
{
  writeword(m_dht.m_marker);
  writeword(m_dht.m_len);

  writeHTComponent(m_dht.m_YDC);
  writeHTComponent(m_dht.m_YAC);
  writeHTComponent(m_dht.m_CbDC);
  writeHTComponent(m_dht.m_CbAC);
}

void Jpeg::writeHTComponent(const DHTComponent &htc)
{
  writebyte(htc.m_index);
  for (int i = 0; i < htc.m_nrcodeSize; i++)
    writebyte(htc.m_nrcodes[i]);
  for (int i = 0; i < htc.m_valuesSize; i++)
    writebyte(htc.m_values[i]);
}

void Jpeg::writeSOS()
{
  writeword(m_sos0.m_marker);
  writeword(m_sos0.m_len);

  writebyte(m_sos0.m_components);
  writebyte(m_sos0.m_idY);
  writebyte(m_sos0.m_HTY);

  writebyte(m_sos0.m_idCb);
  writebyte(m_sos0.m_HTCb);

  writebyte(m_sos0.m_idCr);
  writebyte(m_sos0.m_HTCr);

  writebyte(m_sos0.m_Ss);
  writebyte(m_sos0.m_Se);
  writebyte(m_sos0.m_Bf);
}

void Jpeg::writeComment(BYTE *comment)
{
  int length=(WORD)strlen((const char *)comment);

  writeword(0xFFFE); //The COM marker
  writeword(length+2);
  for (int i = 0; i < length; i++)
    writebyte(comment[i]);
}

void Jpeg::writeBits(const bitstring &bs)
{
  WORD value;
  SBYTE posval;    //bit position in the bitstring we read, should be<=15 and >=0
  const WORD mask[16] = {
                      1,2,4,8,
                      16,32,64,128,
                      256,512,1024,2048,
                      4096,8192,16384,32768
                    };

  value=bs.value;
  posval=bs.length-1;
  while (posval>=0)
  {
    if (value & mask[posval]) 
      m_bytenew |= mask[m_bytepos];

    posval--; m_bytepos--;
    if (m_bytepos < 0) 
    {
      if (m_bytenew == 0xFF)
      {
        writebyte(0xFF);
        writebyte(0);
      }
      else
        writebyte(m_bytenew);

      m_bytepos=7;
      m_bytenew=0;
    }
  }
}
