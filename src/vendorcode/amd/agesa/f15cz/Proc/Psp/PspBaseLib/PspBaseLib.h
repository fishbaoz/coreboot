/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Base Library
 *
 * Contains interface to the PSP library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 **/

#ifndef _PSP_BASE_LIB_H_
#define _PSP_BASE_LIB_H_

#include "AMD.h"
#include "amdlib.h"
#include "PspDirectory.h"

#define PSP_PCI_SEG        0x00    ///< PSP Seg address
#define PSP_PCI_BUS        0x00    ///< PSP Bus address
#define PSP_PCI_DEV        0x08    ///< PSP Device address
#define PSP_PCI_FN         0x00    ///< PSP Fn address
#define PSP_PCI_BDA        ((PSP_PCI_DEV << 11) + (PSP_PCI_FN << 8))
#define GET_PSP_PCI_ADDR (Offset)    MAKE_SBDFO (PSP_PCI_SEG, PSP_PCI_BUS, PSP_PCI_DEV, PSP_PCI_FN, Offset)

#define PSP_PCI_DEVID_REG           0x00    ///< DevId
#define PSP_PCI_CMD_REG             0x04    ///< CmdReg
#define PSP_PCI_BAR1_REG            0x18    ///< Pci Bar1
#define PSP_PCI_BAR3_REG            0x20    ///< Pci Bar3
#define PSP_PCI_MIRRORCTRL1_REG     0x44    ///< PSP Mirror Reg Ctrl 1
#define PSP_PCI_EXTRAPCIHDR_REG     0x48    ///< Extra PCI Header Ctr
#define PSP_PCI_HTMSICAP_REG        0x5C    ///<  HT MSI Capability

#define D8F0x44_PmNxtPtrW_MASK                                  0xff

#define PSP_MAILBOX_BASE            0x70    ///< Mailbox base offset on PCIe BAR
#define PSP_MAILBOX_STATUS_OFFSET   0x4     ///< Staus Offset

//======================================================================================
//
// Define Mailbox Status field
//
//======================================================================================
//
/// MBox Status MMIO space
///
typedef struct {
  UINT32 MboxInitialized:1;   ///< Target will set this to 1 to indicate it is initialized  (for ex. PSP/TPM ready)
  UINT32 Error:1;             ///< Target in adddtion to Done bit will also set this bit to indicate success/error on last command
  UINT32 Terminated:1;        ///< Target will set this bit if it aborted the command due to abort request
  UINT32 Halt:1;              ///< Target will set this error if there is critical error that require reset etc
  UINT32 Recovery:1;          ///< Target will set this error if some PSP entry point by PSP directory has been corrupted.
  UINT32 Reserved:27;          ///< Reserved
} MBOX_STATUS;

//
// Above defined as bitmap
#define MBOX_STATUS_INITIALIZED       0x00000001ul    ///< Mailbox Status: Initialized
#define MBOX_STATUS_ERROR             0x00000002ul    ///< Mailbox Status: Error
#define MBOX_STATUS_ABORT             0x00000004ul    ///< Mailbox Status: Abort
#define MBOX_STATUS_HALT              0x00000008ul    ///< Mailbox Status: Halt
#define MBOX_STATUS_RECOVERY          0x00000010ul    ///< Mailbox Status: Recovery required



UINT32
PspLibPciReadConfig (
  IN  UINT32 Register
  );

VOID
PspLibPciWriteConfig (
  IN  UINT32 Register,
  IN  UINT32 Value
  );

UINT32
PspLibPciReadPspConfig (
  IN UINT16 Offset
  );

VOID
PspLibPciWritePspConfig (
  IN UINT16 Offset,
  IN  UINT32 Value
  );

BOOLEAN
GetPspDirBase (
  IN OUT   UINT32     *Address
);

BOOLEAN
PSPEntryInfo (
  IN      PSP_DIRECTORY_ENTRY_TYPE    EntryType,
  IN OUT  UINT64                      *EntryAddress,
  IN      UINT32                      *EntrySize
  );

BOOLEAN
CheckPspDevicePresent (
  VOID
  );

BOOLEAN
CheckPspPlatformSecureEnable (
  VOID
  );

BOOLEAN
CheckPspRecoveryFlag (
  VOID
  );

BOOLEAN
GetPspBar1Addr (
  IN OUT   UINT32 *PspMmio
  );

BOOLEAN
GetPspBar3Addr (
  IN OUT   UINT32 *PspMmio
  );


BOOLEAN
GetPspMboxStatus (
  IN OUT   MBOX_STATUS **MboxStatus
  );


BOOLEAN
PspBarInitEarly ();

VOID
PspLibPciIndirectRead (
  IN       PCI_ADDR     Address,
  IN       UINT32       IndirectAddress,
  IN       ACCESS_WIDTH Width,
     OUT   VOID         *Value
  );

VOID
PspLibPciIndirectWrite (
  IN      PCI_ADDR      Address,
  IN      UINT32        IndirectAddress,
  IN      ACCESS_WIDTH  Width,
  IN      VOID          *Value
  );

#endif // _AMD_LIB_H_
