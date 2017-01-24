/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2016, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     TAppEncTop.cpp
    \brief    Encoder application class
*/

#include <list>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <iomanip>

#include "TAppEncTop.h"


using namespace std;

//! \ingroup TAppEncoder
//! \{

// ====================================================================================================================
// Constructor / destructor / initialization / destroy
// ====================================================================================================================

TAppEncTop::TAppEncTop()
{
  m_iFrameRcvd = 0;
  m_totalBytes = 0;
  m_essentialBytes = 0;
}

TAppEncTop::~TAppEncTop()
{
}

Void TAppEncTop::xCreateLib()
{
  // Video I/O
  m_cTVideoIOYuvInputFile.open( m_inputFileName, false, m_inputBitDepth, m_MSBExtendedBitDepth, m_internalBitDepth );  // read  mode
  m_cTVideoIOYuvInputFile.skipFrames(m_FrameSkip, m_iSourceWidth - m_aiPad[0], m_iSourceHeight - m_aiPad[1], m_InputChromaFormatIDC);

  if (!m_reconFileName.empty())
  {
    m_cTVideoIOYuvReconFile.open(m_reconFileName, true, m_outputBitDepth, m_outputBitDepth, m_internalBitDepth);  // write mode
  }

}

Void TAppEncTop::xDestroyLib()
{
  // Video I/O
  m_cTVideoIOYuvInputFile.close();
  m_cTVideoIOYuvReconFile.close();

}



// ====================================================================================================================
// Public member functions
// ====================================================================================================================

/**
 - create internal class
 - initialize internal variable
 - until the end of input YUV file, call encoding function in TEncTop class
 - delete allocated buffers
 - destroy internal class
 .
 */
Void TAppEncTop::encode()
{

  TComPicYuv*       pcPicYuvOrg = new TComPicYuv;
  TComPicYuv*       pcPicYuvRec = NULL;

  // initialize internal class & member variables
  xCreateLib();
  printChromaFormat();

  // main encoder loop
  Bool  bEos = false;
  Int   iNumEncoded = 0;

  const InputColourSpaceConversion ipCSC  =  m_inputColourSpaceConvert;


  TComPicYuv cPicYuvTrueOrg;


    pcPicYuvOrg->createWithoutCUInfo   ( m_iSourceWidth, m_iSourceHeight, m_chromaFormatIDC, true );
    cPicYuvTrueOrg.createWithoutCUInfo (m_iSourceWidth, m_iSourceHeight, m_chromaFormatIDC, true );

  while ( !bEos )
  {
    // get buffers
    xGetBuffer(pcPicYuvRec);

    // read input YUV file
    m_cTVideoIOYuvInputFile.read( pcPicYuvOrg, &cPicYuvTrueOrg, ipCSC, m_aiPad, m_InputChromaFormatIDC, m_bClipInputVideoToRec709Range );

    // increase number of received frames
    m_iFrameRcvd++;

    bEos = (m_iFrameRcvd == m_framesToBeEncoded) ;

    Bool flush = 0;
    // if end of file (which is only detected on a read failure) flush the encoder of any queued pictures
    if (m_cTVideoIOYuvInputFile.isEof())
    {
      flush = true;
      bEos = true;
      m_iFrameRcvd--;
    }

    // call encoding function for one frame
    // m_cTEncTop.encode( bEos, flush ? 0 : pcPicYuvOrg, flush ? 0 : &cPicYuvTrueOrg, snrCSC, m_cListPicYuvRec, outputAccessUnits, iNumEncoded );

    m_cOperations.grayScale(&cPicYuvTrueOrg);
    iNumEncoded++;


    // temporally skip frames
    if( m_temporalSubsampleRatio > 1 )
    {
      m_cTVideoIOYuvInputFile.skipFrames(m_temporalSubsampleRatio-1, m_iSourceWidth - m_aiPad[0], m_iSourceHeight - m_aiPad[1], m_InputChromaFormatIDC);
    }

    xWriteOutput(&cPicYuvTrueOrg);
  }

  printf("Frames: %d\n", m_iFrameRcvd);
  // delete original YUV buffer
  pcPicYuvOrg->destroy();
  delete pcPicYuvOrg;
  pcPicYuvOrg = NULL;

  // delete used buffers in encoder class
  //m_cTEncTop.deletePicBuffer();
  cPicYuvTrueOrg.destroy();

  // delete buffers & classes
  xDeleteBuffer();
  xDestroyLib();

  printRateSummary();

  return;
}

// ====================================================================================================================
// Protected member functions
// ====================================================================================================================

/**
 - application has picture buffer list with size of GOP
 - picture buffer list acts as ring buffer
 - end of the list has the latest picture
 .
 */
Void TAppEncTop::xGetBuffer( TComPicYuv*& rpcPicYuvRec)
{

    rpcPicYuvRec = new TComPicYuv;

    rpcPicYuvRec->createWithoutCUInfo ( m_iSourceWidth, m_iSourceHeight, m_chromaFormatIDC,  true );

    m_cListPicYuvRec.pushBack( rpcPicYuvRec );
}

Void TAppEncTop::xDeleteBuffer( )
{
  TComList<TComPicYuv*>::iterator iterPicYuvRec  = m_cListPicYuvRec.begin();

  Int iSize = Int( m_cListPicYuvRec.size());

  for ( Int i = 0; i < iSize; i++ )
  {
    TComPicYuv*  pcPicYuvRec  = *(iterPicYuvRec++);
    pcPicYuvRec->destroy();
    delete pcPicYuvRec; pcPicYuvRec = NULL;
  }

}

/** 
  Write access units to output file.
  \param bitstreamFile  target bitstream file
  \param iNumEncoded    number of encoded frames
  \param accessUnits    list of access units to be written
 */


Void TAppEncTop::printRateSummary()
{
  Double time = (Double) m_iFrameRcvd / m_iFrameRate * m_temporalSubsampleRatio;
  printf("Bytes written to file: %u (%.3f kbps)\n", m_totalBytes, 0.008 * m_totalBytes / time);
  if (m_summaryVerboseness > 0)
  {
    printf("Bytes for SPS/PPS/Slice (Incl. Annex B): %u (%.3f kbps)\n", m_essentialBytes, 0.008 * m_essentialBytes / time);
  }
}

Void TAppEncTop::printChromaFormat()
{
  std::cout << std::setw(43) << "Input ChromaFormatIDC = ";
  switch (m_InputChromaFormatIDC)
  {
  case CHROMA_400:  std::cout << "  4:0:0"; break;
  case CHROMA_420:  std::cout << "  4:2:0"; break;
  case CHROMA_422:  std::cout << "  4:2:2"; break;
  case CHROMA_444:  std::cout << "  4:4:4"; break;
  default:
    std::cerr << "Invalid";
    exit(1);
  }
  std::cout << std::endl;

  std::cout << std::setw(43) << "Output (internal) ChromaFormatIDC = ";
  
  std::cout << "\n" << std::endl;
}

Void TAppEncTop::xWriteOutput(TComPicYuv* pcPicYuvRec)
{
  const InputColourSpaceConversion ipCSC = (!m_outputInternalColourSpace) ? m_inputColourSpaceConvert : IPCOLOURSPACE_UNCHANGED;

  if (!m_reconFileName.empty())
      {
        m_cTVideoIOYuvReconFile.write(pcPicYuvRec, ipCSC, 0, 0, 0, 0,
            NUM_CHROMA_FORMAT, m_bClipOutputVideoToRec709Range);
      }


}



//! \}
