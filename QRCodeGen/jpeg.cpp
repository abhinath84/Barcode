#include "jpeg.h"

using namespace JPEG;

Jpeg::Jpeg()
  :m_app0(),
  m_sof0(),
  m_dqt(),
  m_dht(),
  m_sos0(),
  p_rgb(NULL),
  p_file(NULL)
{
}

Jpeg::Jpeg(const WORD height, const WORD width)
  :m_app0(),
  m_sof0(),
  m_dqt(),
  m_dht(),
  m_sos0(),
  p_rgb(NULL),
  p_file(NULL)
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
  p_rgb(other.p_rgb),
  p_file(other.p_file)
{
}

Jpeg::~Jpeg()
{
  if(p_file != NULL)
    fclose(p_file);

  if(p_rgb != NULL)
    delete(p_rgb);
}

// Assigment Operator
Jpeg& Jpeg::operator=(const Jpeg &other)
{
  if(this != &other)
  {
    m_app0 = other.m_app0;
    m_sof0 = other.m_sof0;
    m_dqt = other.m_dqt;
    m_dht = other.m_dht;
    m_sos0 = other.m_sos0;
  }

  return(*this);
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
  //WORD value;
  //SBYTE posval;    //bit position in the bitstring we read, should be<=15 and >=0
  //const WORD mask[16] = {
  //                    1,2,4,8,
  //                    16,32,64,128,
  //                    256,512,1024,2048,
  //                    4096,8192,16384,32768
  //                  };

  //value=bs.value;
  //posval=bs.length-1;
  //while (posval>=0)
  //{
  //  if (value & mask[posval]) 
  //    bytenew|=mask[bytepos];

  //  posval--; bytepos--;
  //  if (bytepos<0) 
  //  {
  //    if (bytenew==0xFF)
  //    {
  //      writebyte(0xFF);
  //      writebyte(0);
  //    }
  //    else
  //      writebyte(bytenew);

  //    bytepos=7;
  //    bytenew=0;
  //  }
  //}
}
