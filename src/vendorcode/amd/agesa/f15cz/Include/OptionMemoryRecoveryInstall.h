/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: Memory
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 */
/*****************************************************************************
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
 *
 ***************************************************************************/

#ifndef _OPTION_MEMORY_RECOVERY_INSTALL_H_
#define _OPTION_MEMORY_RECOVERY_INSTALL_H_

#if (AGESA_ENTRY_INIT_RECOVERY == TRUE)

  #if (OPTION_MEMCTLR_TN == TRUE)
    extern MEM_REC_NB_CONSTRUCTOR MemRecConstructNBBlockTN;
    #define MEM_REC_NB_SUPPORT_TN MemRecConstructNBBlockTN,
  #else
    #define MEM_REC_NB_SUPPORT_TN
  #endif

  MEM_REC_NB_CONSTRUCTOR* MemRecNBInstalled[] = {
    MEM_REC_NB_SUPPORT_TN
    NULL
  };

  #define MEM_REC_TECH_CONSTRUCTOR_DDR2
  #if (OPTION_DDR3 == TRUE)
    extern MEM_REC_TECH_CONSTRUCTOR MemRecConstructTechBlock3;
    #define MEM_REC_TECH_CONSTRUCTOR_DDR3 MemRecConstructTechBlock3,
  #else
    #define MEM_REC_TECH_CONSTRUCTOR_DDR3
  #endif

  MEM_REC_TECH_CONSTRUCTOR* MemRecTechInstalled[] = {
    MEM_REC_TECH_CONSTRUCTOR_DDR3
    MEM_REC_TECH_CONSTRUCTOR_DDR2
    NULL
  };

  MEM_PLATFORM_CFG* memRecPlatformTypeInstalled[] = {
    NULL
  };

  /*---------------------------------------------------------------------------------------------------
   * EXTRACTABLE PLATFORM SPECIFIC CONFIGURATION
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  #define MEM_PSC_REC_FLOW_BLOCK_END NULL
  #define PSC_REC_TBL_END NULL
  #define MEM_REC_PSC_FLOW_DEFTRUE (BOOLEAN (*) (MEM_NB_BLOCK*, MEM_PSC_TABLE_BLOCK *)) MemRecDefTrue

  #if OPTION_MEMCTLR_TN
    #if OPTION_UDIMMS
      extern PSC_TBL_ENTRY RecTNDramTermTblEntU;
      #define PSC_REC_TBL_TN_UDIMM3_DRAM_TERM  &RecTNDramTermTblEntU,
      extern PSC_TBL_ENTRY RecTNSAOTblEntU3;
      #define PSC_REC_TBL_TN_UDIMM3_SAO  &RecTNSAOTblEntU3,
    #endif
    #if OPTION_SODIMMS
      extern PSC_TBL_ENTRY RecTNSAOTblEntSO3;
      #define PSC_REC_TBL_TN_SODIMM3_SAO  &RecTNSAOTblEntSO3,
      extern PSC_TBL_ENTRY RecTNDramTermTblEntSO;
      #define PSC_REC_TBL_TN_SODIMM3_DRAM_TERM  &RecTNDramTermTblEntSO,
    #endif
    extern PSC_TBL_ENTRY RecTNMR0WrTblEntry;
    extern PSC_TBL_ENTRY RecTNMR0CLTblEntry;
    extern PSC_TBL_ENTRY RecTNDdr3CKETriEnt;
    extern PSC_TBL_ENTRY RecTNOdtPatTblEnt;

    #ifndef PSC_REC_TBL_TN_UDIMM3_DRAM_TERM
      #define PSC_REC_TBL_TN_UDIMM3_DRAM_TERM
    #endif
    #ifndef PSC_REC_TBL_TN_SODIMM3_DRAM_TERM
      #define PSC_REC_TBL_TN_SODIMM3_DRAM_TERM
    #endif
    #ifndef PSC_REC_TBL_TN_SODIMM3_SAO
      #define PSC_REC_TBL_TN_SODIMM3_SAO
    #endif
    #ifndef PSC_REC_TBL_TN_UDIMM3_SAO
      #define PSC_REC_TBL_TN_UDIMM3_SAO
    #endif

    PSC_TBL_ENTRY* memRecPSCTblDramTermArrayTN[] = {
      PSC_REC_TBL_TN_UDIMM3_DRAM_TERM
      PSC_REC_TBL_TN_SODIMM3_DRAM_TERM
      PSC_REC_TBL_END
    };

    PSC_TBL_ENTRY* memRecPSCTblODTPatArrayTN[] = {
      &RecTNOdtPatTblEnt,
      PSC_REC_TBL_END
    };

    PSC_TBL_ENTRY* memRecPSCTblSAOArrayTN[] = {
      PSC_REC_TBL_TN_SODIMM3_SAO
      PSC_REC_TBL_TN_UDIMM3_SAO
      PSC_REC_TBL_END
    };

    PSC_TBL_ENTRY* memRecPSCTblMR0WRArrayTN[] = {
      &RecTNMR0WrTblEntry,
      PSC_REC_TBL_END
    };

    PSC_TBL_ENTRY* memRecPSCTblMR0CLArrayTN[] = {
      &RecTNMR0CLTblEntry,
      PSC_REC_TBL_END
    };

    MEM_PSC_TABLE_BLOCK memRecPSCTblBlockTN = {
      NULL,
      (PSC_TBL_ENTRY **)&memRecPSCTblDramTermArrayTN,
      (PSC_TBL_ENTRY **)&memRecPSCTblODTPatArrayTN,
      (PSC_TBL_ENTRY **)&memRecPSCTblSAOArrayTN,
      (PSC_TBL_ENTRY **)&memRecPSCTblMR0WRArrayTN,
      (PSC_TBL_ENTRY **)&memRecPSCTblMR0CLArrayTN,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL
    };
    extern MEM_PSC_FLOW MemPRecGetRttNomWr;
    #define PSC_REC_FLOW_TN_DRAM_TERM   MemPRecGetRttNomWr
    extern MEM_PSC_FLOW MemPRecGetODTPattern;
    #define PSC_REC_FLOW_TN_ODT_PATTERN   MemPRecGetODTPattern
    extern MEM_PSC_FLOW MemPRecGetSAO;
    #define PSC_REC_FLOW_TN_SAO   MemPRecGetSAO
    extern MEM_PSC_FLOW MemPRecGetMR0WrCL;
    #define PSC_REC_FLOW_TN_MR0_WRCL   MemPRecGetMR0WrCL

    MEM_PSC_FLOW_BLOCK memRecPlatSpecFlowTN = {
      &memRecPSCTblBlockTN,
      MEM_REC_PSC_FLOW_DEFTRUE,
      PSC_REC_FLOW_TN_DRAM_TERM,
      PSC_REC_FLOW_TN_ODT_PATTERN,
      PSC_REC_FLOW_TN_SAO,
      PSC_REC_FLOW_TN_MR0_WRCL,
      MEM_REC_PSC_FLOW_DEFTRUE,
      MEM_REC_PSC_FLOW_DEFTRUE,
      MEM_REC_PSC_FLOW_DEFTRUE,
      MEM_REC_PSC_FLOW_DEFTRUE,
      MEM_REC_PSC_FLOW_DEFTRUE,
      MEM_REC_PSC_FLOW_DEFTRUE
    };
    #define MEM_PSC_REC_FLOW_BLOCK_TN &memRecPlatSpecFlowTN,
  #else
    #define MEM_PSC_REC_FLOW_BLOCK_TN
  #endif

  MEM_PSC_FLOW_BLOCK* memRecPlatSpecFlowArray[] = {
    MEM_PSC_REC_FLOW_BLOCK_TN
    MEM_PSC_REC_FLOW_BLOCK_END
  };

#else
  /*---------------------------------------------------------------------------------------------------
   * DEFAULT TECHNOLOGY BLOCK
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  MEM_TECH_CONSTRUCTOR* MemRecTechInstalled[] = {    // Types of technology installed
    NULL
  };
  /*---------------------------------------------------------------------------------------------------
   * DEFAULT NORTHBRIDGE SUPPORT LIST
   *
   *
   *---------------------------------------------------------------------------------------------------
   */
  MEM_REC_NB_CONSTRUCTOR* MemRecNBInstalled[] = {
    NULL
  };
  /*----------------------------------------------------------------------
   * DEFAULT PSCFG DEFINITIONS
   *
   *----------------------------------------------------------------------
   */
  MEM_PLATFORM_CFG* memRecPlatformTypeInstalled[] = {
    NULL
  };
  /*----------------------------------------------------------------------
   * EXTRACTABLE PLATFORM SPECIFIC CONFIGURATION
   *
   *----------------------------------------------------------------------
   */
  MEM_PSC_FLOW_BLOCK* memRecPlatSpecFlowArray[] = {
    NULL
  };
#endif
#endif  // _OPTION_MEMORY_RECOVERY_INSTALL_H_
