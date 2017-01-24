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

/** \file     TAppEncCfg.h
    \brief    Handle encoder configuration parameters (header)
*/

#ifndef __TAPPENCCFG__
#define __TAPPENCCFG__

#include "CommonDef.h"

//#include "TLibEncoder/TEncCfg.h"
#include <sstream>
#include <vector>
//! \ingroup TAppEncoder
//! \{

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// encoder configuration class
class TAppEncCfg
{
protected:
  // file I/O
  std::string m_inputFileName;                                ///< source file name
  std::string m_reconFileName;                                ///< output reconstruction file
  Int       warnUnknowParameter;

  // Lambda modifiers
  Int       m_iFrameRate;                                     ///< source frame-rates (Hz)
  UInt      m_FrameSkip;                                      ///< number of skipped frames from the beginning
  UInt      m_temporalSubsampleRatio;                         ///< temporal subsample ratio, 2 means code every two frames
  Int       m_iSourceWidth;                                   ///< source width in pixel
  Int       m_iSourceHeight;                                  ///< source height in pixel (when interlaced = field height)

  Int       m_iSourceHeightOrg;                               ///< original source height in pixel (when interlaced = frame height)

  Int       m_framesToBeEncoded;                              ///< number of encoded frames
  Int       m_aiPad[2];                                       ///< number of padded pixels for width and height
                          
  InputColourSpaceConversion m_inputColourSpaceConvert;       ///< colour space conversion to apply to input video
  Bool      m_snrInternalColourSpace;                       ///< if true, then no colour space conversion is applied for snr calculation, otherwise inverse of input is applied.
  Bool      m_outputInternalColourSpace;                    ///< if true, then no colour space conversion is applied for reconstructed video, otherwise inverse of input is applied.
  ChromaFormat m_InputChromaFormatIDC;

  Bool      m_bClipInputVideoToRec709Range;
  Bool      m_bClipOutputVideoToRec709Range;

  Int       m_iGOPSize;                                       ///< GOP size of hierarchical structure


  // coding tools (bit-depth)
  Int       m_inputBitDepth   [MAX_NUM_CHANNEL_TYPE];         ///< bit-depth of input file
  Int       m_outputBitDepth  [MAX_NUM_CHANNEL_TYPE];         ///< bit-depth of output file
  Int       m_MSBExtendedBitDepth[MAX_NUM_CHANNEL_TYPE];      ///< bit-depth of input samples after MSB extension
  Int       m_internalBitDepth[MAX_NUM_CHANNEL_TYPE];         ///< bit-depth codec operates at (input/output files will be converted)
  Bool      m_extendedPrecisionProcessingFlag;
  Bool      m_highPrecisionOffsetsEnabledFlag;

  //coding tools (chroma format)
  ChromaFormat m_chromaFormatIDC;

  std::string m_summaryOutFilename;                           ///< filename to use for producing summary output file.
  std::string m_summaryPicFilenameBase;                       ///< Base filename to use for producing summary picture output files. The actual filenames used will have I.txt, P.txt and B.txt appended.
  UInt        m_summaryVerboseness;                           ///< Specifies the level of the verboseness of the text output.

  // internal member functions
  Void  xCheckParameter ();                                   ///< check validity of configuration values
  Void  xPrintParameter ();                                   ///< print configuration values
  Void  xPrintUsage     ();                                   ///< print usage
  
public:
  TAppEncCfg();
  virtual ~TAppEncCfg();

public:
  Void  create    ();                                         ///< create option handling class
  Void  destroy   ();                                         ///< destroy option handling class
  Bool  parseCfg  ( Int argc, TChar* argv[] );                ///< parse configuration file to fill member variables

};// END CLASS DEFINITION TAppEncCfg

//! \}

#endif // __TAPPENCCFG__

