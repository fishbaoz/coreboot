/* $NoKeywords:$ */
/**
 * @file
 *
 * Graphics controller BIF straps control services.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */
/*
*****************************************************************************
*
* Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
* AMD is granting you permission to use this software (the Materials)
* pursuant to the terms and conditions of your Software License Agreement
* with AMD.  This header does *NOT* give you permission to use the Materials
* or any rights under AMD's intellectual property.  Your use of any portion
* of these Materials shall constitute your acceptance of those terms and
* conditions.  If you do not agree to the terms and conditions of the Software
* License Agreement, please do not use any portion of these Materials.
*
* CONFIDENTIALITY:  The Materials and all other information, identified as
* confidential and provided to you by AMD shall be kept confidential in
* accordance with the terms and conditions of the Software License Agreement.
*
* LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
* PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
* OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
* IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
* (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
* INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
* GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
* RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
* EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
* THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
*
* AMD does not assume any responsibility for any errors which may appear in
* the Materials or any other related information provided to you by AMD, or
* result from use of the Materials or any related information.
*
* You agree that you will not reverse engineer or decompile the Materials.
*
* NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
* further information, software, technical information, know-how, or show-how
* available to you.  Additionally, AMD retains the right to modify the
* Materials at any time, without notice, and is not obligated to provide such
* modified Materials to you.
*
* U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
* "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
* subject to the restrictions as set forth in FAR 52.227-14 and
* DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
* Government constitutes acknowledgement of AMD's proprietary rights in them.
*
* EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
* direct product thereof will be exported directly or indirectly, into any
* country prohibited by the United States Export Administration Act and the
* regulations thereunder, without the required authorization from the U.S.
* government nor will be used for any purpose prohibited by the same.
* ***************************************************************************
*
*/


#ifndef _GNBFUSETABLE_H_
#define _GNBFUSETABLE_H_

#pragma pack (push, 1)

/// Fuse definition structure
typedef struct  {
  UINT8                       PPlayTableRev;              ///< PP table revision
  UINT8                       VclkDid[8];                 ///< VCLK DID - entended to 8
  UINT8                       DclkDid[8];                 ///< DCLK DID - entended to 8
  UINT8                       SclkThermDid;               ///< *Thermal SCLK - removed on CZ
  UINT8                       VclkDclkSel[6];             ///< *Vclk/Dclk selector - removed on CZ
  UINT8                       LclkDid[8];                 ///< Lclk DPM DID - entended to 8
  UINT8                       DispClkDid[8];              ///< DispClk DID - entended to 8
  UINT8                       PcieGen2Vid;                ///< Pcie Gen 2 VID
  UINT8                       MainPllId;                  ///< *Main PLL Id from fuses - removed on CZ
  UINT8                       WrCkDid;                    ///< *WRCK SMU clock Divisor - removed on CZ
  UINT8                       GpuBoostCap;                ///< *GPU boost cap - removed on CZ
  UINT32                      SclkDpmBoostMargin;         ///< *Boost margin - removed on CZ
  UINT32                      SclkDpmThrottleMargin;      ///< *Throttle margin - removed on CZ
  BOOLEAN                     VceStateTableSupport;       ///< Support VCE in PP table
  UINT8                       VceFlags[8];                ///< VCE Flags - entended to 8
  UINT8                       VceMclk;                    ///< MCLK for VCE bit map
  UINT8                       VceMclk4;                   ///< MCLK for VCE bit map -- New added for CZ, entended bit 4
  UINT8                       VceMclk5;                   ///< MCLK for VCE bit map -- New added for CZ, entended bit 5
  UINT8                       VceMclk6;                   ///< MCLK for VCE bit map -- New added for CZ, entended bit 6
  UINT8                       VceMclk7;                   ///< MCLK for VCE bit map -- New added for CZ, entended bit 7
  UINT8                       VceReqSclkSel[8];           ///< SCLK selector for VCE - - entended to 8
  UINT8                       EclkDid[8];                 ///< Eclk DID - entended to 8
  UINT8                       HtcEn;                      ///< HTC Enabled
  UINT8                       HtcTmpLmt;                  ///< HTC Temperature Limit
  UINT8                       HtcHystLmt;                 ///< HTC Hysteresis Limit
  UINT8                       MainPllOpFreqIdStartup;     ///< GCK_PLL_FUSES
  UINT8                       DisDllShutdownSR[4];        ///< DRAM Configuration Low [per DCT]
  UINT8                       MemClkFreq[4];              ///< DRAM Configuration High [per DCT]
  UINT8                       MemPhyPllPdMode[4];         ///< *DRAM Controller Miscellaneous 2 [per DCT] - removed on CZ
  UINT8                       M1MemClkFreq[4];            ///< Memory PState Control and Status [per DCT]
  UINT8                       NbPstateEn[4];              ///< NB PState Enable
  UINT8                       NbVid_6_0[4];               ///< NB VID [6:0]
  UINT8                       NbVid_7[4];                 ///< NB VID [7]
  UINT8                       MemPstate[4];               ///< Memory P State
  UINT8                       NbFid[4];                   ///< NB FID
  UINT8                       NbDid[4];                   ///< NB DID
  UINT8                       VddNbVid[8];                ///< Sclk VID - entended to 8
  UINT8                       SclkDid[8];                 ///< Sclk DID - entended to 8
  UINT8                       SamClkDid[5];               ///< *SamClk DID - removed on CZ
  UINT8                       VddNbVidOffset[8];          ///< VddNbVid offset - entended to 8
  UINT8                       NumBoostStates;             ///< NumBoostStates
  UINT8                       PerfFlag;                   ///< PerfFlag bits
  UINT8                       PerfFlag5;                  ///< PerfFlag bit -- New added for CZ, entended bit 5
  UINT8                       PerfFlag6;                  ///< PerfFlag bit -- New added for CZ, entended bit 6
  UINT8                       PerfFlag7;                  ///< PerfFlag bit -- New added for CZ, entended bit 7
  UINT8                       PowerSavingsFlag;           ///< PS Flag bits
  UINT8                       PowerSavingsFlag5;          ///< PS Flag bits -- New added for CZ, entended bit 5
  UINT8                       PowerSavingsFlag6;          ///< PS Flag bits -- New added for CZ, entended bit 6
  UINT8                       PowerSavingsFlag7;          ///< PS Flag bits -- New added for CZ, entended bit 7
  UINT8                       AcpClkDid[8];               ///< Acp Clk DID - entended to 8
  UINT8                       PcieGen3Vid;                ///< Pcie Gen 3 VID
  UINT8                       GfxVid[8];                  ///< Gfx VID - New added for CZ, entended to 8
  UINT8                       GfxVidOffset[8];            ///< GfxVidOffset - New added for CZ, entended to 8
} PP_FUSE_ARRAY_V2;

#pragma pack (pop)

#endif

