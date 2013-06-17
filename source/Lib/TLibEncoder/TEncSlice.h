/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2013, ITU/ISO/IEC
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

/** \file     TEncSlice.h
    \brief    slice encoder class (header)
*/

#ifndef __TENCSLICE__
#define __TENCSLICE__

// Include files
#include "TLibCommon/CommonDef.h"
#include "TLibCommon/TComList.h"
#include "TLibCommon/TComPic.h"
#include "TLibCommon/TComPicYuv.h"
#include "TEncCu.h"
#include "WeightPredAnalysis.h"
#include "TEncRateCtrl.h"

//! \ingroup TLibEncoder
//! \{

class TEncTop;
class TEncGOP;

namespace x265 { class EncodeFrame; }

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// slice encoder class
class TEncSlice
    : public WeightPredAnalysis
{
private:

    // encoder configuration
    TEncCfg*                m_pcCfg;                            ///< encoder configuration class
    UInt                    m_uiSliceIdx;
    std::vector<TEncSbac*>  CTXMem;

public:

    TEncSlice();
    virtual ~TEncSlice();

    Void    create(Int iWidth, Int iHeight, UInt iMaxCUWidth, UInt iMaxCUHeight, UChar uhTotalDepth);
    Void    destroy();
    Void    init(TEncTop* pcEncTop);

    /// preparation of slice encoding (reference marking, QP and lambda)
    TComSlice *initEncSlice(TComPic* pcPic, x265::EncodeFrame *pcEncodeFrame, Bool bForceISlice, Int pocLast, Int pocCurr, Int iGOPid, TComSPS* pSPS, TComPPS *pPPS);

    Void    resetQP(TComPic* pic, x265::EncodeFrame *pcEncodeFrame, Int sliceQP, Double lambda);

    // compress and encode slice
    Void    compressSlice(TComPic* rpcPic, x265::EncodeFrame* pcEncodeFrame);        ///< analysis stage of slice

    Void    encodeSlice(TComPic*& rpcPic, TComOutputBitstream* pcSubstreams, x265::EncodeFrame* pcEncodeFrame);

    // misc. functions
    Void    setSearchRange(TComSlice* pcSlice, x265::EncodeFrame *pcEncodeframe);    ///< set ME range adaptively

    Void    xDetermineStartAndBoundingCUAddr(TComPic* rpcPic, Bool bEncodeSlice);

    UInt    getSliceIdx()         { return m_uiSliceIdx; }

    Void    setSliceIdx(UInt i)   { m_uiSliceIdx = i; }

    Void    initCtxMem(UInt i);
    Void    setCtxMem(TEncSbac* sb, Int b)   { CTXMem[b] = sb; }

private:

    Double  xGetQPValueAccordingToLambda(Double lambda);
};

//! \}

#endif // __TENCSLICE__
