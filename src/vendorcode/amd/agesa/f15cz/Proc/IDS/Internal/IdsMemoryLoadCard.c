/* $NoKeywords:$ */
/**
 * @file
 *
 * IdsMemoryLoadCard.c
 *
 * Contains AMD AGESA Loadcard functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project: (Proc/Ids/Internal)
 * @e \$Revision: 311625 $   @e \$Date: 2015-01-26 10:35:21 +0800 (Mon, 26 Jan 2015) $
 */
/*****************************************************************************
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
 ******************************************************************************
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "Ids.h"
#include "IdsLib.h"
#include "IdsInternal.h"
#include "IdsInternalLib.h"
#include "Topology.h"
#include "amdlib.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mnreg.h"
#include "mn.h"
#include "mt.h"
#include "mfStandardTraining.h"
#include "S3.h"
#include "mfs3.h"
#include "cpuRegisters.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuPostInit.h"
#include "heapManager.h"
#include "cpuServices.h"
#include "CommonReturns.h"


///Please check IdsInternalLib.h for internal FILECODE
#define FILECODE PROC_IDS_INTERNAL_IDSMEMORYLOADCARD_FILECODE

#define LC_BUFF_SEG_0   0
#define LC_BUFF_SEG_1   1
#define LC_BUFF_SEG_2   29
#define LC_BUFF_SEG_3   45

/**
 *
 *  AGESA_IDS_NV_LOAD_CARD_MODE[2:0] specifies the slot that load card is (will be) installed
 *    000  Disabled
 *    001  Channel A
 *    010  Nearest slot from processor
 *    011  Other Channels (B, C, D)
 *    100  Farthest slot from processor
 *    101  Reserved
 *    110  Reserved
 *    111  Reserved
 *   1XXX  Reserved for solder-down DIMM
 *
 *  AGESA_IDS_NV_LOAD_CARD_MODE[3] is used to indicate solder-down DRAM.
 *
 *  AGESA_IDS_NV_LOAD_CARD_ACTIVE[0] specifies whether load card is installed
 *    0   No load card in system
 *    1   Some real DIMMs have been replaced with load cards
 *
 **/

VOID
STATIC
IdsIntSelfTrap (
  IN OUT   S3_DATA_BLOCK *S3DataBlock
  );

extern MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[];

/**
 *  IDS Backend Function
 *
 *  This function is used to override settings before S3 Restore
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_S3LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBeforeS3Restore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function
 *
 *  This function is used to override settings before DQS training
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBeforeDramInit (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS    LcMode;
  IDS_STATUS    LcModeActive;
  MEM_NB_BLOCK  *NBPtr;
  UINT32        CSBaseAddrNear0;
  UINT32        CSBaseAddrNear1;
  UINT32        CsSize;
  UINT32        CSBaseAddr;
  UINT32        Value32;
  UINT8         CS;
  UINT8         NearCS;
  UINT8         Dct;
  BIT_FIELD_NAME Reg;

  NBPtr = (MEM_NB_BLOCK *) DataPtr;
  if (NBPtr->Node == 0) {
    IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
      if ((LcMode & 7) == (IDS_STATUS)2) {
        for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
          // Load card will be installed in the nearest DIMM slot.
          // We need to swap the low memory address to far DIMMs.
          NBPtr->SwitchDCT (NBPtr, Dct);
          if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
            CSBaseAddrNear0 = 0;
            CSBaseAddrNear1 = 0;
            CsSize = 0;
            NearCS = 0xFF;
            for (CS = 0; CS < MAX_CS_PER_CHANNEL; CS += 2) {
              CSBaseAddr = NBPtr->GetBitField (NBPtr, BFCSBaseAddr0Reg + CS);
              if ((CSBaseAddr & 1) != 0) {
                if ((CSBaseAddr & 0xFFF80000) == CsSize) {
                  CsSize = NBPtr->GetBitField (NBPtr, BFCSMask0Reg + (CS >> 1));
                  CsSize += ((UINT32)1 << 19);
                  CsSize &= 0xFFF80000;
                  if (NearCS == 0xFF) {
                    // This is the nearest DIMM and has base address of 0.
                    NearCS = CS;
                    CSBaseAddrNear1 = NBPtr->GetBitField (NBPtr, BFCSBaseAddr0Reg + CS + 1);
                    if ((CSBaseAddrNear1 & 1) != 0) {
                      // If dual-rank DIMM
                      CSBaseAddrNear1 = CsSize | (CSBaseAddrNear1 & 0xF);
                      CsSize <<= 1;
                    }
                    CSBaseAddrNear0 = CSBaseAddr & 0xF;
                  } else {
                    // Swap base address of the nearest DIMM with this DIMM
                    NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + CS, CSBaseAddr & 0xF);
                    CSBaseAddr = NBPtr->GetBitField (NBPtr, BFCSBaseAddr0Reg + CS + 1);
                    if ((CSBaseAddr & 1) != 0) {
                      // If dual-rank DIMM
                      NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + CS + 1, CsSize + (CSBaseAddr & 0xF));
                      CsSize <<= 1;
                    }

                    NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + NearCS, CsSize + CSBaseAddrNear0);
                    if ((CSBaseAddrNear1 & 1) != 0) {
                      NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + NearCS + 1, CsSize + CSBaseAddrNear1);
                    }
                    break;
                  }
                }
              }
            }
          }
        }
      } else if ((LcMode & 7) == (IDS_STATUS)1 || (LcMode & 7) == (IDS_STATUS)3) {
        IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
          if (LcModeActive == (IDS_STATUS)1) {
            // Load card is installed, we need to force all DRAM channels to be initialized
            // even if there are no DIMMs on that channel.
            for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
              NBPtr->SwitchDCT (NBPtr, Dct);
              if (NBPtr->DCTPtr->Timings.DctMemSize == 0) {
                // If a channel has no DIMMs, force it to have the same amount of DIMMs as the other channel
                NBPtr->DCTPtr->Timings.DctMemSize = 0x1;
                NBPtr->SetBitField (NBPtr, BFDisDramInterface, 0);
                for (Reg = BFCSBaseAddr0Reg; Reg <= BFDramConfigHiReg; Reg++) {
                  // Copy the other channel's registers to its own.
                  NBPtr->SwitchDCT (NBPtr, Dct ^ 1);
                  Value32 = NBPtr->GetBitField (NBPtr, Reg);
                  NBPtr->SwitchDCT (NBPtr, Dct);
                  NBPtr->SetBitField (NBPtr, Reg, Value32);
                }
              }
            }
          }
        }
      }
    }
  }

  return IDS_SUCCESS;
}


/**
 *  IDS Backend Function
 *
 *  This function is used to override settings before MemClk frequency change
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBeforeMemFreqChg (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS    LcMode;
  IDS_STATUS    LcModeActive;
  MEM_NB_BLOCK  *NBPtr;
  UINT8         Dct;

  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if ((LcMode & 7) == (IDS_STATUS)1 || (LcMode & 7) == (IDS_STATUS)3) {
      IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
        if (LcModeActive == (IDS_STATUS)1) {
          // Load card is installed, we need to force all DRAM channels to be initialized
          // even if there are no DIMMs on that channel.
          NBPtr = (MEM_NB_BLOCK *) DataPtr;
          for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
            NBPtr->SwitchDCT (NBPtr, Dct);
            if (NBPtr->DCTPtr->Timings.DctMemSize == 0) {
              // If a channel has no DIMMs installed, force it to have some memory to do frequency change
              NBPtr->DCTPtr->Timings.DctMemSize = 1;

              NBPtr->BrdcstSet (NBPtr, BFMemClkFreqVal, 0);

              NBPtr->BrdcstSet (NBPtr, BFMemClkFreq, NBPtr->GetMemClkFreqId (NBPtr, NBPtr->DCTPtr->Timings.Speed));

              NBPtr->BrdcstSet (NBPtr, BFMemClkFreqVal, 1);

              // Clear DctMemSize after frequency change is done
              NBPtr->DCTPtr->Timings.DctMemSize = 0;
              break;
            }
          }
        }
      }
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for load card support
 *
 *  This function is used to set up
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubLoadCardSupport (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcMode;
  MEM_PARAMETER_STRUCT *RefPtr;
  PLATFORM_CONFIGURATION *PlatCfgPtr;

  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    RefPtr = &(((AMD_POST_PARAMS *) DataPtr)->MemConfig);
    PlatCfgPtr = &(((AMD_POST_PARAMS *) DataPtr)->PlatformConfig);
    if (LcMode != (IDS_STATUS)0) {
      RefPtr->BottomIo = 0x20;
      if (RefPtr->UmaMode != UMA_NONE) {
        RefPtr->UmaMode = UMA_SPECIFIED;
        RefPtr->UmaSize = 64 << (20 - 16);
      }
      RefPtr->MemHoleRemapping = TRUE;

      RefPtr->EnableBankIntlv = FALSE;
      RefPtr->EnableNodeIntlv = FALSE;
      RefPtr->EnableChannelIntlv = FALSE;
      RefPtr->EnableBankSwizzle = FALSE;
      RefPtr->EnableOnLineSpareCtl = FALSE;
      RefPtr->EnableMemClr = FALSE;

      // RefPtr->EnableParity = FALSE;
      RefPtr->EnableEccFeature = FALSE;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support of S3 Save before
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBeforeS3Save (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AGESA_STATUS            RetVal;
  MEM_MAIN_DATA_BLOCK     mmData;
  S3_MEM_NB_BLOCK         *S3NBPtr;
  MEM_DATA_STRUCT         *MemData;
  UINT8                   Die;
  UINT8                   Dct;
  IDS_STATUS              LcMode;
  IDS_STATUS              LcModeActive;
  AMD_RTB_PARAMS          *AmdRtbParams;
  VOID                    *OrMaskPtr;
  BSC_AP_MSR_SYNC         ApMsrSync[16];
  UINT64                  SMsr;
  CPU_LOGICAL_ID          LogicalCpuid;
  AGESA_TP                TestPoint;
  ALLOCATE_HEAP_PARAMS    AllocParams;

  AmdRtbParams = (AMD_RTB_PARAMS *) DataPtr;

  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if (LcMode != (IDS_STATUS)0) {
      IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
        if (LcModeActive == (IDS_STATUS)1) {
          IDS_HDT_CONSOLE_INIT (StdHeader);
          //
          // Solder-down load card support
          //
          if (LcMode > (IDS_STATUS)8) {
            // Display Post code CE to signal user to load NvStorage
            TestPoint = TpIfAmdInitRtbEntry;
            LibAmdIoWrite (AccessWidth8, 0x80, &TestPoint, StdHeader);

            // Allocate heap space for user to load NvStorage
            AllocParams.RequestedBufferSize = 0x8000;
            AllocParams.BufferHandle = AMD_S3_INFO_BUFFER_HANDLE;
            if (HeapAllocateBuffer (&AllocParams, &AmdRtbParams->StdHeader) != AGESA_SUCCESS) {
              ASSERT (FALSE);
              return IDS_SUCCESS;
            }
            AmdRtbParams->S3DataBlock.NvStorageSize = 0x8000;
            AmdRtbParams->S3DataBlock.NvStorage = AllocParams.BufferPtr;

            // Hand control to HDT for user to load NvStorage
            IdsIntSelfTrap (&(AmdRtbParams->S3DataBlock));
          }

          //
          // Restore registers to enable load card
          //
          if (AmdRtbParams->S3DataBlock.NvStorage != NULL) {
            WaitMicroseconds (1, &AmdRtbParams->StdHeader);
            MemS3ResumeInitNB (&AmdRtbParams->StdHeader);

            //  Creation of NB Block for S3 resume
            RetVal = MemS3InitNB (&S3NBPtr, &MemData, &mmData, &AmdRtbParams->StdHeader);
            if (RetVal == AGESA_FATAL) {
              return RetVal;
            }

            // Clear MemClkFreqVal before restoring AddrCmdTmg
            for (Die = 0; Die < mmData.DieCount; Die ++) {
              for (Dct = 0; Dct < S3NBPtr[Die].NBPtr->DctCount; Dct++) {
                if ((Die == 0) && (Dct == 0)) {
                  if ((LcMode & 7) == (IDS_STATUS)1) {
                    S3NBPtr[Die].NBPtr->SwitchDCT (S3NBPtr[Die].NBPtr, Dct);
                    if (S3NBPtr[Die].NBPtr->GetBitField (S3NBPtr[Die].NBPtr, BFODCControl) == 0x30333333) {
                      // If there is only one load card in channel A, clear MemClkFreqVal on channel A
                      S3NBPtr[Die].NBPtr->SetBitField (S3NBPtr[Die].NBPtr, BFMemClkFreqVal, 0);
                    }
                  }
                } else {
                  if ((LcMode & 7) == (IDS_STATUS)3) {
                    S3NBPtr[Die].NBPtr->SwitchDCT (S3NBPtr[Die].NBPtr, Dct);
                    if (S3NBPtr[Die].NBPtr->GetBitField (S3NBPtr[Die].NBPtr, BFODCControl) == 0x30333333) {
                      // If there is only one load card in channel B,C,D, clear MemClkFreqVal on their channels
                      S3NBPtr[Die].NBPtr->SetBitField (S3NBPtr[Die].NBPtr, BFMemClkFreqVal, 0);
                    }
                  }
                }
              }
            }

            // Restore registers before exiting self refresh
            RestorePreESRContext (&OrMaskPtr,
                                  AmdRtbParams->S3DataBlock.NvStorage,
                                  INIT_RESUME,
                                  &AmdRtbParams->StdHeader);

            if ((LcMode & 7) == (IDS_STATUS)1 || (LcMode & 7) == (IDS_STATUS)3) {
              // Restore F2x[1,0]90_x00, F2x9C_x0A, and F2x[1,0]9C_x0C
              // Restore F2x[1,0]9C_x04
              // Get the register value from the heap.
              for (Die = 0; Die < mmData.DieCount; Die ++) {
                S3NBPtr->MemS3Resume (&S3NBPtr[Die], Die);
              }
            }

            // Restore registers after exiting self refresh
            RestorePostESRContext (OrMaskPtr,
                                   AmdRtbParams->S3DataBlock.NvStorage,
                                   INIT_RESUME,
                                   &AmdRtbParams->StdHeader);

            if (S3NBPtr->NBPtr->DctCount == 1) {
              // When there is only one DCT, speculative reads to load card may foobar that only DCT and crash.
              // Disable branch prediction to avoid speculative reads
              LibAmdMsrRead (0xC0011021, &SMsr, &AmdRtbParams->StdHeader);
              SMsr |= 1;
              LibAmdMsrWrite (0xC0011021, &SMsr, &AmdRtbParams->StdHeader);
            }

            GetLogicalIdOfSocket (0, &LogicalCpuid, &(AmdRtbParams->StdHeader));

            ApMsrSync[0].RegisterAddress = SYS_CFG;
            ApMsrSync[1].RegisterAddress = TOP_MEM;
            ApMsrSync[2].RegisterAddress = TOP_MEM2;
            ApMsrSync[3].RegisterAddress = 0x200;
            ApMsrSync[4].RegisterAddress = 0x201;
            ApMsrSync[5].RegisterAddress = 0x202;
            ApMsrSync[6].RegisterAddress = 0x203;
            ApMsrSync[7].RegisterAddress = 0x204;
            ApMsrSync[8].RegisterAddress = 0x205;
            ApMsrSync[9].RegisterAddress = 0x206;
            ApMsrSync[10].RegisterAddress = 0x207;
            ApMsrSync[11].RegisterAddress = 0x208;
            ApMsrSync[12].RegisterAddress = 0x209;
            ApMsrSync[13].RegisterAddress = 0x20A;
            ApMsrSync[14].RegisterAddress = 0x20B;
            ApMsrSync[15].RegisterAddress = 0;
            GetLogicalIdOfSocket (0, &LogicalCpuid, &(AmdRtbParams->StdHeader));
            if ((LogicalCpuid.Family & AMD_FAMILY_15_CZ) == 0) {
              SyncApMsrsToBsc (ApMsrSync, &AmdRtbParams->StdHeader);
              RelinquishControlOfAllAPs (&AmdRtbParams->StdHeader);
            }
            return IDS_UNSUPPORTED;
          }

          HeapDeallocateBuffer (AMD_S3_INFO_BUFFER_HANDLE, &AmdRtbParams->StdHeader);
        }
      }
    }
  }
  AmdRtbParams->S3DataBlock.NvStorage = NULL;
  return IDS_SUCCESS;

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function is used to skip NBPstate transition
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - Succeed
 *     @return          FALSE - Fail
 */

BOOLEAN
IdsChangeNbFreq (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  BOOLEAN Status;

  Status = FALSE;

  // State machine to change NB frequency and NB Pstate
  if (NBPtr->NbFreqChgState == 0) {
    // Starting up by not changing NB P state, but only updating NB frequency based on current MemClk frequency
    Status = NBPtr->ChangeNbFrequencyWrap (NBPtr, 0);
    if (NBPtr->DCTPtr->Timings.Speed == NBPtr->DCTPtr->Timings.TargetSpeed) {
      // When MemClk has been ramped up to its max, exit
      NBPtr->NbFreqChgState = 4;
    }
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 *  IDS Backend Function for LoadCard Support of Dqs training after
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardAfterDqsTraining (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS    LcMode;
  IDS_STATUS    LcModeActive;
  MEM_NB_BLOCK  *NBPtr;
  UINT32        DctSelBaseAddr;
  UINT8 Die;
  UINT8  Dct;
  BIT_FIELD_NAME Reg;
  CPU_LOGICAL_ID LogicalCpuid;

  IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
    if (LcModeActive == (IDS_STATUS)1) {
      // If Load card is installed, check and disable DRAM channel that was forced to be enabled
      for (Die = 0 ; Die < ((MEM_MAIN_DATA_BLOCK *) DataPtr)->DieCount ; Die ++ ) {
        NBPtr = &(((MEM_MAIN_DATA_BLOCK *) DataPtr)->NBPtr[Die]);
        if (NBPtr->MCTPtr->NodeMemSize != 0) {
          for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
            NBPtr->SwitchDCT (NBPtr, Dct);
            if (NBPtr->DCTPtr->Timings.DctMemSize == 1) {
              // If a channel has no DIMMs but was forced to be enabled, disable it.
              NBPtr->DCTPtr->Timings.DctMemSize = 0;
              for (Reg = 0; Reg <= 7; Reg++) {
                if ((NBPtr->DCTPtr->Timings.CsTestFail & (1 << Reg)) != 0) {
                  NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + Reg, (0x00100000 * Reg) | 4);
                } else {
                  NBPtr->SetBitField (NBPtr, BFCSBaseAddr0Reg + Reg, 0);
                }
                if (Dct == 0) {
                  NBPtr->SetBitField (NBPtr, BFDctSelBaseAddr, 0);
                  NBPtr->SetBitField (NBPtr, BFDctSelBaseOffset, 0);
                }
              }
            }
          }
        }
      }
    }
  }

  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    NBPtr = &(((MEM_MAIN_DATA_BLOCK *) DataPtr)->NBPtr[BSP_DIE]);
    if ((LcMode & 7) == (IDS_STATUS)1) {
      GetLogicalIdOfSocket (0, &LogicalCpuid, StdHeader);
      if ((LogicalCpuid.Family & AMD_FAMILY_15_CZ) == 0) {
        // Load card will be installed in channel A of Node 0
        if (NBPtr->GetBitField (NBPtr, BFDctSelHi) == 1) {
          DctSelBaseAddr = NBPtr->MCTPtr->DctData[1].Timings.DctMemSize + 0x10000 - NBPtr->MCTPtr->NodeHoleBase;

          NBPtr->SetBitField (NBPtr, BFDctSelHi, 0);
          NBPtr->SetBitField (NBPtr, BFDctSelBaseAddr, DctSelBaseAddr >> 11);
          NBPtr->SetBitField (NBPtr, BFDctSelBaseOffset, DctSelBaseAddr >> 10);
        }
      }
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for special handling or Memory Error Recovery
 *  when load card is active.
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardMemErrorRecovery (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcModeActive;
  IDS_STATUS LcMode;
  DCT_STRUCT *DCTPtr;

  // if load card is active, always exclude all the dimms on a channel if
  // there exists dimm that has error.
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if ((LcMode & 7) == (IDS_STATUS)1) {
      IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
        if (LcModeActive == (IDS_STATUS)1) {
          DCTPtr = (DCT_STRUCT *)DataPtr;
          if (DCTPtr->Dct == 0) {
            DCTPtr->Timings.CsTestFail |= DCTPtr->Timings.CsPresent;
          }
        }
      }
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to bypass some special function during S3 restore
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBypassHook (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcMode;

  // if load card is active, always bypass the special S3 restore function.
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if (LcMode != (IDS_STATUS)0) {
      if (DataPtr == NULL) {
        return IDS_UNSUPPORTED;
      }
      if ((LcMode & 7) == (IDS_STATUS)1 && *(UINT8 *)DataPtr == 1 ||
        (LcMode & 7) == (IDS_STATUS)3 && *(UINT8 *)DataPtr == 0) {
        return IDS_UNSUPPORTED;
      }
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to tailor mapping profile from MCT to DCT
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardMemMappingOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcMode;

  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    // If load card in channel A, map system memory to other channels first
    if ((LcMode & 7) == (IDS_STATUS)1) {
      return IDS_UNSUPPORTED;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function
 *
 *  This function is used to fake DIMM settings before platform specific tables are processed
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBeforePlatTables (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS    LcMode;
  IDS_STATUS    LcModeActive;
  MEM_NB_BLOCK  *NBPtr;
  UINT8         LrDimmPresent;
  UINT8         DimmQrPresent;
  UINT8         DimmDrPresent;
  UINT8         DimmSRPresent;

  if (memPlatSpecFlowArray[0] != NULL) {
    // Only process if memPlatSpecFlowArray is supported with this current family
    IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
      if (LcModeActive == (IDS_STATUS)1) {
        IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
          NBPtr = (MEM_NB_BLOCK *) DataPtr;
          if ((LcMode & 7) == (IDS_STATUS)2 || (LcMode & 7) == (IDS_STATUS)4) {
            // Fake DIMMs on both slots
            if ((LrDimmPresent = NBPtr->ChannelPtr->LrDimmPresent) != 0) {
              NBPtr->ChannelPtr->LrDimmPresent |= 3;
            }
            if ((DimmQrPresent = NBPtr->ChannelPtr->DimmQrPresent) != 0) {
              NBPtr->ChannelPtr->DimmQrPresent |= 3;
            }
            if ((DimmDrPresent = NBPtr->ChannelPtr->DimmDrPresent) != 0) {
              NBPtr->ChannelPtr->DimmDrPresent |= 3;
            }
            if ((DimmSRPresent = NBPtr->ChannelPtr->DimmSRPresent) != 0) {
              NBPtr->ChannelPtr->DimmSRPresent |= 3;
            }
            // Save original DIMM presence
            LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1]), &LrDimmPresent, 1, StdHeader);
            LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1 + 1]), &DimmQrPresent, 1, StdHeader);
            LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1 + 2]), &DimmDrPresent, 1, StdHeader);
            LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1 + 3]), &DimmSRPresent, 1, StdHeader);
          }
        }
      }
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function
 *
 *  This function is used to override settings after platform specific tables are processed
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardAfterPlatTables (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS    LcMode;
  IDS_STATUS    LcModeActive;
  MEM_NB_BLOCK  *NBPtr;
  BOOLEAN       CopyRequested;
  UINT32        Twr;
  UINT32        Tcl;
  UINT16        OrgCsEn;

  if (memPlatSpecFlowArray[0] != NULL) {
    // Only process if memPlatSpecFlowArray is supported with this current family
    IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
      if (LcModeActive == (IDS_STATUS)1) {
        // Copy ODT settings
        IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
          NBPtr = (MEM_NB_BLOCK *) DataPtr;
          CopyRequested = TRUE;
          if ((LcMode & 7) == (IDS_STATUS)1) {
            if (NBPtr->Node == 0) {
              if (NBPtr->Dct == 0) {
                // Load card is installed on channel A, we need to initialize channel B data first.
                Twr = NBPtr->GetBitField (NBPtr, BFTwr);
                Tcl = NBPtr->GetBitField (NBPtr, BFTcl);
                OrgCsEn = NBPtr->MCTPtr->DctData[1].Timings.CsEnabled;
                NBPtr->MCTPtr->DctData[1].Timings.CsEnabled = NBPtr->MCTPtr->DctData[0].Timings.CsEnabled;
                NBPtr->SwitchDCT (NBPtr, 1);
                NBPtr->SetBitField (NBPtr, BFTwr, Twr);
                NBPtr->SetBitField (NBPtr, BFTcl, Tcl);
                NBPtr->PsPtr->MemPDoPs (NBPtr);
                NBPtr->SwitchDCT (NBPtr, 0);
                NBPtr->MCTPtr->DctData[1].Timings.CsEnabled = OrgCsEn;

                // Copy ODT settings of channel B to all channels
                LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1]), &(NBPtr->MCTPtr->DctData[1].ChData[0].DctOdcCtl), 28, StdHeader);
                LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_2]), &(NBPtr->MCTPtr->DctData[1].ChData[0].MemClkDisMap), 16, StdHeader);
                LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_3]), &(NBPtr->PSBlock[1]), sizeof (MEM_PS_BLOCK), StdHeader);
              } else {
                CopyRequested = FALSE;
              }
            } else {
              CopyRequested = FALSE;
            }
          } else if ((LcMode & 7) == (IDS_STATUS)2 || (LcMode & 7) == (IDS_STATUS)4) {
            // Restore orignial DIMM presence
            LibAmdMemCopy (&(NBPtr->ChannelPtr->LrDimmPresent), &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1]), 1, StdHeader);
            LibAmdMemCopy (&(NBPtr->ChannelPtr->DimmQrPresent), &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1 + 1]), 1, StdHeader);
            LibAmdMemCopy (&(NBPtr->ChannelPtr->DimmDrPresent), &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1 + 2]), 1, StdHeader);
            LibAmdMemCopy (&(NBPtr->ChannelPtr->DimmSRPresent), &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1 + 3]), 1, StdHeader);
            CopyRequested = FALSE;
          } else if ((LcMode & 7) == (IDS_STATUS)3) {
            if (NBPtr->Node == 0) {
              // Load card is installed on channel B, use ODT settings of channel A for all channels
              LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1]), &(NBPtr->MCTPtr->DctData[0].ChData[0].DctOdcCtl), 28, StdHeader);
              LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_2]), &(NBPtr->MCTPtr->DctData[0].ChData[0].MemClkDisMap), 16, StdHeader);
              LibAmdMemCopy (&(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_3]), &(NBPtr->PSBlock[0]), sizeof (MEM_PS_BLOCK), StdHeader);
              if ((NBPtr->Node == 0) && (NBPtr->Dct == 0)) {
                CopyRequested = FALSE;
              }
            }
          } else {
            CopyRequested = FALSE;
          }
          if (CopyRequested) {
            LibAmdMemCopy (&(NBPtr->ChannelPtr->DctOdcCtl), &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_1]), 28, StdHeader);
            LibAmdMemCopy (&(NBPtr->ChannelPtr->MemClkDisMap), &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_2]), 16, StdHeader);
            LibAmdMemCopy (NBPtr->PsPtr, &(NBPtr->MemPtr->FunctionList.Reserved[LC_BUFF_SEG_3]), sizeof (MEM_PS_BLOCK), StdHeader);
          }
        }
        return IDS_UNSUPPORTED;
      }
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to override Dimm Mask
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardDimmMaskOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT8       *DimmMask;
  IDS_STATUS  LcMode;

  DimmMask = (UINT8 *) DataPtr;
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if ((LcMode & 7) == (IDS_STATUS)1) {
      // Load card in channel A. Avoid saving/restoring channel B timings.
      *DimmMask &= 0x55;
    } else if ((LcMode & 7) == (IDS_STATUS)2) {
      // Load card in near slot. Avoid saving/restoring DIMM 1 timings.
      *DimmMask &= 0x3;
    } else if ((LcMode & 7) == (IDS_STATUS)3) {
      // Load card in channel B. Avoid saving/restoring channel A timings.
      *DimmMask &= 0xAA;
    } else if ((LcMode & 7) == (IDS_STATUS)4) {
      // Load card in far slot. Avoid saving/restoring DIMM 0 timings.
      *DimmMask &= 0xC;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to bypass restoring certain S3 registers
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardBypassS3Registers (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  PCI_ADDR    *Register;
  IDS_STATUS  LcMode;

  Register = (PCI_ADDR *) DataPtr;
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if ((LcMode & 7) == (IDS_STATUS)2 || (LcMode & 7) == (IDS_STATUS)4) {
      // Load card in near/far slot. Avoid saving/restoring RdPtrOffset
      if (Register->Address.Register >= RegDataRdPtrOffset0 && Register->Address.Register <= RegDataRdPtrOffset8) {
        return IDS_UNSUPPORTED;
      }
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support after S3 Save
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardAfterS3Save (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS  LcMode;
  IDS_STATUS  LcModeActive;
  AGESA_TP    TestPoint;
  AMD_RTB_PARAMS *AmdRtbParams;

  AmdRtbParams = (AMD_RTB_PARAMS *) DataPtr;

  //
  // Solder-down load card support
  //
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if (LcMode > (IDS_STATUS)8) {
      IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
        if (LcModeActive == (IDS_STATUS)0) {
          // Display Post code CF to signal user to save NvStorage
          TestPoint = TpIfAmdInitRtbEntry;
          LibAmdIoWrite (AccessWidth8, 0x80, &TestPoint, StdHeader);

          // Hand control to HDT for user to save NvStorage
          IdsIntSelfTrap (&(AmdRtbParams->S3DataBlock));
        }
      }
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS static function that self-traps and hands control to HDT
 *  to pass S3DataBlock
 *
 *  @param[in,out]   S3DataBlock  - Pointer to S3 NV storage
 *
 **/
VOID
STATIC
IdsIntSelfTrap (
  IN OUT   S3_DATA_BLOCK *S3DataBlock
  )
{
  UINT32 Dr2Reg;
  UINT32 Dr7Reg;
  UINT32 CR4reg;
  UINT64 Msr;

  LibAmdMsrRead (0xC001100A, &Msr, NULL);
  Msr |= 1;
  LibAmdMsrWrite (0xC001100A, &Msr, NULL);

  LibAmdReadCpuReg (DR2_REG, &Dr2Reg);
  LibAmdWriteCpuReg (DR2_REG, 0xCECF);

  LibAmdReadCpuReg (DR2_REG, &Dr7Reg);
  LibAmdWriteCpuReg (DR7_REG, 0x02000420);

  LibAmdReadCpuReg (CR4_REG, &CR4reg);
  LibAmdWriteCpuReg (CR4_REG, CR4reg | ((UINT32)1 << 3));

  IdsOutPort (0xCECF, (UINT32) (UINT64) S3DataBlock->NvStorage, (UINT32) S3DataBlock->NvStorageSize);

  LibAmdWriteCpuReg (DR2_REG, Dr2Reg);
  LibAmdWriteCpuReg (DR7_REG, Dr7Reg);
  LibAmdWriteCpuReg (CR4_REG, CR4reg);
}

/**
 *  IDS Backend Function for LoadCard Support to poll DctAccessDone
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardAfterDctPhyAccess (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcModeActive;
  PCI_ADDR PciAddr;
  UINT32 RegValueRead;
  MEM_NB_BLOCK  *NBPtr;

  // if load card is active, poll DctAccessDone
  IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
    if (LcModeActive == (IDS_STATUS)1) {
      if (DataPtr != NULL) {
        NBPtr = (MEM_NB_BLOCK *) DataPtr;
        PciAddr.Address.Device = NBPtr->PciAddr.Address.Device;
        PciAddr.Address.Bus = NBPtr->PciAddr.Address.Bus;
        PciAddr.Address.Segment = NBPtr->PciAddr.Address.Segment;
      } else {
        PciAddr.Address.Device = 24;
        PciAddr.Address.Bus = 0;
        PciAddr.Address.Segment = 0;
      }
      PciAddr.Address.Function = 2;
      PciAddr.Address.Register = 0x98;
      RegValueRead = 0;
      while ((RegValueRead & 0x80000000) == 0) {
        LibAmdPciRead (AccessS3SaveWidth32, PciAddr, &RegValueRead, StdHeader);
      }
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to bypass M0 enforcement
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardForcePhyToM0 (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcModeActive;
  MEM_NB_BLOCK  *NBPtr;

  // if load card is active, always bypass the special S3 restore function.
  IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
    if (LcModeActive == (IDS_STATUS)1) {
      NBPtr = (MEM_NB_BLOCK *) DataPtr;
      NBPtr->IsSupported[ForcePhyToM0] = FALSE;
      return IDS_SUCCESS;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to add some delay after every PCI restore
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardAfterPciRegRestore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS LcModeActive;

  IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
    if (LcModeActive == (IDS_STATUS)1) {
      WaitMicroseconds (1, StdHeader);
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for LoadCard Support to change Power Policy to performance
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubLoadCardPlatCfgOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  PLATFORM_CONFIGURATION *PlatformConfig;
  IDS_STATUS LcMode;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;

  // if load card is active, always disable NBPstate
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if (LcMode != 0) {
      PlatformConfig->PlatformProfile.MemoryPowerPolicy = Performance;
      PlatformConfig->PlatformProfile.NbPstatesSupported = FALSE;
    }
  }

  return IDS_SUCCESS;
}

