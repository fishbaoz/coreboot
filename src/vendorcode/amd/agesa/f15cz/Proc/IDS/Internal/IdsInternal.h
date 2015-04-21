/* $NoKeywords:$ */
/**
 * @file
 *
 * IdsInternal.h
 *
 * Contains AMD AGESA Internal Integrated Debug Macros
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project: (Proc/Ids/Internal)
 * @e \$Revision: 313864 $   @e \$Date: 2015-03-02 15:15:13 +0800 (Mon, 02 Mar 2015) $
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

  /* Macros to aid debugging */
  /* These definitions expand to zero (0) bytes of code when disabled */

#ifndef _IDS_INTERNAL_H_
#define _IDS_INTERNAL_H_

#define __IDS_EXTENDED__
//proto type
IDS_STATUS
IdsInternalOptionHook (
  IN       AGESA_IDS_OPTION IdsOption,
  IN OUT   VOID *DataPtr,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsInternalGetDataPei (
  IN       UINTN              Data,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );


VOID
IdsInternalGetDataDxe (
  IN       UINTN              Data,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

///Data struct for BVM AGESA entry point
typedef struct {
  IN OUT    AMD_CONFIG_PARAMS   StdHeader;            ///< Standard header
  IN        UINT16              platformtp;       ///< platform time point
} IDS_BVM_PARAMS;

AGESA_STATUS
IdsBvmAgesaEntry (
  IN       IDS_BVM_PARAMS *IdsBvmPara
  );

#define IDS_PAD_4K 4096
//
// internal options
//

#define IDS_EXTENDED_HOOK IdsInternalOptionHook
#define IDS_EXTENDED_HEAP_SIZE 100
#define IDS_TRACE_DEFAULT (BVM_TRACE)

//For IDS internal UEFI implementation
#define IDS_EXTENDED_GET_DATA_EARLY(data, StdHeader) IdsInternalGetDataPei (data, StdHeader)
#define IDS_EXTENDED_GET_DATA_LATE(data, StdHeader) IdsInternalGetDataDxe (data, StdHeader)


//IDS extend include file
#define OPTION_IDS_EXT_INSTALL_FILE IDS_STRINGIZE(Internal/OptionIdsInternalInstall.h)
#define IDS_EXT_INCLUDE(file) IDS_STRINGIZE(Internal/file.h)
#define IDS_EXT_INCLUDE_F15(file) IDS_STRINGIZE(Internal/Family/0x15/file.h)
#define IDS_EXT_INCLUDE_F15_CZ(file) IDS_STRINGIZE(Internal/Family/0x15/CZ/file.h)
#define IDS_EXT_INCLUDE_F15_ST(file) IDS_STRINGIZE(Internal/Family/0x15/ST/file.h)

///Common IDS extend code macro
#define IDS_EXTENDED_CODE(code) code

/// Define Internal CALLOUT IDs
#define IDS_CALLOUT_INT_EARLY_AFTER      0x81       ///< The function data of IDS internal callout function of early after.
#define IDS_CALLOUT_INT_LATE_AFTER      0x82       ///< The function data of IDS internal callout function of late after.
#define IDS_CALLOUT_BVM_GET_BASE        0xC0      ///< Callout Id for BVM get binary base.

/// SMU BIOS general header
typedef struct _SMU_FIRMWARE_BIOS_HEADER_STRUCT {
  UINT64 Signature;     ///< "_SMU_SMU"
  UINT32 Length;        ///< Length include header exclue Pad in byte
  UINT32 OptionPadSize; ///< Optional, Size of Pad in byte
  UINT32 Revision;      ///< Revision of the header
  UINT32 CheckSum;      ///< Checksum include header
  UINT32 ProgramStr;    ///< Program String
  UINT8  Reserved [20]; ///< Reserved
} SMU_FIRMWARE_BIOS_HEADER_STRUCT;

/// Define SMU Firmware Pads for possibility of binary replacement
#define SMU_FIRMWARE_PADS_1K\
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000, \
        0x00000000, 0x00000000, 0x00000000, 0x00000000,

/// AGESA IDS Internal NV ID Definitions

typedef enum {
  AGESA_IDS_INT_ID_START = AGESA_IDS_EXT_ID_END + 1,///< 0x0024                                                             specify the start of internal NV id

  AGESA_IDS_NV_ACPI_SRAT,                                     ///< 0x0025                                                          Enable or disable the ACPI SRAT tables
  AGESA_IDS_NV_CORE_LEVEL,                                    ///< 0x0026                                                              Set the number of Cores to be used

  AGESA_IDS_NV_CACHEFLUSHHLT,                                 ///< 0x0027                                   Set the core clock divisor after the caches have been flushed
  AGESA_IDS_NV_CACHEFLUSHHLTTMR,                              ///< 0x0028                                                 Set the time of each core needs to stay in halt
  AGESA_IDS_NV_CACHEFLUSHHLTDIV,                              ///< 0x0029                                                               Cache Flush on Halt Clock divisor
  AGESA_IDS_NV_CACHEFLUSHTMR,                                 ///< 0x002A                                                                               Cache Flush Timer
  AGESA_IDS_NV_CFSUCCESSMONCTL,                               ///< 0x002B                                                             Cache Flush Success Monitor Control
  AGESA_IDS_NV_CACHEFLUSHSUCMONTHRESHOLD,                     ///< 0x002C                                                           Cache flush success monitor threshold

  AGESA_IDS_NV_DCACHE,                                        ///< 0x002D                                                                       Enable or disable D-Cache

  AGESA_IDS_NV_ACPI_PSS,                                      ///< 0x002E                                                              Enable or disable ACPI PSS objects
  AGESA_IDS_NV_ACPI_CST,                                      ///< 0x002F                                                                        Disable ACPI CST objects
  AGESA_IDS_NV_ACPI_WHEA,                                     ///< 0x0030                                                             Enable or disable ACPI WHEA objects
  AGESA_IDS_NV_ACPI_SLIT,                                     ///< 0x0031                                                             Enable or disable ACPI SLIT objects
  AGESA_IDS_NV_DMIEN,                                         ///< 0x0032                                                           Controls activation of CPU DMI Table!

  AGESA_IDS_NV_CUSTOM_PSTATE,                                 ///< 0x0033                                                               whether to configure the P-States
  AGESA_IDS_NV_NUMCUSTOMPSTATES,                              ///< 0x0034                                                                  Defines the number of P-states
  AGESA_IDS_NV_PSTATENBFID,                                   ///< 0x0035                                                                                  Set the NB FID

  AGESA_IDS_NV_PSTATE0COREFID,                                ///< 0x0036                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE0COREVID,                                ///< 0x0037                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE0COREDID,                                ///< 0x0038                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE0COREDIDLSD,                             ///< 0x0039                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE0NBVID,                                  ///< 0x003A                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE0NBDID,                                  ///< 0x003B                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE0NBFID,                                  ///< 0x003C                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE1COREFID,                                ///< 0x003D                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE1COREVID,                                ///< 0x003E                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE1COREDID,                                ///< 0x003F                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE1COREDIDLSD,                             ///< 0x0040                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE1NBVID,                                  ///< 0x0041                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE1NBDID,                                  ///< 0x0042                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE1NBFID,                                  ///< 0x0043                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE2COREFID,                                ///< 0x0044                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE2COREVID,                                ///< 0x0045                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE2COREDID,                                ///< 0x0046                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE2COREDIDLSD,                             ///< 0x0047                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE2NBVID,                                  ///< 0x0048                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE2NBDID,                                  ///< 0x0049                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE2NBFID,                                  ///< 0x004A                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE3COREFID,                                ///< 0x004B                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE3COREVID,                                ///< 0x004C                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE3COREDID,                                ///< 0x004D                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE3COREDIDLSD,                             ///< 0x004E                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE3NBVID,                                  ///< 0x004F                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE3NBDID,                                  ///< 0x0050                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE3NBFID,                                  ///< 0x0051                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE4COREFID,                                ///< 0x0052                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE4COREVID,                                ///< 0x0053                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE4COREDID,                                ///< 0x0054                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE4COREDIDLSD,                             ///< 0x0055                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE4NBVID,                                  ///< 0x0056                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE4NBDID,                                  ///< 0x0057                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE4NBFID,                                  ///< 0x0058                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE5COREFID,                                ///< 0x0059                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE5COREVID,                                ///< 0x005A                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE5COREDID,                                ///< 0x005B                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE5COREDIDLSD,                             ///< 0x005C                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE5NBVID,                                  ///< 0x005D                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE5NBDID,                                  ///< 0x005E                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE5NBFID,                                  ///< 0x005F                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE6COREFID,                                ///< 0x0060                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE6COREVID,                                ///< 0x0061                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE6COREDID,                                ///< 0x0062                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE6COREDIDLSD,                             ///< 0x0063                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE6NBVID,                                  ///< 0x0064                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE6NBDID,                                  ///< 0x0065                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE6NBFID,                                  ///< 0x0066                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE7COREFID,                                ///< 0x0067                                                                Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE7COREVID,                                ///< 0x0068                                                                Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE7COREDID,                                ///< 0x0069                                                                Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE7COREDIDLSD,                             ///< 0x006A                                                            Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE7NBVID,                                  ///< 0x006B                                                              Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE7NBDID,                                  ///< 0x006C                                                              Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE7NBFID,                                  ///< 0x006D                                                              Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSIEN,                                         ///< 0x006E                                                                           Enable or disable PSI


  AGESA_IDS_NV_MEMORY_PARITY,                                 ///< 0x006F                                                               Enable or disable the DRAM Parity

  AGESA_IDS_NV_CLKHZALTVIDC3EN,                               ///< 0x0070                                                       Enable or disable the AltVidC3MemClkTriEn

  AGESA_IDS_NV_SYSMEMSIZEOVERLAYEN,                           ///< 0x0071                                                    Enable or disable System Memory Size Overlay
  AGESA_IDS_NV_SYSMEMSIZE,                                    ///< 0x0072                                                                      System Memory Size Overlay

  AGESA_IDS_NV_INTSWAPEN,                                     ///< 0x0073                                                                   Interleave region swap enable
  AGESA_IDS_NV_SWAPBASE,                                      ///< 0x0074                                                 Interleave swap region base address bits[33:27]
  AGESA_IDS_NV_SWAPLIMIT,                                     ///< 0x0075                                                Interleave swap region limit address bits[33:27]
  AGESA_IDS_NV_SWAPSIZE,                                      ///< 0x0076                                                         Interleave swap region size bits[33:27]

  AGESA_IDS_NV_POR_MEM_FREQ,                                  ///< 0x0077                                                   Enable of diable MemClk frequency enforcement
  AGESA_IDS_NV_DRAMTIME,                                      ///< 0x0078                                                               Set the DRAM Timing Configuration
  AGESA_IDS_NV_MEMCLKSPEED,                                   ///< 0x0079                                                                      Set the memory clock speed
  AGESA_IDS_NV_MEMSPEED,                                      ///< 0x007A                                                                  Set the memory clock frequency
  AGESA_IDS_NV_RDPTRDDR3,                                     ///< 0x007B                                                                                           RdPtr
  AGESA_IDS_NV_TCL,                                           ///< 0x007C                                                                                Set the tCL time
  AGESA_IDS_NV_TRCD,                                          ///< 0x007D                                               This Set the RAS# Active to CAS# read/write delay
  AGESA_IDS_NV_TRP,                                           ///< 0x007E                                                                  Specify the row precharge time
  AGESA_IDS_NV_TRTP,                                          ///< 0x007F                                                       Specifies the read CAS# to precharge time
  AGESA_IDS_NV_TRAS,                                          ///< 0x0080                                                                Specify the min RAS# active time
  AGESA_IDS_NV_TRRD,                                          ///< 0x0081                                                              Specify the active-to-active delay
  AGESA_IDS_NV_TRC,                                           ///< 0x0082                                                     Specify the RAS# active to RAS# active time
  AGESA_IDS_NV_TRWTTO,                                        ///< 0x0083                                                                        This Set the tRWTTO time
  AGESA_IDS_NV_TWR,                                           ///< 0x0084                                           Specify when the last write is registered by the DRAM
  AGESA_IDS_NV_TWRRD,                                         ///< 0x0085                                           Specify when the last write is registered by the DRAM
  AGESA_IDS_NV_TWTR,                                          ///< 0x0086                                    Specify the write to read delay when accessing the same DIMM
  AGESA_IDS_NV_TWRWR,                                         ///< 0x0087                                                                          Specify the Twrwr time
  AGESA_IDS_NV_TWRWRSDSC,                                     ///< 0x0088                                                                                       tWRWRSdSc
  AGESA_IDS_NV_TWRWRSDDC,                                     ///< 0x0089                                                                                       tWRWRSdDc
  AGESA_IDS_NV_TWRWRDD,                                       ///< 0x008A                                                                                         tWRWRDd
  AGESA_IDS_NV_TRDRD,                                         ///< 0x008B                                                                          Specify the Trdrd time
  AGESA_IDS_NV_TRDRDSDSC,                                     ///< 0x008C                                                                                       tRDRDSdSc
  AGESA_IDS_NV_TRDRDSDDC,                                     ///< 0x008D                                                                                       tRDRDSdDc
  AGESA_IDS_NV_TRDRDDD,                                       ///< 0x008E                                                                                         tRDRDDd
  AGESA_IDS_NV_TREF,                                          ///< 0x008F                                                                                            tREF
  AGESA_IDS_NV_TRWTWB,                                        ///< 0x0090                                                                         Specify the Trwtwb time
  AGESA_IDS_NV_TRFCx,                                         ///< 0x0091                                                                          Specify the Trfcx time
  AGESA_IDS_NV_TRFC0,                                         ///< 0x0092                                                                          Specify the Trfc0 time
  AGESA_IDS_NV_TRFC1,                                         ///< 0x0093                                                                          Specify the Trfc1 time
  AGESA_IDS_NV_TRFC2,                                         ///< 0x0094                                                                          Specify the Trfc2 time
  AGESA_IDS_NV_TRFC3,                                         ///< 0x0095                                                                          Specify the Trfc3 time

  AGESA_IDS_NV_DRAMCONTRSEL,                                  ///< 0x0096                                                                      Select the DRAM Controller
  AGESA_IDS_NV_PCHGPDMODESEL,                                 ///< 0x0097                                                                Precharge power down mode select
  AGESA_IDS_NV_DIMMODT,                                       ///< 0x0098                                                                      Set the On Die Termination
  AGESA_IDS_NV_DRAMDRVWEAK,                                   ///< 0x0099                                                                     Set the DRAM drive strength
  AGESA_IDS_NV_DRAMSELFREFRATEEN,                             ///< 0x009A                                                    Enable or disable the DRAM Self Refresh Rate

  AGESA_IDS_NV_CMDMOTIME,                                     ///< 0x009B                                                       Select between 1T and 2T mode on ADDR/CMD
  AGESA_IDS_NV_BANK_SWIZZLE,                                  ///< 0x009C                                                             Enable or disable bank swizzle mode
  AGESA_IDS_NV_THROTTLEDCT,                                   ///< 0x009D            When MEMHOT_L pin is asserted throttle both DCT channels to the  average utilization
  AGESA_IDS_NV_TREFRATEDCT,                                   ///< 0x009E                                 When MEMHOT_L pin is asserted Tref forced to 3.9us auto refresh
  AGESA_IDS_NV_LOAD_CARD_MODE,                                ///< 0x009F                                                    Set where the load card *will* be swapped in
  AGESA_IDS_NV_LOAD_CARD_ACTIVE,                              ///< 0x00A0                                               Specify if the load card is swapped in the system

  AGESA_IDS_NV_MemClr,                                        ///< 0x00A1                                                                        Enable or disable MemClr
  AGESA_IDS_NV_PhyDLLStandby,                                 ///< 0x00A2                                                                 Enable or disable PhyDLLStandby
  AGESA_IDS_NV_RxDLLWakeupTime,                               ///< 0x00A3                                                                         Set the RxDLLWakeupTime
  AGESA_IDS_NV_RxCPUpdPeriod,                                 ///< 0x00A4                                                                           Set the RxCPUpdPeriod
  AGESA_IDS_NV_RxMaxDurDllNoLock,                             ///< 0x00A5                                                                       Set the RxMaxDurDllNoLock
  AGESA_IDS_NV_TxDLLWakeupTime,                               ///< 0x00A6                                                                         Set the TxDLLWakeupTime
  AGESA_IDS_NV_TxCPUpdPeriod,                                 ///< 0x00A7                                                                           Set the TxCPUpdPeriod
  AGESA_IDS_NV_TxMaxDurDllNoLock,                             ///< 0x00A8                                                                       Set the TxMaxDurDllNoLock
  AGESA_IDS_NV_MEM_ERROR_RECOVERY,                            ///< 0x00A9                                                           Specify if error recovery is enabled.
  AGESA_IDS_NV_MEM_RETRAIN_TIMES,                             ///< 0x00AA                          Specify how many times training should be executed should error occurs
  AGESA_IDS_NV_DLLRegDis,                                     ///< 0x00AB                                                         Enable or disable DLL Regulator Disable

  AGESA_IDS_NV_DRSTRCON,                                      ///< 0x00AC                                                  Set the mode for Drive Strength initialization
  AGESA_IDS_NV_CKEDRSTR,                                      ///< 0x00AD                                                                      Set the CKE drive strength
  AGESA_IDS_NV_CODRSTR,                                       ///< 0x00AE                                                                   Set the CS/ODT drive strength
  AGESA_IDS_NV_ACDRSTR,                                       ///< 0x00AF                                                                 Set the Addr/Cmd drive strength
  AGESA_IDS_NV_CLKDRSTR,                                      ///< 0x00B0                                                                    Set the Clock drive strength
  AGESA_IDS_NV_DTDRSTR,                                       ///< 0x00B1                                                                     Set the data drive strength
  AGESA_IDS_NV_DQSDRSTR,                                      ///< 0x00B2                                                                      Set the DQS drive strength
  AGESA_IDS_NV_DQSPROCODT,                                    ///< 0x00B3                                                                              Set the ProcOdtDis

  AGESA_IDS_NV_DQSDELAYS,                                     ///< 0x00B4                                                     Set the mode for DQS training configuration
  AGESA_IDS_NV_WRBYTEDL0,                                     ///< 0x00B5                                                            Set the write data fine delay byte 0
  AGESA_IDS_NV_WRBYTEDLG0,                                    ///< 0x00B6                                                           Set the write data gross delay byte 0
  AGESA_IDS_NV_WRBYTEDL1,                                     ///< 0x00B7                                                            Set the write data fine delay byte 1
  AGESA_IDS_NV_WRBYTEDLG1,                                    ///< 0x00B8                                                           Set the write data gross delay byte 1
  AGESA_IDS_NV_WRBYTEDL2,                                     ///< 0x00B9                                                            Set the write data fine delay byte 2
  AGESA_IDS_NV_WRBYTEDLG2,                                    ///< 0x00BA                                                           Set the write data gross delay byte 2
  AGESA_IDS_NV_WRBYTEDL3,                                     ///< 0x00BB                                                            Set the write data fine delay byte 3
  AGESA_IDS_NV_WRBYTEDLG3,                                    ///< 0x00BC                                                           Set the write data gross delay byte 3
  AGESA_IDS_NV_WRBYTEDL4,                                     ///< 0x00BD                                                            Set the write data fine delay byte 4
  AGESA_IDS_NV_WRBYTEDLG4,                                    ///< 0x00BE                                                           Set the write data gross delay byte 4
  AGESA_IDS_NV_WRBYTEDL5,                                     ///< 0x00BF                                                            Set the write data fine delay byte 5
  AGESA_IDS_NV_WRBYTEDLG5,                                    ///< 0x00C0                                                           Set the write data gross delay byte 5
  AGESA_IDS_NV_WRBYTEDL6,                                     ///< 0x00C1                                                            Set the write data fine delay byte 6
  AGESA_IDS_NV_WRBYTEDLG6,                                    ///< 0x00C2                                                           Set the write data gross delay byte 6
  AGESA_IDS_NV_WRBYTEDL7,                                     ///< 0x00C3                                                            Set the write data fine delay byte 7
  AGESA_IDS_NV_WRBYTEDLG7,                                    ///< 0x00C4                                                           Set the write data gross delay byte 7
  AGESA_IDS_NV_WRDTECCDL,                                     ///< 0x00C5                                                               Set the write data ECC fine delay
  AGESA_IDS_NV_WRDTECCDLG,                                    ///< 0x00C6                                                              Set the write data ECC gross delay
  AGESA_IDS_NV_RDDQSBTDL0,                                    ///< 0x00C7                                                                   Set the Read DQS Byte 0 Delay
  AGESA_IDS_NV_RDDQSBTDL1,                                    ///< 0x00C8                                                                   Set the Read DQS Byte 1 Delay
  AGESA_IDS_NV_RDDQSBTDL2,                                    ///< 0x00C9                                                                   Set the Read DQS Byte 2 Delay
  AGESA_IDS_NV_RDDQSBTDL3,                                    ///< 0x00CA                                                                   Set the Read DQS Byte 3 Delay
  AGESA_IDS_NV_RDDQSBTDL4,                                    ///< 0x00CB                                                                   Set the Read DQS Byte 4 Delay
  AGESA_IDS_NV_RDDQSBTDL5,                                    ///< 0x00CC                                                                   Set the Read DQS Byte 5 Delay
  AGESA_IDS_NV_RDDQSBTDL6,                                    ///< 0x00CD                                                                   Set the Read DQS Byte 6 Delay
  AGESA_IDS_NV_RDDQSBTDL7,                                    ///< 0x00CE                                                                   Set the Read DQS Byte 7 Delay
  AGESA_IDS_NV_RDDQSECCDL,                                    ///< 0x00CF                                                                      Set the Read DQS ECC Delay
  AGESA_IDS_NV_CKEFNDL,                                       ///< 0x00D0                                                                          Set the CKE Fine Delay
  AGESA_IDS_NV_CKESETUPTIME,                                  ///< 0x00D1                                                                          Set the CKE Setup Time
  AGESA_IDS_NV_COFNDL,                                        ///< 0x00D2                                                                       Set the CS/ODT Fine Delay
  AGESA_IDS_NV_COSETUPTIME,                                   ///< 0x00D3                                                                       Set the CS/ODT Setup Time
  AGESA_IDS_NV_ADDRCMDDNDL,                                   ///< 0x00D4                                                                     Set the Addr/Cmd Fine Delay
  AGESA_IDS_NV_ADDRCMDSETUPTIME,                              ///< 0x00D5                                                                     Set the Addr/Cmd Setup Time

  AGESA_IDS_NV_MEMDQSWRBYTE0CTL,                              ///< 0x00D6                                                                       Write Data Byte 0 Control
  AGESA_IDS_NV_MEMDQSWRBYTE1CTL,                              ///< 0x00D7                                                                       Write Data Byte 1 Control
  AGESA_IDS_NV_MEMDQSWRBYTE2CTL,                              ///< 0x00D8                                                                       Write Data Byte 2 Control
  AGESA_IDS_NV_MEMDQSWRBYTE3CTL,                              ///< 0x00D9                                                                       Write Data Byte 3 Control
  AGESA_IDS_NV_MEMDQSWRBYTE4CTL,                              ///< 0x00DA                                                                       Write Data Byte 4 Control
  AGESA_IDS_NV_MEMDQSWRBYTE5CTL,                              ///< 0x00DB                                                                       Write Data Byte 5 Control
  AGESA_IDS_NV_MEMDQSWRBYTE6CTL,                              ///< 0x00DC                                                                       Write Data Byte 6 Control
  AGESA_IDS_NV_MEMDQSWRBYTE7CTL,                              ///< 0x00DD                                                                       Write Data Byte 7 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT0CTL,                             ///< 0x00DE                                                                         Read DQS Byte 0 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT1CTL,                             ///< 0x00DF                                                                         Read DQS Byte 1 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT2CTL,                             ///< 0x00E0                                                                         Read DQS Byte 2 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT3CTL,                             ///< 0x00E1                                                                         Read DQS Byte 3 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT4CTL,                             ///< 0x00E2                                                                         Read DQS Byte 4 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT5CTL,                             ///< 0x00E3                                                                         Read DQS Byte 5 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT6CTL,                             ///< 0x00E4                                                                         Read DQS Byte 6 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT7CTL,                             ///< 0x00E5                                                                         Read DQS Byte 7 Control
  AGESA_IDS_NV_MEMDQSCKEFINEDELAYCTL,                         ///< 0x00E6                                                                          CKE Fine Delay Control
  AGESA_IDS_NV_MEMDQSCSODTFINEDELAYCTL,                       ///< 0x00E7                                                                       CS/ODT Fine Delay Control
  AGESA_IDS_NV_MEMDQSADDRCMDFINEDELAYCTL,                     ///< 0x00E8                                                                     Addr/Cmd Fine Delay Control


  AGESA_IDS_NV_ALWAYS_USE_ACTIVE_DQS_TRAINING,                ///< 0x00E9                                       Enable or disable Read and Write DQS Training data output

  AGESA_IDS_NV_CPUDEEMPHASIS,                                 ///< 0x00EA                                                                                   DcvDeemphasis
  AGESA_IDS_NV_NBDEEMPHASIS,                                  ///< 0x00EB                                                                              ReceiverDeemphasis

  AGESA_IDS_NV_REGISTER1D4L,                                  ///< 0x00EC                                                                                    F3x1D4[15:0]
  AGESA_IDS_NV_REGISTER1D4H,                                  ///< 0x00ED                                                                                   F3x1D4[31:16]
  AGESA_IDS_NV_REGISTER1D8L,                                  ///< 0x00EE                                                                                    F3x1D8[15:0]
  AGESA_IDS_NV_REGISTER1D8H,                                  ///< 0x00EF                                                                                   F3x1D8[23:16]

  AGESA_IDS_NV_GENREG1E,                                      ///< 0x00F0                                                             Enable or disable register access A
  AGESA_IDS_NV_GENREG1T,                                      ///< 0x00F1                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG1W,                                      ///< 0x00F2                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG1PCIANODES,                              ///< 0x00F3                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG1MSRACORES,                              ///< 0x00F4                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG1MSRNODE,                                ///< 0x00F5                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG1MSRCORE,                                ///< 0x00F6                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG1BUS,                                    ///< 0x00F7                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG1DEVICE,                                 ///< 0x00F8                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG1FUNC,                                   ///< 0x00F9                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG1OFFSET,                                 ///< 0x00FA                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH1,                                      ///< 0x00FB                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW1,                                       ///< 0x00FC                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG1ORHIGHEST,                              ///< 0x00FD                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG1ORHIGHER,                               ///< 0x00FE                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG1ORLOWER,                                ///< 0x00FF                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG1ORLOWEST,                               ///< 0x0100                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG1ANDHIGHEST,                             ///< 0x0101                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG1ANDHIGHER,                              ///< 0x0102                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG1ANDLOWER,                               ///< 0x0103                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG1ANDLOWEST,                              ///< 0x0104                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG1MMIOBASEHIGH,                           ///< 0x0105                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG1MMIOBASELOW,                            ///< 0x0106                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG1PCI_INDIRECT_OFF_HIGH,                  ///< 0x0107                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG1PCI_INDIRECT_OFF_LOW,                   ///< 0x0108                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG1PCI_INDIRECT_WEBIT,                     ///< 0x0109                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG1GMMX_OFF_HIGH,                          ///< 0x010A                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG1GMMX_OFF_LOW,                           ///< 0x010B                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG2E,                                      ///< 0x010C                                                             Enable or disable register access B
  AGESA_IDS_NV_GENREG2T,                                      ///< 0x010D                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG2W,                                      ///< 0x010E                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG2PCIANODES,                              ///< 0x010F                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG2MSRACORES,                              ///< 0x0110                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG2MSRNODE,                                ///< 0x0111                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG2MSRCORE,                                ///< 0x0112                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG2BUS,                                    ///< 0x0113                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG2DEVICE,                                 ///< 0x0114                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG2FUNC,                                   ///< 0x0115                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG2OFFSET,                                 ///< 0x0116                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH2,                                      ///< 0x0117                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW2,                                       ///< 0x0118                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG2ORHIGHEST,                              ///< 0x0119                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG2ORHIGHER,                               ///< 0x011A                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG2ORLOWER,                                ///< 0x011B                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG2ORLOWEST,                               ///< 0x011C                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG2ANDHIGHEST,                             ///< 0x011D                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG2ANDHIGHER,                              ///< 0x011E                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG2ANDLOWER,                               ///< 0x011F                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG2ANDLOWEST,                              ///< 0x0120                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG2MMIOBASEHIGH,                           ///< 0x0121                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG2MMIOBASELOW,                            ///< 0x0122                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG2PCI_INDIRECT_OFF_HIGH,                  ///< 0x0123                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG2PCI_INDIRECT_OFF_LOW,                   ///< 0x0124                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG2PCI_INDIRECT_WEBIT,                     ///< 0x0125                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG2GMMX_OFF_HIGH,                          ///< 0x0126                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG2GMMX_OFF_LOW,                           ///< 0x0127                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG3E,                                      ///< 0x0128                                                             Enable or disable register access C
  AGESA_IDS_NV_GENREG3T,                                      ///< 0x0129                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG3W,                                      ///< 0x012A                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG3PCIANODES,                              ///< 0x012B                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG3MSRACORES,                              ///< 0x012C                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG3MSRNODE,                                ///< 0x012D                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG3MSRCORE,                                ///< 0x012E                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG3BUS,                                    ///< 0x012F                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG3DEVICE,                                 ///< 0x0130                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG3FUNC,                                   ///< 0x0131                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG3OFFSET,                                 ///< 0x0132                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH3,                                      ///< 0x0133                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW3,                                       ///< 0x0134                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG3ORHIGHEST,                              ///< 0x0135                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG3ORHIGHER,                               ///< 0x0136                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG3ORLOWER,                                ///< 0x0137                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG3ORLOWEST,                               ///< 0x0138                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG3ANDHIGHEST,                             ///< 0x0139                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG3ANDHIGHER,                              ///< 0x013A                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG3ANDLOWER,                               ///< 0x013B                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG3ANDLOWEST,                              ///< 0x013C                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG3MMIOBASEHIGH,                           ///< 0x013D                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG3MMIOBASELOW,                            ///< 0x013E                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG3PCI_INDIRECT_OFF_HIGH,                  ///< 0x013F                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG3PCI_INDIRECT_OFF_LOW,                   ///< 0x0140                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG3PCI_INDIRECT_WEBIT,                     ///< 0x0141                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG3GMMX_OFF_HIGH,                          ///< 0x0142                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG3GMMX_OFF_LOW,                           ///< 0x0143                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG4E,                                      ///< 0x0144                                                             Enable or disable register access D
  AGESA_IDS_NV_GENREG4T,                                      ///< 0x0145                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG4W,                                      ///< 0x0146                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG4PCIANODES,                              ///< 0x0147                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG4MSRACORES,                              ///< 0x0148                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG4MSRNODE,                                ///< 0x0149                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG4MSRCORE,                                ///< 0x014A                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG4BUS,                                    ///< 0x014B                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG4DEVICE,                                 ///< 0x014C                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG4FUNC,                                   ///< 0x014D                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG4OFFSET,                                 ///< 0x014E                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH4,                                      ///< 0x014F                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW4,                                       ///< 0x0150                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG4ORHIGHEST,                              ///< 0x0151                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG4ORHIGHER,                               ///< 0x0152                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG4ORLOWER,                                ///< 0x0153                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG4ORLOWEST,                               ///< 0x0154                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG4ANDHIGHEST,                             ///< 0x0155                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG4ANDHIGHER,                              ///< 0x0156                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG4ANDLOWER,                               ///< 0x0157                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG4ANDLOWEST,                              ///< 0x0158                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG4MMIOBASEHIGH,                           ///< 0x0159                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG4MMIOBASELOW,                            ///< 0x015A                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG4PCI_INDIRECT_OFF_HIGH,                  ///< 0x015B                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG4PCI_INDIRECT_OFF_LOW,                   ///< 0x015C                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG4PCI_INDIRECT_WEBIT,                     ///< 0x015D                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG4GMMX_OFF_HIGH,                          ///< 0x015E                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG4GMMX_OFF_LOW,                           ///< 0x015F                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG5E,                                      ///< 0x0160                                                             Enable or disable register access E
  AGESA_IDS_NV_GENREG5T,                                      ///< 0x0161                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG5W,                                      ///< 0x0162                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG5PCIANODES,                              ///< 0x0163                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG5MSRACORES,                              ///< 0x0164                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG5MSRNODE,                                ///< 0x0165                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG5MSRCORE,                                ///< 0x0166                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG5BUS,                                    ///< 0x0167                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG5DEVICE,                                 ///< 0x0168                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG5FUNC,                                   ///< 0x0169                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG5OFFSET,                                 ///< 0x016A                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH5,                                      ///< 0x016B                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW5,                                       ///< 0x016C                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG5ORHIGHEST,                              ///< 0x016D                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG5ORHIGHER,                               ///< 0x016E                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG5ORLOWER,                                ///< 0x016F                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG5ORLOWEST,                               ///< 0x0170                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG5ANDHIGHEST,                             ///< 0x0171                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG5ANDHIGHER,                              ///< 0x0172                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG5ANDLOWER,                               ///< 0x0173                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG5ANDLOWEST,                              ///< 0x0174                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG5MMIOBASEHIGH,                           ///< 0x0175                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG5MMIOBASELOW,                            ///< 0x0176                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG5PCI_INDIRECT_OFF_HIGH,                  ///< 0x0177                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG5PCI_INDIRECT_OFF_LOW,                   ///< 0x0178                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG5PCI_INDIRECT_WEBIT,                     ///< 0x0179                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG5GMMX_OFF_HIGH,                          ///< 0x017A                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG5GMMX_OFF_LOW,                           ///< 0x017B                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG6E,                                      ///< 0x017C                                                             Enable or disable register access F
  AGESA_IDS_NV_GENREG6T,                                      ///< 0x017D                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG6W,                                      ///< 0x017E                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG6PCIANODES,                              ///< 0x017F                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG6MSRACORES,                              ///< 0x0180                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG6MSRNODE,                                ///< 0x0181                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG6MSRCORE,                                ///< 0x0182                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG6BUS,                                    ///< 0x0183                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG6DEVICE,                                 ///< 0x0184                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG6FUNC,                                   ///< 0x0185                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG6OFFSET,                                 ///< 0x0186                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH6,                                      ///< 0x0187                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW6,                                       ///< 0x0188                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG6ORHIGHEST,                              ///< 0x0189                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG6ORHIGHER,                               ///< 0x018A                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG6ORLOWER,                                ///< 0x018B                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG6ORLOWEST,                               ///< 0x018C                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG6ANDHIGHEST,                             ///< 0x018D                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG6ANDHIGHER,                              ///< 0x018E                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG6ANDLOWER,                               ///< 0x018F                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG6ANDLOWEST,                              ///< 0x0190                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG6MMIOBASEHIGH,                           ///< 0x0191                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG6MMIOBASELOW,                            ///< 0x0192                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG6PCI_INDIRECT_OFF_HIGH,                  ///< 0x0193                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG6PCI_INDIRECT_OFF_LOW,                   ///< 0x0194                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG6PCI_INDIRECT_WEBIT,                     ///< 0x0195                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG6GMMX_OFF_HIGH,                          ///< 0x0196                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG6GMMX_OFF_LOW,                           ///< 0x0197                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG7E,                                      ///< 0x0198                                                             Enable or disable register access G
  AGESA_IDS_NV_GENREG7T,                                      ///< 0x0199                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG7W,                                      ///< 0x019A                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG7PCIANODES,                              ///< 0x019B                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG7MSRACORES,                              ///< 0x019C                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG7MSRNODE,                                ///< 0x019D                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG7MSRCORE,                                ///< 0x019E                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG7BUS,                                    ///< 0x019F                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG7DEVICE,                                 ///< 0x01A0                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG7FUNC,                                   ///< 0x01A1                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG7OFFSET,                                 ///< 0x01A2                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH7,                                      ///< 0x01A3                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW7,                                       ///< 0x01A4                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG7ORHIGHEST,                              ///< 0x01A5                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG7ORHIGHER,                               ///< 0x01A6                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG7ORLOWER,                                ///< 0x01A7                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG7ORLOWEST,                               ///< 0x01A8                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG7ANDHIGHEST,                             ///< 0x01A9                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG7ANDHIGHER,                              ///< 0x01AA                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG7ANDLOWER,                               ///< 0x01AB                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG7ANDLOWEST,                              ///< 0x01AC                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG7MMIOBASEHIGH,                           ///< 0x01AD                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG7MMIOBASELOW,                            ///< 0x01AE                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG7PCI_INDIRECT_OFF_HIGH,                  ///< 0x01AF                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG7PCI_INDIRECT_OFF_LOW,                   ///< 0x01B0                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG7PCI_INDIRECT_WEBIT,                     ///< 0x01B1                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG7GMMX_OFF_HIGH,                          ///< 0x01B2                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG7GMMX_OFF_LOW,                           ///< 0x01B3                                                                                 GMMX offset low

  AGESA_IDS_NV_GENREG8E,                                      ///< 0x01B4                                                             Enable or disable register access H
  AGESA_IDS_NV_GENREG8T,                                      ///< 0x01B5                                                              Select the type of register to set
  AGESA_IDS_NV_GENREG8W,                                      ///< 0x01B6                                                              When to modify the value specified
  AGESA_IDS_NV_GENREG8PCIANODES,                              ///< 0x01B7                                 The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG8MSRACORES,                              ///< 0x01B8                                                  The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG8MSRNODE,                                ///< 0x01B9                                            The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG8MSRCORE,                                ///< 0x01BA                                           The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG8BUS,                                    ///< 0x01BB                                                         Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG8DEVICE,                                 ///< 0x01BC                                                      Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG8FUNC,                                   ///< 0x01BD                                                    Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG8OFFSET,                                 ///< 0x01BE                                                             Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH8,                                      ///< 0x01BF                                                       Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW8,                                       ///< 0x01C0                                                        Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG8ORHIGHEST,                              ///< 0x01C1                                                              The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG8ORHIGHER,                               ///< 0x01C2                                                               Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG8ORLOWER,                                ///< 0x01C3                                                               Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG8ORLOWEST,                               ///< 0x01C4                                                               The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG8ANDHIGHEST,                             ///< 0x01C5                                                             The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG8ANDHIGHER,                              ///< 0x01C6                                                              Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG8ANDLOWER,                               ///< 0x01C7                                                              Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG8ANDLOWEST,                              ///< 0x01C8                                                              The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG8MMIOBASEHIGH,                           ///< 0x01C9                                                                                  MMIO base High
  AGESA_IDS_NV_GENREG8MMIOBASELOW,                            ///< 0x01CA                                                                                   MMIO base low
  AGESA_IDS_NV_GENREG8PCI_INDIRECT_OFF_HIGH,                  ///< 0x01CB                                                                        PCI indirect Offset High
  AGESA_IDS_NV_GENREG8PCI_INDIRECT_OFF_LOW,                   ///< 0x01CC                                                                         PCI indirect Offset Low
  AGESA_IDS_NV_GENREG8PCI_INDIRECT_WEBIT,                     ///< 0x01CD                                                                   PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG8GMMX_OFF_HIGH,                          ///< 0x01CE                                                                                GMMX offset High
  AGESA_IDS_NV_GENREG8GMMX_OFF_LOW,                           ///< 0x01CF                                                                                 GMMX offset low

  AGESA_IDS_NV_BVM_IMAGE_BASE_HIGHEST,                        ///< 0x01D0                                                     Bits 48 to 63 of the BVM image base address
  AGESA_IDS_NV_BVM_IMAGE_BASE_HIGHER,                         ///< 0x01D1                                                     Bits 32 to 47 of the BVM image base address
  AGESA_IDS_NV_BVM_IMAGE_BASE_LOWER,                          ///< 0x01D2                                                     Bits 16 to 31 of the BVM image base address
  AGESA_IDS_NV_BVM_IMAGE_BASE_LOWEST,                         ///< 0x01D3                                                     Bits  0 to 15 of the BVM image base address
  AGESA_IDS_NV_IGNORE_MEM_ERR,                                ///< 0x01D4                                                     Whether to ignore memory error and continue

  AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_EN,                       ///< 0x01D5                                                                                ORB Dynamic Wake
  AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_HYSTERESIS,               ///< 0x01D6                                                                             ORB Wake Hysteresis
  AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_HYSTERESIS_VAL,           ///< 0x01D7                                                                       ORB Wake Hysteresis Value

  AGESA_IDS_NV_HDTBREAKPOINT_BFDRAMINIT,                      ///< 0x01D8                                                               hdtout breakpoint before Draminit
  AGESA_IDS_NV_HDTBREAKPOINT_BFMEMCLR,                        ///< 0x01D9                                                         hdtout breakpoint before set Memclr bit
  AGESA_IDS_NV_GNB_ABM_SUPPORT,                               ///< 0x01DA                                                                                     ABM support
  AGESA_IDS_NV_GNB_LCD_BACKLIGHT_CONTROL,                     ///< 0x01DB                                                      The PWM frequency to LCD backlight control
  AGESA_IDS_NV_GNB_LCD_BACKLIGHT,                             ///< 0x01DC                                                              The PWM frequency to LCD backlight
  AGESA_IDS_NV_GNB_DRR_SUPPORT,                               ///< 0x01DD                                                                                     DRR support
  AGESA_IDS_NV_GNB_PLATFORM_TYPE,                             ///< 0x01DE                                                                                   platform type
  AGESA_IDS_NV_GNB_UMA_STEERING,                              ///< 0x01DF                                                                                    UMA steering
  AGESA_IDS_NV_GNB_PRIMARY_GRAPHICS_ADAPTOR,                  ///< 0x01E0                                                                        Primary Graphics Adapter

  AGESA_IDS_NV_GNB_LCLK_DEEPSLEEP,                            ///< 0x01E1                                                                                 LCLK deep sleep

  AGESA_IDS_NV_GNB_LCLK_DPM,                                  ///< 0x01E2                                                                                        LCLK DPM

  AGESA_IDS_NV_GNB_SMU_LCLK_GATING,                           ///< 0x01E3                                                                                 SMU LCLK gating
  AGESA_IDS_NV_GNB_SMU_SCLK_GATING,                           ///< 0x01E4                                                                                 SMU SCLK gating
  AGESA_IDS_NV_GNB_ORB_LCLK_GATING,                           ///< 0x01E5                                                                                 ORB LCLK gating
  AGESA_IDS_NV_GNB_ORB_SCLK_GATING,                           ///< 0x01E6                                                                                 ORB SCLK gating
  AGESA_IDS_NV_GNB_IOC_LCLK_GATING,                           ///< 0x01E7                                                                                 IOC LCLK gating
  AGESA_IDS_NV_GNB_IOC_SCLK_GATING,                           ///< 0x01E8                                                                                 IOC SCLK gating
  AGESA_IDS_NV_GNB_BIF_SCLK_GATING,                           ///< 0x01E9                                                                                 BIF SCLK gating
  AGESA_IDS_NV_GNB_GMC_SCLK_GATING_CTRL,                      ///< 0x01EA                                                                         Control GMC SCLK gating
  AGESA_IDS_NV_GNB_GMC_SCLK_GATING,                           ///< 0x01EB                                                                                 GMC SCLK gating
  AGESA_IDS_NV_GNB_DCE_DISPCLK_GATING,                        ///< 0x01EC                                                                        Dce display clock gating
  AGESA_IDS_NV_GNB_DCE_SCLK_GATING,                           ///< 0x01ED                                                                               Dce Sclock gating
  AGESA_IDS_NV_GNB_ORB_CLK_GATING,                            ///< 0x01EE                                                                                  ORB CLK gating
  AGESA_IDS_NV_GNB_NB_IOMMU_L1_LCLKGATING,                    ///< 0x01EF                                                                            IOMMU L1 LCLK gating
  AGESA_IDS_NV_GNB_NB_IOMMU_L2_LCLKGATING,                    ///< 0x01F0                                                                            IOMMU L2 LCLK gating
  AGESA_IDS_NV_GNB_PCIE_CORE_POWER_GATING,                    ///< 0x01F1                                                                      GNB PCIe Core Power Gating
  AGESA_IDS_NV_GNB_PCIE_CORE_POWER_GATING_DATA,               ///< 0x01F2                                                                 GNB PCIe Core Power Gating Data
  AGESA_IDS_NV_GNB_PCIE_PHY_POWER_GATING,                     ///< 0x01F3                                                                  GNB PCIe Phy Power Gating Data

  AGESA_IDS_NV_GNB_FORCE_CABLESAFE,                           ///< 0x01F4                                                                                Force Cable Safe
  AGESA_IDS_NV_GNB_ALTVDDNB,                                  ///< 0x01F5                                                                                        AltVddNb
  AGESA_IDS_NV_GNB_PMM_BAPMCTL,                               ///< 0x01F6                                                                                    BAPM Control
  AGESA_IDS_NV_GNB_PMM_BAPMSMUCTL,                            ///< 0x01F7                                                                                BAPM SMU Control
  AGESA_IDS_NV_GNB_PMM_TDCTRACKING,                           ///< 0x01F8                                                                                    TDC Tracking
  AGESA_IDS_NV_GNB_PMM_PACKAGEPOWERTRACKING,                  ///< 0x01F9                                                                          Package Power Tracking
  AGESA_IDS_NV_GNB_PMM_NBDPMCTL,                              ///< 0x01FA                                                                                   NBDPM Control
  AGESA_IDS_NV_GNB_PMM_GNBSWTJOFFSETCTL,                      ///< 0x01FB                                                                           GNBSWTJOFFSET Control
  AGESA_IDS_NV_GNB_PMM_GNBSWTJOFFSET,                         ///< 0x01FC                                                                                   GNBSWTJOFFSET
  AGESA_IDS_NV_GNB_PMM_LOADLINEOPTIMIZATION,                  ///< 0x01FD                                                                   Loadline Optimization Control
  AGESA_IDS_NV_GNB_PMM_SMUSCLKCLOCKGATING,                    ///< 0x01FE                                                                           SMU SCLK Clock Gating
  AGESA_IDS_NV_GNB_PMM_GNBPMMLHTC,                            ///< 0x01FF                                                                                            LHTC
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTCTL,                        ///< 0x0200                                                                            Hybrid Boost Control
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDHYSTCTL,           ///< 0x0201                                                              Hybrid Boost Threshold hystControl
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDHYST,              ///< 0x0202                                                                     Hybrid Boost Threshold hyst
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDOFFSETCTL,         ///< 0x0203                                                           Hybrid Boost Threshold offset Control
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDOFFSET,            ///< 0x0204                                                                   Hybrid Boost Threshold offset
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTSCALARCTL,                  ///< 0x0205                                                                     Hybrid Boost Scalar Control
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTSCALAR,                     ///< 0x0206                                                                             Hybrid Boost Scalar
  AGESA_IDS_NV_GNB_PMM_PCIEPHYISOLATION,                      ///< 0x0207                                                                              Pcie Phy Isolation
  AGESA_IDS_NV_GNB_PMM_NATIVEGEN1PLL,                         ///< 0x0208                                                                                 Native Gen1 PLL

  AGESA_IDS_NV_GNB_NB_SWDEVICEIDREMAPPING,                    ///< 0x0209                                                                          SW Device ID Remapping
  AGESA_IDS_NV_GNB_NB_PBR0DEVFNMAP,                           ///< 0x020A                                                                                   PBr0 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR1DEVFNMAP,                           ///< 0x020B                                                                                   PBr1 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR2DEVFNMAP,                           ///< 0x020C                                                                                   PBr2 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR3DEVFNMAP,                           ///< 0x020D                                                                                   PBr3 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR4DEVFNMAP,                           ///< 0x020E                                                                                   PBr4 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR5DEVFNMAP,                           ///< 0x020F                                                                                   PBr5 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR6DEVFNMAP,                           ///< 0x0210                                                                                   PBr6 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR7DEVFNMAP,                           ///< 0x0211                                                                                   PBr7 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR8DEVFNMAP,                           ///< 0x0212                                                                                   PBr8 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR9DEVFNMAP,                           ///< 0x0213                                                                                   PBr9 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR10DEVFNMAP,                          ///< 0x0214                                                                                  PBr10 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR11DEVFNMAP,                          ///< 0x0215                                                                                  PBr11 DevFnMap

  AGESA_IDS_NV_MAIN_PLL_CON,                                  ///< 0x0216                                                                          00 - Auto, 01 - Manual
  AGESA_IDS_NV_MAIN_PLL_FID_EN,                               ///< 0x0217                                                                            MainPllFreqId Enable
  AGESA_IDS_NV_MAIN_PLL_FID,                                  ///< 0x0218                                                                                   MainPllFreqId

  AGESA_IDS_NV_CUSTOM_NB_PSTATE,                              ///< 0x0219                                                                          00 - Auto, 01 - Manual
  AGESA_IDS_NV_NUMOFNBPSTATES,                                ///< 0x021A                                                                            Number of NB PStates
  AGESA_IDS_NV_CUSTOM_NB_P0_DIV_CTRL,                         ///< 0x021B                                                                        00 - Manual, 01 - MCLK/2
  AGESA_IDS_NV_CUSTOM_NB_P1_DIV_CTRL,                         ///< 0x021C                                                                        00 - Manual, 01 - MCLK/2
  AGESA_IDS_NV_FORCE_NB_PSTATE,                               ///< 0x021D                                                                 00 - NBP0  01 - NBP1  02 - Auto

  AGESA_IDS_NV_CPB_GNB_EN,                                    ///< 0x021E                                                               Graphics northbridge boost enable
  AGESA_IDS_NV_CPB_THROTTLE_CORE,                             ///< 0x021F                                                                               Throttle One Core
  AGESA_IDS_NV_CPB_THROTTLE_CORE_TIMER,                       ///< 0x0220                                                                             Throttle Core Timer
  AGESA_IDS_NV_CPB_EN_ALLCORES,                               ///< 0x0221                                                                          Boost enable all cores
  AGESA_IDS_NV_CPB_PLL_RANGE_MODE,                            ///< 0x0222                                                                                  Pll Range Mode
  AGESA_IDS_NV_CPB_BOOST_SOURCE,                              ///< 0x0223                                                                                    Boost source
  AGESA_IDS_NV_CPB_NUM_BOOST_CTRL,                            ///< 0x0224                                                                Number of boosted states Control
  AGESA_IDS_NV_CPB_NUM_BOOST,                                 ///< 0x0225                                                                        Number of boosted states
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE0,                           ///< 0x0226                                                         Core Performance Boost disable on core0
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE1,                           ///< 0x0227                                                         Core Performance Boost disable on core1
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE2,                           ///< 0x0228                                                         Core Performance Boost disable on core2
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE3,                           ///< 0x0229                                                         Core Performance Boost disable on core3

  AGESA_IDS_NV_CPBMASTEREN,                                   ///< 0x022A                                                            Core performance boost master enable
  AGESA_IDS_NV_C_STATE_BOOSTCTL,                              ///< 0x022B                                                                           C-state Boost Control
  AGESA_IDS_NV_CSTATECNT,                                     ///< 0x022C                                                                                   C-state count
  AGESA_IDS_NV_CSTATEBOOST,                                   ///< 0x022D                                                                                     CstateBoost
  AGESA_IDS_NV_APMBOOSTSTATES,                                ///< 0x022E                                                                            APM TDP Boost States
  AGESA_IDS_NV_TDPCTL,                                        ///< 0x022F                                                                                 APM TDP Control
  AGESA_IDS_NV_NODETDPLIMITEN,                                ///< 0x0230                                                                             NodeTdpLimit Enable
  AGESA_IDS_NV_APMLSWTDPLIMITEN,                              ///< 0x0231                                                                           ApmlSwTdpLimit Enable

  AGESA_IDS_NV_CC6CTL,                                        ///< 0x0232                                                                                     CC6 Control

  AGESA_IDS_NV_FASTSLAMTIMEDOWN,                              ///< 0x0233                                                                             Fast Slam Time Down

  AGESA_IDS_NV_NB_PSTATE_CTRL_DIS,                            ///< 0x0234                                                                      NB P-state control disable

  AGESA_IDS_NV_GNB_GMC_POWG_GATING_CTRL,                      ///< 0x0235                                                                              Control GMC gating
  AGESA_IDS_NV_GNB_GMC_POWG_GATING,                           ///< 0x0236                                                                                GMC power gating

  AGESA_IDS_NV_DRAMTERMDYN,                                   ///< 0x0237                                                                                     DramTermDyn
  AGESA_IDS_NV_DRAMTERM,                                      ///< 0x0238                                                                                        DramTerm
  AGESA_IDS_NV_DATATXFIFOWRDLY,                               ///< 0x0239                                                                  Data transmit FIFO write delay
  AGESA_IDS_NV_FASTSELFREFENTRYDIS,                           ///< 0x023A                                                                 Fast self refresh entry disable

  AGESA_IDS_NV_LOWPOWERDRVSTRENGTHEN,                         ///< 0x023B                                                                 Low power drive strength enable
  AGESA_IDS_NV_CC6DSMWORKAROUND,                              ///< 0x023C                                                                                CC6DSMworkaround
  AGESA_IDS_NV_ODTSEN,                                        ///< 0x023D                                                               On DIMM temperature sensor Enable

  AGESA_IDS_NV_HTT0TIMECTL,                                   ///< 0x023E                                                                               HT T0Time Control
  AGESA_IDS_NV_HTT0TIME,                                      ///< 0x023F                                                                  This specifies training 0 time
  AGESA_IDS_NV_ATMMODEEN,                                     ///< 0x0240                                   This specifies accelerated transition to modified mode enable
  AGESA_IDS_NV_VICTIMDISTRIBUTIONMODE,                        ///< 0x0241                              This specifies cHT VicBlk and VicBlkClean packet distribution mode
  AGESA_IDS_NV_HTFULLT0TIMECTL,                               ///< 0x0242                                                                           HT FullT0Time Control
  AGESA_IDS_NV_HTFULLT0TIME,                                  ///< 0x0243                                                                  This specifies training 0 time
  AGESA_IDS_NV_HTPOWERSAVINGSMODE,                            ///< 0x0244                                                            This specifies HT Power Savings Mode
  AGESA_IDS_NV_TXLS23CLKGATEEN,                               ///< 0x0245                                                      This specifies LS2/LS3 clock gating enable
  AGESA_IDS_NV_LINKPHYDLLREGCTL,                              ///< 0x0246                                                                  Link Phy DLL registers Control
  AGESA_IDS_NV_LINKPHYDLLREGHI,                               ///< 0x0247                                                                     Link Phy DLL registers High
  AGESA_IDS_NV_LINKPHYDLLREGLO,                               ///< 0x0248                                                                      Link Phy DLL registers Low
  AGESA_IDS_NV_TXSSBUSPWRSAVEEN,                              ///< 0x0249                                               Transmit source synchronous bus power save Enable

  AGESA_IDS_NV_INTLINKPHYOVERRIDE,                            ///< 0x024A                                                                      Internal Link Phy Override
  AGESA_IDS_NV_DFEINT,                                        ///< 0x024B                                                              Set Decision feedback equalization
  AGESA_IDS_NV_DFEVOLTAGEINT,                                 ///< 0x024C                                                                        DFE offset voltage level
  AGESA_IDS_NV_DCVINT,                                        ///< 0x024D                                                 Specifies the peak singleended DC voltage level
  AGESA_IDS_NV_DEEMPHASISINT1,                                ///< 0x024E                                                  Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_DEEMPHASISINT2,                                ///< 0x024F                                                  Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_EXTLINKPHYOVERRIDE,                            ///< 0x0250                                                                      External Link Phy Override
  AGESA_IDS_NV_DFEEXT,                                        ///< 0x0251                                                              Set Decision feedback equalization
  AGESA_IDS_NV_DFEVOLTAGEEXT,                                 ///< 0x0252                                                                        DFE offset voltage level
  AGESA_IDS_NV_DCVEXT,                                        ///< 0x0253                                                 Specifies the peak singleended DC voltage level
  AGESA_IDS_NV_DEEMPHASISEXT1,                                ///< 0x0254                                                  Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_DEEMPHASISEXT2,                                ///< 0x0255                                                  Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_LINKFIFOP0,                                    ///< 0x0256                                                     Set the Link FIFO Read Pointer for P0 state
  AGESA_IDS_NV_P0XMTRDPTR,                                    ///< 0x0257                                    This specifies transmit FIFO read pointer for P0 power state
  AGESA_IDS_NV_P0RCVRDPTR,                                    ///< 0x0258                                     This specifies receive FIFO read pointer for P0 power state
  AGESA_IDS_NV_LINKFIFOP1,                                    ///< 0x0259                                                     Set the Link FIFO Read Pointer for P1 state
  AGESA_IDS_NV_P1XMTRDPTR,                                    ///< 0x025A                                    This specifies transmit FIFO read pointer for P1 power state
  AGESA_IDS_NV_P1RCVRDPTR,                                    ///< 0x025B                                     This specifies receive FIFO read pointer for P1 power state

  AGESA_IDS_NV_ONIONDATABUFCOUNT,                             ///< 0x025C                                                                 Onion Data Buffer Count Control
  AGESA_IDS_NV_UPHIRESPCBCD,                                  ///< 0x025D                                               Upstream high priority response data buffer count
  AGESA_IDS_NV_UPHINPREQCBCD,                                 ///< 0x025E                                             Upstream high priority non-posted data buffer count
  AGESA_IDS_NV_UPHIPREQCBCD,                                  ///< 0x025F                                                 Upstream high priority posted data buffer count
  AGESA_IDS_NV_UPLORESPCBCD,                                  ///< 0x0260                                                Upstream low priority response data buffer count
  AGESA_IDS_NV_UPLONPREQCBCD,                                 ///< 0x0261                                              Upstream low priority non-posted data buffer count
  AGESA_IDS_NV_UPLOPREQCBCD,                                  ///< 0x0262                                                  Upstream low priority posted data buffer count

  AGESA_IDS_NV_ONIONCOMBUFCOUNT,                              ///< 0x0263                                     Controls the setting of Upstream ONION Command Buffer Count
  AGESA_IDS_NV_UPHIRESPCBCC,                                  ///< 0x0264                                            Upstream high priority response command buffer count
  AGESA_IDS_NV_UPHINPREQCBCC,                                 ///< 0x0265                                          Upstream high priority non-posted command buffer count
  AGESA_IDS_NV_UPHIPREQCBCC,                                  ///< 0x0266                                              Upstream high priority posted command buffer count
  AGESA_IDS_NV_UPLORESPCBCC,                                  ///< 0x0267                                             Upstream low priority response command buffer count
  AGESA_IDS_NV_UPLONPREQCBCC,                                 ///< 0x0268                                           Upstream low priority non-posted command buffer count
  AGESA_IDS_NV_UPLOPREQCBCC,                                  ///< 0x0269                                               Upstream low priority posted command buffer count

  AGESA_IDS_NV_IFQBUFALLOCATIONCON,                           ///< 0x026A                              Controls the setting of In-Flight Queue Buffer Allocation Register
  AGESA_IDS_NV_FREEPOOLBC,                                    ///< 0x026B                                                                          Free pool buffer count
  AGESA_IDS_NV_LOWPRINPBC,                                    ///< 0x026C                                                    Low priority channel non-posted buffer count
  AGESA_IDS_NV_LOWPRIPBC,                                     ///< 0x026D                                                        Low priority channel posted buffer count
  AGESA_IDS_NV_CPUBC,                                         ///< 0x026E                                                                                CPU buffer count

  AGESA_IDS_NV_IFQEXTBUFALLOCATIONCON,                        ///< 0x026F                     Controls the setting of In-Flight Queue Extended Buffer Allocation Register
  AGESA_IDS_NV_HIPRINPBC,                                     ///< 0x0270                                                   High priority channel non-posted buffer count
  AGESA_IDS_NV_HIPRIPBC,                                      ///< 0x0271                                                       High priority channel posted buffer count

  AGESA_IDS_NV_LSDSMWORDAROUND,                               ///< 0x0272                                                                  Control Erratum 463 workaround

  AGESA_IDS_NV_PDPHYPSDIS,                                    ///< 0x0273                                                               Power down phy power save Disable
  AGESA_IDS_NV_REFCHCMDMGTDIS,                                ///< 0x0274                                                      Refresh channel command management disable
  AGESA_IDS_NV_OPPWRENHDIS,                                   ///< 0x0275                                                                 Opportunistic write enh disable
  AGESA_IDS_NV_PRTLCHPDENHEN,                                 ///< 0x0276                                                           Partial channel power down enh enable
  AGESA_IDS_NV_AGGRPDEN,                                      ///< 0x0277                                                                    Aggressive power down enable
  AGESA_IDS_NV_ADDRCMDTRIEN,                                  ///< 0x0278                                                                 Address command tristate enable
  AGESA_IDS_NV_PRTLCHPDDYNDLYCTL,                             ///< 0x0279                                                Partial channel power down dynamic delay Control
  AGESA_IDS_NV_PRTLCHPDDYNDLY,                                ///< 0x027A                                                        Partial channel power down dynamic delay
  AGESA_IDS_NV_AGGRPDDELAYCTL,                                ///< 0x027B                                                             Aggressive power down delay Control
  AGESA_IDS_NV_AGGRPDDELAY,                                   ///< 0x027C                                                                     Aggressive power down delay

  AGESA_IDS_NV_PROCODTADV,                                    ///< 0x027D                                                                                 ProcOdt advance
  AGESA_IDS_NV_EXTRAPROCODTADV,                               ///< 0x027E                                                                             ExtraProcOdtAdvance

  AGESA_IDS_NV_WRODTONDURATIONCTL,                            ///< 0x027F                                                                   Write ODT on duration Control
  AGESA_IDS_NV_WRODTONDURATION,                               ///< 0x0280                                                                           Write ODT on duration
  AGESA_IDS_NV_WRODTTRNONDLYCTL,                              ///< 0x0281                                                                 Write ODT Turn On Delay Control
  AGESA_IDS_NV_WRODTTRNONDLY,                                 ///< 0x0282                                                                         Write ODT Turn On Delay
  AGESA_IDS_NV_RDODTONDURATIONCTL,                            ///< 0x0283                                                                    Read ODT On Duration Control
  AGESA_IDS_NV_RDODTONDURATION,                               ///< 0x0284                                                                            Read ODT On Duration
  AGESA_IDS_NV_RDODTTRNONDLYCTL,                              ///< 0x0285                                                                  Read ODT Turn On Delay Control
  AGESA_IDS_NV_RDODTTRNONDLY,                                 ///< 0x0286                                                                          Read ODT Turn On Delay

  AGESA_IDS_NV_CSTATECTL,                                     ///< 0x0287                                                                                 C-State Control
  AGESA_IDS_NV_NUMCSTATES,                                    ///< 0x0288                                                                   Select the Number of C-States
  AGESA_IDS_NV_CSTACT1,                                       ///< 0x0289                                                                      Set C-State Action Field 1
  AGESA_IDS_NV_CSTACT2,                                       ///< 0x028A                                                                      Set C-State Action Field 2
  AGESA_IDS_NV_CSTACT3,                                       ///< 0x028B                                                                      Set C-State Action Field 3
  AGESA_IDS_NV_ENFORCE_VDDIO,                                 ///< 0x028C                                                                                    Enfore VDDIO

  AGESA_IDS_NV_DEADLOCK_WIDGET_DIS,                           ///< 0x028D                                                                               DeadLockWidgetDis

  AGESA_IDS_NV_PCHGPDENDELAYCTL,                              ///< 0x028E                                                        Precharge Power Down Entry Delay Control
  AGESA_IDS_NV_PCHGPDENDELAY,                                 ///< 0x028F                                                                Precharge Power Down Entry Delay
  AGESA_IDS_NV_REDIRECT_SHUTDOWN_TO_HDT,                      ///< 0x0290                                                             Switch of Redirect Powerdown to HDT
  AGESA_IDS_NV_DEBUGCTRL_LBR,                                 ///< 0x0291                                                                     Switch of Debug Control Lbr
  AGESA_IDS_NV_NUMOFHIGHPERFORMANCENBPSTATE,                  ///< 0x0292                                                                     Switch of Debug Control Lbr

  AGESA_IDS_NV_FCH_NBSB_GEN2,                                 ///< 0x0293                                                                 Bus generation between NB & FCH
  AGESA_IDS_NV_FCH_SPI_BURST_WRITE,                           ///< 0x0294                                                                             FCH SPI brust write
  AGESA_IDS_NV_HTC_LOCK,                                      ///< 0x0295                                                                               Override HTC lock
  AGESA_IDS_NV_MEMPSTATEDIS,                                  ///< 0x0296                                                                            Config  mempstatedis
  AGESA_IDS_NV_DLL_DAC_MONITOR,                               ///< 0x0297                                                                                 Dll DAC Monitor
  AGESA_IDS_NV_FORCE_ASSERT,                                  ///< 0x0298                                                                                    Force Assert

  AGESA_IDS_NV_REDUCELOOPDELAY,                               ///< 0x0299                                                                                 REDUCELOOPDELAY
  AGESA_IDS_NV_DLLCSRBIASTRIM,                                ///< 0x029A                                                                                  DLLCSRBIASTRIM
  AGESA_IDS_NV_D3DBYTE,                                       ///< 0x029B                                                                                         D3DBYTE
  AGESA_IDS_NV_D3CLK,                                         ///< 0x029C                                                                                           D3CLK
  AGESA_IDS_NV_D3CSODT,                                       ///< 0x029D                                                                                         D3CSODT
  AGESA_IDS_NV_D3ABYTE2,                                      ///< 0x029E                                                                                        D3ABYTE2
  AGESA_IDS_NV_CPUNBPS0MEMPSTATE,                             ///< 0x029F                                                                          State 0 Memory P-state
  AGESA_IDS_NV_CPUNBPS1MEMPSTATE,                             ///< 0x02A0                                                                          State 1 Memory P-state
  AGESA_IDS_NV_CPUNBPS2MEMPSTATE,                             ///< 0x02A1                                                                          State 2 Memory P-state
  AGESA_IDS_NV_CPUNBPS3MEMPSTATE,                             ///< 0x02A2                                                                          State 3 Memory P-state
  AGESA_IDS_NV_MEMCTRLLEREFFARBDIS,                           ///< 0x02A3                                                                   Efficient arbitration disable
  AGESA_IDS_NV_MEMCTRLLERDISHALFNCLKPWRGATE,                  ///< 0x02A4                                                                     Disable HalfNCLK power gate
  AGESA_IDS_NV_CPUPSTSWNBPSTATELODIS,                         ///< 0x02A5                                                                 Software NB P-state low disable
  AGESA_IDS_NV_TRACEMODE,                                     ///< 0x02A6                                                                        Enable Trace buffer mode
  AGESA_IDS_NV_NCEXTLINKPHYOVERRIDE,                          ///< 0x02A7                                                                      External Link Phy Override
  AGESA_IDS_NV_NCDFEEXT,                                      ///< 0x02A8                                                              Set Decision feedback equalization
  AGESA_IDS_NV_NCDFEVOLTAGEEXT,                               ///< 0x02A9                                                                        DFE offset voltage level
  AGESA_IDS_NV_NCDCVEXT,                                      ///< 0x02AA                                                 Specifies the peak singleended DC voltage level
  AGESA_IDS_NV_NCDEEMPHASISEXT1,                              ///< 0x02AB                                                  Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_NCDEEMPHASISEXT2,                              ///< 0x02AC                                                  Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_MEMORY_POWER_POLICY,                           ///< 0x02AD                                                                             Memory power policy
  AGESA_IDS_NV_MEM_LOCKDRAM_CONFIG,                           ///< 0x02AE                                                                             Mem LockDram Config
  AGESA_IDS_NV_CPUCURRENTLIMITCTL,                            ///< 0x02AF                                                                       CPU Current Limit Control
  AGESA_IDS_NV_CPUCURRENTLIMIT,                               ///< 0x02B0                                                                               CPU Current Limit
  AGESA_IDS_NV_CPUMAXCURRENTLIMIT,                            ///< 0x02B1                                                                           CPU Max Current Limit
  AGESA_IDS_NV_NBCURRENTLIMITCTL,                             ///< 0x02B2                                                                        NB Current Limit Control
  AGESA_IDS_NV_NBCURRENTLIMIT,                                ///< 0x02B3                                                                                NB Current Limit
  AGESA_IDS_NV_NBMAXCURRENTLIMIT,                             ///< 0x02B4                                                                            NB Max Current Limit
  AGESA_IDS_NV_BEFORE_GEN2_INIT,                              ///< 0x02B5                                                                 Hdt breakpoint Before Gen2 Init
  AGESA_IDS_NV_BEFORE_GPP_TRAINING,                           ///< 0x02B6                                                              Hdt breakpoint Before Gpp training
  AGESA_IDS_NV_BEFORE_RECONFIGURATION,                        ///< 0x02B7                                                           Hdt breakpoint Before Reconfiguration
  AGESA_IDS_NV_BEFORE_GEN3_INIT,                              ///< 0x02B8                                                                 Hdt breakpoint Before Gen3 Init
  AGESA_IDS_NV_BEFORE_POWER_GATING,                           ///< 0x02B9                                                              Hdt breakpoint Before Power Gating
  AGESA_IDS_NV_NBCORE_SYNC_POINTER_ADJ_CTL,                   ///< 0x02BA                                                        NB/core sync FIFO pointer adjust Control
  AGESA_IDS_NV_NBCORE_SYNC_POINTER_ADJ,                       ///< 0x02BB                                                                NB/core sync FIFO pointer adjust
  AGESA_IDS_NV_CPUCACHEFLUSHSUCMONTMRSEL,                     ///< 0x02BC                                                        Cache Flush Success Monitor Timer Select
  AGESA_IDS_NV_CPUCACHEFLUSHSUCMONMISPREDICTACT,              ///< 0x02BD                                                   Cache Flush Success Monitor Mispredict Action
  AGESA_IDS_NV_CPUCSTCACHEFLUSHIMMONALLHLT,                   ///< 0x02BE                                                               Cache Flush Immediate on All Halt
  AGESA_IDS_NV_CPUHLTCSTINDEX,                                ///< 0x02BF                                                                               Halt CState Index
  AGESA_IDS_NV_CPUCSTMSGDIS,                                  ///< 0x02C0                                                                        CState Messaging Disable
  AGESA_IDS_NV_CPUCSTCC6PWRDWNREGEN,                          ///< 0x02C1                                                                 CC6 Power Down Regulator Enable
  AGESA_IDS_NV_CPUCSTMISCTIMERTHRESH,                         ///< 0x02C2                                                                            Misc Timer Threshold
  AGESA_IDS_NV_CPUCSTXC6CTL,                                  ///< 0x02C3                                                                                     XC6 Control
  AGESA_IDS_NV_CPUCSTXC6CACHEFLUSHMODE,                       ///< 0x02C4                                                                            XC6 Cache Flush Mode
  AGESA_IDS_NV_CPUCSTXC6MONITORTHRESH,                        ///< 0x02C5                                                                           XC6 Monitor Threshold
  AGESA_IDS_NV_CPUCSTXC6MONITORTIMERSEL,                      ///< 0x02C6                                                                        XC6 Monitor Timer Select
  AGESA_IDS_NV_CPUCSTXC6MONMISPREDICTACT,                     ///< 0x02C7                                                                   XC6 Monitor Mispredict Action
  AGESA_IDS_NV_CPUCSTCACHERINSERCTL,                          ///< 0x02C8                                                                            Cache Rinser Control
  AGESA_IDS_NV_CPUCSTDCRINSERINTERVAL,                        ///< 0x02C9                                                                              DC Rinser Interval
  AGESA_IDS_NV_CPUCSTDCRINSERDIS,                             ///< 0x02CA                                                                               DC Rinser Disable
  AGESA_IDS_NV_CPUCSTL2RINSERDIS,                             ///< 0x02CB                                                                               L2 Rinser Disable
  AGESA_IDS_NV_CPUCSTL2RINSERINTERVAL,                        ///< 0x02CC                                                                              L2 Rinser Interval
  AGESA_IDS_NV_CPUCSTL2SECTORDIRTYDIS,                        ///< 0x02CD                                                                         L2 Sector Dirty Disable
  AGESA_IDS_NV_APIC330_MASK_CONFIG,                           ///< 0x02CE                                                                              APIC330[16] Config
  AGESA_IDS_NV_tCKSRXCTL,                                     ///< 0x02CF                                                                                  tCKSRX Control
  AGESA_IDS_NV_tCKSRX,                                        ///< 0x02D0                                                                                          tCKSRX
  AGESA_IDS_NV_tCKSRECTL,                                     ///< 0x02D1                                                                                  tCKSRE Control
  AGESA_IDS_NV_tCKSRE,                                        ///< 0x02D2                                                                                          tCKSRE
  AGESA_IDS_NV_tCKESRCTL,                                     ///< 0x02D3                                                                                  tCKSRE Control
  AGESA_IDS_NV_tCKESR,                                        ///< 0x02D4                                                                                          tCKSRE
  AGESA_IDS_NV_tPDCTL,                                        ///< 0x02D5                                                                                     tPD Control
  AGESA_IDS_NV_tPD,                                           ///< 0x02D6                                                                                             tPD
  AGESA_IDS_NV_ECCEN,                                         ///< 0x02D7                                                                                      ECC Enable
  AGESA_IDS_NV_ECCSYMOBOLSIZE,                                ///< 0x02D8                                                                                 ECC Symbol Size
  AGESA_IDS_NV_2D_READ_TRAINNING,                             ///< 0x02D9                                                                               2D Read Trainning
  AGESA_IDS_NV_ALWAYSENDLLCLKS,                               ///< 0x02DA                                                                        Always enable DLL clocks
  AGESA_IDS_NV_FASTMSTATECHGDIS,                              ///< 0x02DB                                                                     Fast M-state change disable
  AGESA_IDS_NV_RXSSBMNTCLKEN,                                 ///< 0x02DC                                                                                   RxSsbMntClkEn
  AGESA_IDS_NV_RXCHMNTCLKEN,                                  ///< 0x02DD                                                               Receive channel maitenance clocks
  AGESA_IDS_NV_DLLNOLOCK,                                     ///< 0x02DE                                                                                     Dll no lock
  AGESA_IDS_NV_ENRXPADSTANDBY,                                ///< 0x02DF                                                                     Enable receiver pad standby
  AGESA_IDS_NV_DISAUTOREFRESH,                                ///< 0x02E0                                                                       Disable automatic refresh
  AGESA_IDS_NV_CMDTHROTTLEMODECTL,                            ///< 0x02E1                                                                         CmdThrottleMode Control
  AGESA_IDS_NV_CMDTHROTTLEMODE,                               ///< 0x02E2                                                                                 CmdThrottleMode
  AGESA_IDS_NV_DATABYTETXSLEWRATECTL,                         ///< 0x02E3                                                              Data Byte Tx Cal Slew Rate Control
  AGESA_IDS_NV_DATABYTETXPREDRIVERCALTXPREP,                  ///< 0x02E4                                                                   DataByteTxPreDriverCal TxPreP
  AGESA_IDS_NV_DATABYTETXPREDRIVERCALTXPREN,                  ///< 0x02E5                                                                   DataByteTxPreDriverCal TxPreN
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD1TXPREP,             ///< 0x02E6                                                              DataByteTxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD1TXPREN,             ///< 0x02E7                                                              DataByteTxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD2TXPREP,             ///< 0x02E8                                                              DataByteTxPreDriverCal2Pad2 TxPreP
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD2TXPREN,             ///< 0x02E9                                                              DataByteTxPreDriverCal2Pad2 TxPreN
  AGESA_IDS_NV_CMDADDR0TXCALSLEWRATECTL,                      ///< 0x02EA                                                       CmdAddr0 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCALPAD0TXPREP,              ///< 0x02EB                                                               CmdAddr0TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCALPAD0TXPREN,              ///< 0x02EC                                                               CmdAddr0TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD1TXPREP,             ///< 0x02ED                                                              CmdAddr0TxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD1TXPREN,             ///< 0x02EE                                                              CmdAddr0TxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD2TXPREP,             ///< 0x02EF                                                              CmdAddr0TxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD2TXPREN,             ///< 0x02F0                                                              CmdAddr0TxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_CMDADDR1TXCALSLEWRATECTL,                      ///< 0x02F1                                                       CmdAddr1 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCALPAD0TXPREP,              ///< 0x02F2                                                               CmdAddr1TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCALPAD0TXPREN,              ///< 0x02F3                                                               CmdAddr1TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD1TXPREP,             ///< 0x02F4                                                              CmdAddr1TxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD1TXPREN,             ///< 0x02F5                                                              CmdAddr1TxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD2TXPREP,             ///< 0x02F6                                                              CmdAddr1TxPreDriverCal2Pad2 TxPreP
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD2TXPREN,             ///< 0x02F7                                                              CmdAddr1TxPreDriverCal2Pad2 TxPreN
  AGESA_IDS_NV_ADDRTXCALSLEWRATECTL,                          ///< 0x02F8                                                           Addr Tx Calibration Slew Rate Control
  AGESA_IDS_NV_ADDRTXPREDRIVERCALPAD0TXPREP,                  ///< 0x02F9                                                                   AddrTxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCALPAD0TXPREN,                  ///< 0x02FA                                                                   AddrTxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD1TXPREP,                 ///< 0x02FB                                                                  AddrTxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD1TXPREN,                 ///< 0x02FC                                                                  AddrTxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD2TXPREP,                 ///< 0x02FD                                                                  AddrTxPreDriverCal2Pad2 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD2TXPREN,                 ///< 0x02FE                                                                  AddrTxPreDriverCal2Pad2 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD3TXPREP,                 ///< 0x02FF                                                                  AddrTxPreDriverCal2Pad3 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD3TXPREN,                 ///< 0x0300                                                                  AddrTxPreDriverCal2Pad3 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD4TXPREP,                 ///< 0x0301                                                                  AddrTxPreDriverCal2Pad4 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD4TXPREN,                 ///< 0x0302                                                                  AddrTxPreDriverCal2Pad4 TxPreN
  AGESA_IDS_NV_CLK0TXSLEWRATECTL,                             ///< 0x0303                                                         Clock0 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CLOCK0TXPREDRIVERCALPAD0TXPREP,                ///< 0x0304                                                                 Clock0TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CLOCK0TXPREDRIVERCALPAD0TXPREN,                ///< 0x0305                                                                 Clock0TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CLOCK1TXCALSLEWRATECTL,                        ///< 0x0306                                                         Clock1 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CLOCK1TXPREDRIVERCALPAD0TXPREP,                ///< 0x0307                                                                 Clock1TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CLOCK1TXPREDRIVERCALPAD0TXPREN,                ///< 0x0308                                                                 Clock1TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CLOCK2TXCALSLEWRATECTL,                        ///< 0x0309                                                         Clock2 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CLOCK2TXPREDRIVERCALPAD0TXPREP,                ///< 0x030A                                                                 Clock2TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CLOCK2TXPREDRIVERCALPAD0TXPREN,                ///< 0x030B                                                                 Clock2TxPreDriverCalPad0 TxPreN
  AGESA_IDS_SMU_PORT80_LOGGING,                               ///< 0x030C                                                                      SMU port80 monitor logging
  AGESA_IDS_MSR_ACCESS,                                       ///< 0x030D                                                                                      MSR Access
  AGESA_IDS_CTDP_NPST,                                        ///< 0x030E                                                                   cTDP NbPstate Selection Table
  AGESA_IDS_NV_PSTATES_SUPPORT,                               ///< 0x030F                                                                             NB P-states Support
  AGESA_IDS_NV_GNB_LOAD_SAMU_PATCH,                           ///< 0x0310                                                                                 Load SAMU patch
  AGESA_IDS_NV_DLLSTAGGERDLY,                                 ///< 0x0311                                                                               Dll Stagger Delay
  AGESA_IDS_NV_CSAMPLE_TIMER_CTRL,                            ///< 0x0312                                                                                 CSampleTimerCtl
  AGESA_IDS_NV_CSAMPLE_TIMER,                                 ///< 0x0313                                                                                    CSampleTimer
  AGESA_IDS_NV_PIPE0_THROTTLE_PACKED,                         ///< 0x0314                                                                             Pipe0ThrottlePacked
  AGESA_IDS_NV_PIPE1_THROTTLE_PACKED,                         ///< 0x0315                                                                             Pipe1ThrottlePacked
  AGESA_IDS_NV_PIPE0_THROTTLE_SELECT,                         ///< 0x0316                                                                             Pipe0ThrottleSelect
  AGESA_IDS_NV_PIPE1_THROTTLE_SELECT,                         ///< 0x0317                                                                             Pipe1ThrottleSelect
  AGESA_IDS_NV_NB_TRACING_BUFFER,                             ///< 0x0318                                                                               NB Tracing bugger
  AGESA_IDS_NV_DISSIMRDWR,                                    ///< 0x0319                                                             Disable simultaneous read and write
  AGESA_IDS_NV_DCTPWRGATEEN_CTRL,                             ///< 0x031A                                                                 Enable DCT power gating Control
  AGESA_IDS_NV_DCTPWRGATEEN,                                  ///< 0x031B                                                                         Enable DCT power gating
  AGESA_IDS_NV_DCTCLKGATEEN_CTRL,                             ///< 0x031C                                                                 Enable DCT clock gating Control
  AGESA_IDS_NV_DCTCLKGATEEN,                                  ///< 0x031D                                                                         Enable DCT clock gating
  AGESA_IDS_NV_ACP_ENGINE,                                    ///< 0x031E                                                         Reserve system memory for KV ACP engine
  AGESA_IDS_NV_GNB_GPP_PCIEMASTERPLLSELECTION,                ///< 0x031F                                                                       PCIe Master PLL Selection
  AGESA_IDS_NV_WATCHDOG_TIMER,                                ///< 0x0320                                                                          watchdog timer disable
  AGESA_IDS_NV_REDIRECT_INT1_INT3_TO_HDT,                     ///< 0x0321                                                redirect debug exceptions and breakpoints to HDT
  AGESA_IDS_NV_RDRAND,                                        ///< 0x0322                                                                                          RdRand
  AGESA_IDS_NV_EXCEPTION_BREAKPOINT_CTRL,                     ///< 0x0323                                                                    Exception breakpoint control
  AGESA_IDS_NV_EXCEPTION_BREAKPOINT_VECTOR,                   ///< 0x0324                                                                     Exception breakpoint vector
  AGESA_IDS_NV_ADAPT_FOM_BER_EN,                              ///< 0x0325                                                                    PHYLANE GEN3 FOM BER Control
  AGESA_IDS_NV_ADAPT_FOM_BER_VAL,                             ///< 0x0326                                                                      PHYLANE GEN3 FOM BER Value
  AGESA_IDS_NV_ADAPT_OFF_CAL_TIME_EN,                         ///< 0x0327                                                    PHYLANE GEN3 Offset Calculation Time Control
  AGESA_IDS_NV_ADAPT_OFF_CAL_TIME,                            ///< 0x0328                                                      PHYLANE GEN3 Offset Calculation Time Value
  AGESA_IDS_NV_ADAPT_CDR_TIME_EN,                             ///< 0x0329                                                                   PHYLANE GEN3 CDR Time Control
  AGESA_IDS_NV_ADAPT_CDR_TIME,                                ///< 0x032A                                                                     PHYLANE GEN3 CDR Time Value
  AGESA_IDS_NV_ADAPT_LEQ_TIME_EN,                             ///< 0x032B                                                                   PHYLANE GEN3 LEQ Time Control
  AGESA_IDS_NV_ADAPT_LEQ_TIME,                                ///< 0x032C                                                                     PHYLANE GEN3 LEQ Time Value
  AGESA_IDS_NV_ADAPT_DFE_TIME_EN,                             ///< 0x032D                                                                   PHYLANE GEN3 DFE Time Control
  AGESA_IDS_NV_ADAPT_DFE_TIME,                                ///< 0x032E                                                                     PHYLANE GEN3 DFE Time Value
  AGESA_IDS_NV_ADAPT_FOM_TIME_EN,                             ///< 0x032F                                                                   PHYLANE GEN3 FOM Time Control
  AGESA_IDS_NV_ADAPT_FOM_TIME,                                ///< 0x0330                                                                     PHYLANE GEN3 FOM Time Value
  AGESA_IDS_NV_ADAPT_PI_OFFSET_BYP_VAL_EN,                    ///< 0x0331                                                           PHYLANE GEN3 PI Offset Bypass Control
  AGESA_IDS_NV_ADAPT_PI_OFFSET_BYP_VAL,                       ///< 0x0332                                                             PHYLANE GEN3 PI Offset Bypass Value
  AGESA_IDS_NV_ADAPT_PI_OFFSET_BYP_EN,                        ///< 0x0333                                                            PHYLANE GEN3 PI Offset Bypass Enable
  AGESA_IDS_NV_ADAPT_TH_LOOPGAIN_EN,                          ///< 0x0334                                                               PHYLANE GEN3 TH Loop Gain Control
  AGESA_IDS_NV_ADAPT_TH_LOOPGAIN_VAL,                         ///< 0x0335                                                                 PHYLANE GEN3 TH Loop Gain Value
  AGESA_IDS_NV_ADAPT_LEQ_LOOPGAIN_EN,                         ///< 0x0336                                                              PHYLANE GEN3 LEQ Loop Gain Control
  AGESA_IDS_NV_ADAPT_LEQ_LOOPGAIN,                            ///< 0x0337                                                                PHYLANE GEN3 LEQ Loop Gain Value
  AGESA_IDS_NV_RX_ADAPT_TRACK_EN,                             ///< 0x0338                                                          PHYLANE GEN3 Adaptive Tracking Control
  AGESA_IDS_NV_RX_ADAPT_TRACK,                                ///< 0x0339                                                            PHYLANE GEN3 Adaptive Tracking Value
  AGESA_IDS_NV_TX_DIFF_EN,                                    ///< 0x033A                                                      PHYLANE GEN3 Transmit Differential Control
  AGESA_IDS_NV_TX_DIFF,                                       ///< 0x033B                                                        PHYLANE GEN3 Transmit Differential Value
  AGESA_IDS_NV_DFR_TIMER_CTL_EN,                              ///< 0x033C                                                          PHYLANE GEN3 DFR Timer Control Control
  AGESA_IDS_NV_DFR_TIMER_CTL,                                 ///< 0x033D                                                            PHYLANE GEN3 DFR Timer Control Value
  AGESA_IDS_NV_GEN12_PI_OFFSET_BYP_VAL_EN,                    ///< 0x033E                                                     PHYLANE GEN3 Gen12 PI Offset Bypass Control
  AGESA_IDS_NV_GEN12_PI_OFFSET_BYP_VAL,                       ///< 0x033F                                                       PHYLANE GEN3 Gen12 PI Offset Bypass Value
  AGESA_IDS_NV_GEN12_PI_OFFSET_BYP_EN,                        ///< 0x0340                                                      PHYLANE GEN3 Gen12 PI Offset Bypass Enable
  AGESA_IDS_NV_ANALOG_WAIT_TIME_EN,                           ///< 0x0341                                                           PHYLANE GEN3 Analog Wait Time Control
  AGESA_IDS_NV_ANALOG_WAIT_TIME,                              ///< 0x0342                                                             PHYLANE GEN3 Analog Wait Time Value
  AGESA_IDS_NV_PHASE_GAIN_GEN12_SEL_EN,                       ///< 0x0343                                                           PHYLANE GEN3 Gen12 Phase Gain Control
  AGESA_IDS_NV_PHASE_GAIN_GEN12_SEL,                          ///< 0x0344                                                             PHYLANE GEN3 Gen12 Phase Gain Value
  AGESA_IDS_NV_PHASE_GAIN_GEN3_SEL_EN,                        ///< 0x0345                                                                 PHYLANE GEN3 Phase Gain Control
  AGESA_IDS_NV_PHASE_GAIN_GEN3_SEL,                           ///< 0x0346                                                                   PHYLANE GEN3 Phase Gain Value
  AGESA_IDS_NV_RECEIVER_POWER_MODE,                           ///< 0x0347                                                                             Reciever power mode
  AGESA_IDS_NV_GNB_ACP_POWER_GATING,                          ///< 0x0348                                                                                ACP Power Gating
  AGESA_IDS_NV_GNB_ACP_CLOCK_GATING,                          ///< 0x0349                                                                                ACP Clock Gating
  AGESA_IDS_NV_GNB_CHUB_CLOCK_GATING,                         ///< 0x034A                                                                               CHUB CLOCK Gating
  AGESA_IDS_NV_DSM_LP_SEL_EN,                                 ///< 0x034B                                                                                          Dsm Lp
  AGESA_IDS_NV_EQUALIZATION_SEARCH_MODE0,                     ///< 0x034C                                                       Equalization Search Mode (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_SEARCH_MODE1,                     ///< 0x034D                                                       Equalization Search Mode (PBr1/Gfx Port1)
  AGESA_IDS_NV_EQUALIZATION_PRESET0_PBR0,                     ///< 0x034E                                                           Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET0_PBR1,                     ///< 0x034F                                                           Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_GEN1_LOOPBACK_MODE,                            ///< 0x0350                                                                            Gen 1 Loop back mode
  AGESA_IDS_CPU_FREQUENCY_LIMIT_EN,                           ///< 0x0351                                                                    CPU frequency limit  Control
  AGESA_IDS_CPU_FREQUENCY_LIMIT,                              ///< 0x0352                                                                             CPU frequency limit
  AGESA_IDS_NV_PMU_TRAIN_MODE,                                ///< 0x0353                                                                                  PMU train mode
  AGESA_IDS_NV_GNB_PMM_EAPM,                                  ///< 0x0354                                                                             Global EAPM Control
  AGESA_IDS_NV_GNB_TARGET_TDP_CONTROL,                        ///< 0x0355                                                                              Target TDP Control
  AGESA_IDS_NV_GNB_TARGET_TDP_VALUE,                          ///< 0x0356                                                                                Target TDP Value
  AGESA_IDS_NV_SYNC_FLOOD_MCA,                                ///< 0x0357                                                                                  Sync Flood MCA
  AGESA_IDS_NV_GNB_NB_IOMMU_L2_DEBUG_3_31,                    ///< 0x0358                                                                               L2_DEBUG_3 bit 31
  AGESA_IDS_NV_GNB_PMM_DCTDP,                                 ///< 0x0359                                                                                   DcTdp Control
  AGESA_IDS_NV_GNB_PMM_BBB_EN,                                ///< 0x035A                                                                                     BBB Control
  AGESA_IDS_NV_GNB_PMM_CHTC,                                  ///< 0x035B                                                                                    CHTC Control
  AGESA_IDS_NV_GNB_PMM_LEAPM,                                 ///< 0x035C                                                                              Local EAPM Control
  AGESA_IDS_NV_GNB_PMM_STAPM,                                 ///< 0x035D                                                                                   STAPM Control
  AGESA_IDS_NV_GNB_PMM_ECT,                                   ///< 0x035E                                                                                     ECT Control
  AGESA_IDS_NV_GNB_PMM_ECT_LIMIT_OVERRIDE_EN,                 ///< 0x035F                                                                      ECT Limit Override Control
  AGESA_IDS_NV_GNB_PMM_ECT_LIMIT_OVERRIDE0,                   ///< 0x0360                                                                            ECT Limit Override 0
  AGESA_IDS_NV_GNB_PMM_ECT_LIMIT_OVERRIDE1,                   ///< 0x0361                                                                            ECT Limit Override 1
  AGESA_IDS_NV_EQUALIZATION_PRESET1_PBR0,                     ///< 0x0362                                                           Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET1_PBR1,                     ///< 0x0363                                                           Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_EQUALIZATION_PRESET2_PBR0,                     ///< 0x0364                                                           Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET2_PBR1,                     ///< 0x0365                                                           Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_EQUALIZATION_PRESET3_PBR0,                     ///< 0x0366                                                           Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET3_PBR1,                     ///< 0x0367                                                           Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_LC_USC_EQ_NOT_REQD_PBR0,                       ///< 0x0368                                                           Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_LC_USC_EQ_NOT_REQD_PBR1,                       ///< 0x0369                                                           Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_GNB_DFSBYPASS,                                 ///< 0x036A                                                                              DFS Bypass Control
  AGESA_IDS_NV_GNB_PMM_MULTIVID,                              ///< 0x036B                                                                                GNB PMM MULTIVID
  AGESA_IDS_NV_GNB_PCIE_HW_INIT_PWR_GATING,                   ///< 0x036C                                                              GNB PCIe Hardware Init PowerGating
  AGESA_IDS_NV_GNB_SPG_CLOCK_GATING,                          ///< 0x036D                                                                            GNB SPG Clock gating
  AGESA_IDS_NV_GNB_ACG_AZ_CLOCK_GATING,                       ///< 0x036E                                                                         GNB ACG_AZ Clock Gating
  AGESA_IDS_NV_GNB_IOMMU_L1_MEM_POWER_GATING,                 ///< 0x036F                                                                GNB IOMMU L1 Memory Power Gating
  AGESA_IDS_NV_GNB_IOMMU_L2_MEM_POWER_GATING,                 ///< 0x0370                                                                GNB IOMMU L2 Memory Power Gating
  AGESA_IDS_NV_TREFCTL,                                       ///< 0x0371                                                                                    TREF Control
  AGESA_IDS_NV_TRFC0CTL,                                      ///< 0x0372                                                                                   TRFC0 Control
  AGESA_IDS_NV_TRFC1CTL,                                      ///< 0x0373                                                                                   TRFC1 Control
  AGESA_IDS_NV_DATASCRAMBLE,                                  ///< 0x0374                                                                                   Data Scramble
  AGESA_IDS_NV_RDPTR_NBP0_CTL,                                ///< 0x0375                                                                       RdPtrInitVal NBP0 Control
  AGESA_IDS_NV_RDPTR_NBP0,                                    ///< 0x0376                                                                               RdPtrInitVal NBP0
  AGESA_IDS_NV_RDPTR_NBP1_CTL,                                ///< 0x0377                                                                       RdPtrInitVal NBP1 Control
  AGESA_IDS_NV_RDPTR_NBP1,                                    ///< 0x0378                                                                               RdPtrInitVal NBP1
  AGESA_IDS_NV_RDPTR_NBP2_CTL,                                ///< 0x0379                                                                       RdPtrInitVal NBP2 Control
  AGESA_IDS_NV_RDPTR_NBP2,                                    ///< 0x037A                                                                               RdPtrInitVal NBP2
  AGESA_IDS_NV_RDPTR_NBP3_CTL,                                ///< 0x037B                                                                       RdPtrInitVal NBP3 Control
  AGESA_IDS_NV_RDPTR_NBP3,                                    ///< 0x037C                                                                               RdPtrInitVal NBP3
  AGESA_IDS_NV_GNB_ORB_TX_MEM_POWER_GATING,                   ///< 0x037D                                                                  GNB ORB TX Memory Power Gating
  AGESA_IDS_NV_GNB_ORB_RX_MEM_POWER_GATING,                   ///< 0x037E                                                                  GNB ORB RX Memory Power Gating
  AGESA_IDS_NV_GNB_ACG_AZ_PRESENT,                            ///< 0x037F                                                                              GNB ACG_AZ present
  AGESA_IDS_NV_GNB_SSTUNL_CLOCK_GATING,                       ///< 0x0380                                                                         GNB SSTUNL Clock Gating
  AGESA_IDS_NV_GNB_SMU_FEAT_BIT_MAP_CTRL,                     ///< 0x0381                                                                     GNB SMU Feature bit control
  AGESA_IDS_NV_GNB_SMU_FEAT_BIT_MAP_SET,                      ///< 0x0382                                                                         GNB SMU Feature bit Map
  AGESA_IDS_NV_GFXCURRENTLIMITCTL,                            ///< 0x0383                                                                       GFX Current Limit Control
  AGESA_IDS_NV_GFXCURRENTLIMIT,                               ///< 0x0384                                                                               GFX Current Limit
  AGESA_IDS_NV_GFXMAXCURRENTLIMIT,                            ///< 0x0385                                                                           GFX Max Current Limit
  AGESA_IDS_NV_PHYPOWERSAVING,                                ///< 0x0386                                                                          DRAM Phy power savings
  AGESA_IDS_NV_MEMCLK_ABOVE_NCLK,                             ///< 0x0387                                                                         Enforce MClk above NClk
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMLO,                          ///< 0x0388                                                                       GNB SMU PORT80 Memory low
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMHI,                          ///< 0x0389                                                                      GNB SMU PORT80 Memory High
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMSIZE,                        ///< 0x038A                                                                  GNB SMU PORT80 Memory Position
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMPOSN,                        ///< 0x038B                                                                       GNB SMU PORT80 Memory low
  AGESA_IDS_NV_GNB_SMU_PORT80_POLL,                           ///< 0x038C                                                                             GNB SMU PORT80 poll
  AGESA_IDS_NV_GNB_SMU_PORT80_CSR,                            ///< 0x038D                                                                              GNB SMU PORT80 CSR
  AGESA_IDS_NV_GNB_SMU_PORT80_DRAM,                           ///< 0x038E                                                                             GNB SMU PORT80 DRAM
  AGESA_IDS_NV_GNB_SMU_PORT80_EN,                             ///< 0x038F                                                                           GNB SMU PORT80 Enable
  AGESA_IDS_NV_BANKGROUPSWAP,                                 ///< 0x0390                                                                                   BankGroupSwap
  AGESA_IDS_NV_WRCRCEN,                                       ///< 0x0391                                                                                Write CRC Enable
  AGESA_IDS_NV_STAPM_CONFIG_CONTROL,                          ///< 0x0392                                                                            STAPM Config Control
  AGESA_IDS_NV_STAPM_SCALAR,                                  ///< 0x0393                                                                                    STAPM Scalar
  AGESA_IDS_NV_STAPM_BOOST,                                   ///< 0x0394                                                                              STAPM Boost Enable
  AGESA_IDS_NV_STAPM_TIME_CONSTANT,                           ///< 0x0395                                                                             STAPM Time Constant
  AGESA_IDS_NV_MEM_MRL_RETRAIN_TIMES,                         ///< 0x0396                      Specify how many times MRL training should be executed should error occurs
  AGESA_IDS_INT_ID_END,                                       ///< 0x0397                                                               specify the end of internal NV ID
} IDS_INT_NV_ID;

/// Ids internal feat define
typedef enum {
  IDS_INT_FEAT_CORE_LEVELING = 0x100,       ///< Feat for Core leveling
  IDS_INT_FEAT_CACHE_FLUSHCTRL,             ///< Feat for Cache Flush Control
  IDS_INT_FEAT_CSTATE_MODE,                 ///< Feat for C state mode
  IDS_INT_FEAT_ACPI_PSS,                    ///< Feat for PSS
  IDS_INT_FEAT_ACPI_CST,                    ///< Feat for CST
  IDS_INT_FEAT_ACPI_WHEA,                   ///< Feat for whea
  IDS_INT_FEAT_DMI_TBL,                     ///< Feat for DMI table
  IDS_INT_FEAT_CUSTOM_PSTATE,               ///< Feat for Custom Pstate
  IDS_INT_FEAT_PSI_EN,                      ///< Feat for PSI enable
  IDS_INT_FEAT_MEMORY_SIZE,                 ///< Feat for Memory size
  IDS_INT_FEAT_DCT_CONFIG_CONTROL,          ///< Feat for Dct Config Control
  IDS_INT_FEAT_MEMORY_TBL_DRV_SUPPORT,      ///< Feat for memory table driven
  IDS_INT_FEAT_ENFORCE_FREQ,                ///< Feat for enforce memory freq
  IDS_INT_FEAT_MEMORY_TIMING,               ///< Feat for memory timing config
  IDS_INT_FEAT_MEMORY_DRIVE_STRENGTH,       ///< Feat for memory drive strength
  IDS_INT_FEAT_DQS_TIMING_CONFIG,           ///< Feat for Dqs timing config
  IDS_INT_FEAT_GRA_FUNCTION,                ///< Feat for GRA function
  IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,     ///< Feat for Memory loadcard support
  IDS_INT_FEAT_MEMORY_PHYDLL_STANDBY_CTRL,  ///< Feat for memory phydll control
  IDS_INT_FEAT_MEM_ERROR_RECOVERY,          ///< Feat for memory error recovery
  IDS_INT_FEAT_MEM_RETRAIN_TIMES,           ///< Feat for memory retrain times
  IDS_INT_FEAT_MEM_MRL_RETRAIN_TIMES,       ///< Feat for memory MRL retrain times
  IDS_INT_FEAT_DEEMPHASIS,                  ///< Feat for deemphasis
  IDS_INT_FEAT_CheckNegativeWl,             ///< Feat for check memory Negative warning
  IDS_INT_FEAT_IGNORE_MEM_ERROR,            ///< Feat for ignore memory error
  IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE,         ///< Feat for override GNB dynamic wake
  IDS_INT_FEAT_HDTBREAKPOINT,               ///< Feat for hdt break point
  IDS_INT_FEAT_GNB_CLOCKGATING,             ///< Feat for override GNB clock gating
  IDS_INT_FEAT_GNB_LCLK_DPM,                ///< Feat for Override GNB LCLK DPM
  IDS_INT_FEAT_GNB_LCLK_DEEP_SLEEP,         ///< Feat for override GNB LCLK DPM Deep sleep
  IDS_INT_FEAT_MAIN_PLL,                    ///< Feat for overriding the main PLL freq
  IDS_INT_FEAT_NB_PSTATE_DIDVID,            ///< Feat for overriding the NB Pstate DIDs and VIDs
  IDS_INT_FEAT_REG_TABLE,                   ///< Feat for add run time register table support
  IDS_INT_FEAT_NB_BUFFER_ALLOCATION,        ///< Feat for override North bridge bufer allocation
  IDS_INT_FEAT_SKIP_ERRATUM463_WORKAROUND,  ///< Feat for skip Erratum463 workaround
  IDS_INT_FEAT_SKIP_DLL_CSR,                ///< Feat for skip Dll CSR
  IDS_INT_FEAT_FCH_CONFIGURATION,           ///< Feat for FCH data block override
  IDS_INT_FEAT_ENFORCE_VDDIO,               ///< Feat for enforcing VDDIO
  IDS_INT_FEAT_SKIP_FUSED_MAX_RATE,         ///< Feat for skipping fused max rate cap
  IDS_INT_FEAT_GNB_FORCE_CABLESAFE,         ///< Feat for overriding Force CableSafe
  IDS_INT_FEAT_GNB_PROPERTY,                ///< Feat for GNB Property
  IDS_INT_FEAT_GNB_PCIE_POWER_GATING,       ///< Feat for GNB PCIe Power Gating
  IDS_INT_TRACE_MODE,                       ///< Feat for Trace mode
  IDS_INT_FEAT_GNB_ALTVDDNB,                ///< Feat for overriding AltVddNb
  IDS_INT_FEAT_MEM_POWER_POLICY,            ///< Feat for overriding memory power policy
  IDS_INT_FEAT_GNB_PMM_SWTJOFFSET,          ///< Feat for overriding SWTJOFFSET
  IDS_INT_FEAT_LOCK_DRAM_CFG,               ///< Feat for overriding BFLockDramCfg
  IDS_INT_FEAT_CURRENT_LIMIT,               ///< Feat for overriding CPU/NB Current Limit
  IDS_INT_FEAT_GNB_PMM_PCIE_PHY_ISOLATION,  ///< Feat for overriding Pcie Phy Isolation
  IDS_INT_FEAT_GNB_PMM_PORT_REMAP,          ///< Feat for SW Device ID Remapping
  IDS_INT_FEAT_MSR_ACCESS_OVERRIDE,         ///< Feat for MSR ACCESS OVERRIDE
  IDS_INT_FEAT_LOAD_SAMU_PATCH,             ///< Feat for Load SAMU patch
  IDS_INT_FEAT_DLLSTAGGERDLY,               ///< Feat for Dll Stagger Delay
  IDS_INT_FEAT_CSAMPLETIMER,                ///< Feat for overriding CSampleTimer
  IDS_INT_FEAT_PIPETHROTTLE,                ///< Feat for overriding PipeThrottle
  IDS_INT_FEAT_GNB_PMM_NATIVEGEN1PLL,       ///< Feat for Native Gen1 PLL
  IDS_INT_FEAT_GNB_GPP_PCIE_MASTERPLL_SELECTION, ///< Feat for PCIe Master PLL Selection
  IDS_INT_FEAT_WATCHDOG_TIMER,              ///< Feat for watchdog timer disable
  IDS_INT_FEAT_EXCEPTION_BP,                ///< Feat for exception breakpoint
  IDS_INT_FEAT_PCIE_PHYLANE_CONFIG,         ///< PCIe PHY Lane Configuration
  IDS_INT_FEAT_DSM_LP,                      ///< DSM low power
  IDS_INT_FEAT_GNB_EQUAL_PRESET,            ///< Feat for setting equalization presets
  IDS_INT_FEAT_GNB_GEN1_LOOPBACK,           ///< Feat for overriding Gen1 loop back mode
  IDS_INT_FEAT_GNB_SMU_SERVICE_MASK,        ///< Feat for overriding Gnb SMU service mask
  IDS_INT_FEAT_GNB_ADAPT_PI_OFFSET_BYP_EN,  ///< Feat for overriding Adapt PI Offset Byp Enable
  IDS_INT_FEAT_GNB_TARGET_TDP,              ///< Feat for overriding target TDP
  IDS_INT_FEAT_GNB_SMU_PORT80_PARAMS,       ///< Feat for Gnb smu port80
  IDS_INT_FEAT_FIXUP_CUSTOM_PSTATES,        ///< Feat for fixing boost states after warm reset
  IDS_INT_FEAT_NB_PSTATES,                  ///< Feat for NB Pstates
  IDS_INT_FEAT_MEMORY_PHY_POWER_SAVING,     ///< Feat for DRAM Phy power savings
  IDS_INT_FEAT_MEMORY_MCLK_ABOVE_NCLK,      ///< Feat for enforcing MCLK above NCLK
  IDS_INT_FEAT_CTDP_NPST,                   ///< Feat for cTDP NbPstate Selection Table
  IDS_INT_FEAT_WORKAROUND,                  ///< Feat for workaround
  IDS_INT_FEAT_END                          ///< End of internal Feat
} IDS_INT_FEAT;


#define IDS_NUM_INT_NV_ITEM (AGESA_IDS_INT_ID_END - AGESA_IDS_INT_ID_START + 1)
#define IDS_NUM_NV_ITEM (IDS_NUM_INT_NV_ITEM + IDS_NUM_EXT_NV_ITEM)

/// enum for IDS Memclk
typedef enum {
  IDS_MEMORYCLOCK_SPEED_400MHZ = 0, ///< 0x0 400mhz
  IDS_MEMORYCLOCK_SPEED_533MHZ, ///< 0x1 533mhz
  IDS_MEMORYCLOCK_SPEED_667MHZ, ///< 0x2 667mhz
  IDS_MEMORYCLOCK_SPEED_800MHZ , ///< 0x3 800mhz
  IDS_MEMORYCLOCK_SPEED_1066MHZ, ///< 0x4 1066mhz
  IDS_MEMORYCLOCK_SPEED_1333MHZ, ///< 0x5 1333mhz
  IDS_MEMORYCLOCK_SPEED_1600MHZ, ///< 0x6 1600mhz
  IDS_MEMORYCLOCK_SPEED_1866MHZ, ///< 0x7 1866mhz
  IDS_MEMORYCLOCK_SPEED_2100MHZ, ///< 0x8 1866mhz
  IDS_MEMORYCLOCK_SPEED_2133MHZ, ///< 0x9 2133mhz
  IDS_MEMORYCLOCK_SPEED_2400MHZ, ///< 0xa 2133mhz
  IDS_MEMORYCLOCK_SPEED_END, ///< END
} IDS_MEMORYCLOCK_SPEED;

/// enum for IDS FORCE ASSERT
typedef enum {
  IDS_FORCE_ASSERT_AUTO = 0,///<Auto
  IDS_FORCE_ASSERT_ENABLE = 1,///<Enable
} IDS_FORCE_ASSERT;

/**
 *  @page iemsupport IEM SUPPORT
 *
 *  The following section supports macros which allow the code flow to be altered with
 *  a single build level switch for the purposes of accomodating the requirements of the
 *  specific environments. The main principle is to have minimal code footprint when
 *  enabled mode and zero footprint when disabled.
 *
 *  IEM_SKIP_CODE(L)
 *    This macro will cause the code block which follows (designated by curly braces) to be SKIPPED OVER
 *
 *    Macro Parameters:
 *                L - IEM_SKIP_LEVEL value which indicates the category of support that each
 *                    usage of this macro belongs to.  If this level is supported in the build,
 *                    then the code will be skipped over.
 *
 *    example:
 *        IEM_SKIP_CODE(IEM_WAIT) {
 *          ...
 *          code that should be skipped over if IEM_WAIT is enabled....
 *          ...
 *        }
 *
 *  IEM_INSERT_CODE(L,Fn,Parm)
 *    This macro will cause the specified function call to be made with the parameter which follows.
 *
 *  Macro Parameters:
 *                L    - IEM_SKIP_LEVEL value which indicates the category of support that each
 *                       usage of this macro belongs to.  If this level is supported in the build,
 *                       then the function will be called.
 *                Fn   - Function to be called.  This is an Internally defined function which will be
 *                       called if the level is supported.
 *                Parm - Parameter List.  This is a parameter list with parenthesis included that is
 *                       to be passed to the function.
 *
 *    example:
 *        IEM_INSERT_CODE (IEM_POLL, IemSetDoneStatus, (&Done,Count))
 *
 *  IEM INSERT FUNCTION PROTOTYPE
 *
 *      The function which is called by the IEM_INSERT_CODE macro must be defined in an
 *  internal-only file and prototyped in a file specific to the code that it affects.
 *
 *    example prototype:
 *      VOID
 *      IemSetDoneStatus(
 *      IN   VOID *Statusval,
 *      IN   UINT32  c
 *      );
 *
 * To Enable IEM Support, the following definitions should be
 * added to OptionsIds.h.
 *
 *    IEMOPT_IEM_ENABLED      TRUE   // This switch turns on all Macros.
 *    IEMOPT_WAIT             TRUE   // Controls insert/skip of wait functions
 *    IEMOPT_POLL             TRUE   // Controls insert/skip of Poll functions
 *    IEMOPT_EARLY_MAX_RD_LAT FALSE  // Controls insert/skip of early max rd latency control functions
 *
 *
 *
 **/

/**
 * IEM OPTION DEFAULTS
 * When defining a new level, set the default below
 */
#ifndef IEMOPT_IEM_ENABLED
  #define IEMOPT_IEM_ENABLED      FALSE
#endif
#ifndef IEMOPT_WAIT
  #define IEMOPT_WAIT             FALSE
#endif
#ifndef IEMOPT_POLL
  #define IEMOPT_POLL             FALSE
#endif
#ifndef IEMOPT_RESET
  #define IEMOPT_RESET             FALSE
#endif
#ifndef IEMOPT_MEM_RESTORE
  #define IEMOPT_MEM_RESTORE      FALSE
#endif
#ifndef IEMOPT_EARLY_MAX_RD_LAT
  #define IEMOPT_EARLY_MAX_RD_LAT FALSE
#endif
#ifndef IEMOPT_NBBLOCK_INIT
  #define IEMOPT_NBBLOCK_INIT     FALSE
#endif
#ifndef IEMOPT_DCT_INIT
  #define IEMOPT_DCT_INIT         FALSE
#endif
#ifndef IEMOPT_BEFORE_DRAM_INIT
  #define IEMOPT_BEFORE_DRAM_INIT FALSE
#endif
#ifndef IEMOPT_EARLY_DCT_CONFIG
  #define IEMOPT_EARLY_DCT_CONFIG     FALSE
#endif
#ifndef IEMOPT_EARLY_DEVICE_INIT
  #define IEMOPT_EARLY_DEVICE_INIT    FALSE
#endif
#ifndef IEMOPT_LATE_DCT_CONFIG
  #define IEMOPT_LATE_DCT_CONFIG      FALSE
#endif
#ifndef IEMOPT_LATE_MAX_RD_LAT
  #define IEMOPT_LATE_MAX_RD_LAT      FALSE
#endif
#ifndef IEMOPT_AFTER_EXTRACT_G5_SPD
  #define IEMOPT_AFTER_EXTRACT_G5_SPD FALSE
#endif
#ifndef IEMOPT_BEFORE_PMU_MSG_BLOCK_WRITE
  #define IEMOPT_BEFORE_PMU_MSG_BLOCK_WRITE FALSE
#endif
#ifndef IEMOPT_EVENT_LOG
  #define IEMOPT_EVENT_LOG FALSE
#endif
#ifndef IEMOPT_CPU_POWER_MANAGER_TABLE
  #define IEMOPT_CPU_POWER_MANAGER_TABLE FALSE
#endif
#ifndef IEMOPT_GNB_ACG_CONFIG
  #define IEMOPT_GNB_ACG_CONFIG FALSE
#endif
/**
 * IEM LEVEL Indexes
 * When defining a new level, add a corresponding index
 */
typedef enum _IEM_SKIP_LEVEL {
  IEM_WAIT,                       ///< Wait
  IEM_POLL,                       ///< Poll
  IEM_RESET,                      ///< Reset
  IEM_MEM_RESTORE,                ///< Restore
  IEM_EARLY_MAX_RD_LAT,           ///< MaxRdLat
  IEM_NBBLOCK_INIT,               ///< Nbblock init
  IEM_DCT_INIT,                   ///< Dct Init
  IEM_BEFORE_DRAM_INIT,           ///< Before Dram Init
  IEM_EARLY_DCT_CONFIG,           ///< Early Dct Config
  IEM_EARLY_DEVICE_INIT,          ///< Early Device Init
  IEM_LATE_DCT_CONFIG,            ///< Late Dct config
  IEM_LATE_MAX_RD_LAT,            ///< Late Max Rd Latency
  IEM_AFTER_EXTRACT_G5_SPD,       ///< After extracting G5 SPD
  IEM_BEFORE_PMU_MSG_BLOCK_WRITE, ///< Before writing the Pmu Msg block
  IEM_EVENT_LOG,                  ///< Evnet log
  IEM_CPU_POWER_MANAGER_TABLE,    ///< CPU power manager table
  IEM_GNB_ACG_CONFIG,             ///< GNB ACG configuration
  IEM_NUM_LEVELS                  ///< End
} IEM_SKIP_LEVEL;

extern CONST BOOLEAN IemOptSkipLevels[];
/**
 * IEM MACRO DEFINITIONS
 */
#if IEMOPT_IEM_ENABLED == TRUE
  #define IEM_SKIP_CODE(L) \
     if (!IemOptSkipLevels[L])

  #define IEM_INSERT_CODE(L, Fn, Parm ) \
  if (IemOptSkipLevels[L]) {\
    Fn Parm; \
  }
#endif

#endif // _IDS_INTERNAL_H_
