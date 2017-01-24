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

/** \file     TAppEncTop.h
    \brief    Encoder application class (header)
*/

#ifndef __TAPPENCTOP__
#define __TAPPENCTOP__

#include <list>
#include <ostream>

#include "TVideoIOYuv.h"
#include "TAppEncCfg.h"
#include "TComList.h"
#include "Operations.h"

//! \ingroup TAppEncoder
//! \{

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// encoder application class
class TAppEncTop : public TAppEncCfg
{
private:
  // class interface
  TVideoIOYuv                m_cTVideoIOYuvInputFile;       ///< input YUV file
  TVideoIOYuv                m_cTVideoIOYuvReconFile;       ///< output reconstruction file

  TComList<TComPicYuv*>      m_cListPicYuvRec;              ///< list of reconstruction YUV files
  Operations 				 m_cOperations;

  Int                        m_iFrameRcvd;                  ///< number of received frames

  UInt m_essentialBytes;
  UInt m_totalBytes;

protected:
  // initialization
  Void  xDestroyLib       ();                               ///< destroy encoder class

  /// obtain required buffers
  Void xGetBuffer(TComPicYuv*& rpcPicYuvRec);

  /// delete allocated buffers
  Void  xDeleteBuffer     ();

  // file I/O
  Void printRateSummary();
  Void printChromaFormat();
  Void xWriteOutput(TComPicYuv* pcPicYuvRec);
  Void sobelOperator(TComPicYuv* pcPicYuvRec, TComPicYuv pcPicYuvOrg, Treshold Treshold);

public:
  TAppEncTop();
  virtual ~TAppEncTop();

  Void  xCreateLib        ();                               ///< create files & encoder class

  Void        encode      ();                               ///< main encoding function

};// END CLASS DEFINITION TAppEncTop

//! \}

#endif // __TAPPENCTOP__

