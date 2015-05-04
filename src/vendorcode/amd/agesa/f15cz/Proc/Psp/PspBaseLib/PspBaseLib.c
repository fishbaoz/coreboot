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
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "Filecode.h"
#include "PspBaseLib.h"

#define FILECODE PROC_PSP_PSPBASELIB_PSPBASELIB_FILECODE
#define PSP_BAR1_TMP_BASE 0xFEA00000ul

#define GET_PCI_BUS(Reg) (((UINT32) Reg >> 16) & 0xFF)
#define GET_PCI_DEV(Reg) (((UINT32) Reg >> 11) & 0x1F)
#define GET_PCI_FUNC(Reg) (((UINT32) Reg >> 8) & 0x7)
#define GET_PCI_OFFSET(Reg) ((UINT32)Reg & 0xFF)

#define PCI_CONFIG_SMU_INDIRECT_INDEX   0xB8          ///<  Gnb Offset index for SMU mbox
#define PCI_CONFIG_SMU_INDIRECT_DATA    0xBC          ///<  Gnb Offset data for SMU mbox

#define SMU_CC_PSP_FUSES_STATUS         0xC0018000ul  ///< offset in GNB to find PSP fusing
#define SMU_CC_PSP_FUSES_SECURE         BIT0          ///< BIT0
#define SMU_CC_PSP_FUSES_FRA_ENABLE     BIT1          ///< BIT1
#define SMU_CC_PSP_FUSES_PROTO          BIT2          ///< BIT2
#define PLATFORM_SECURE_BOOT_EN         BIT4          ///< BIT4


#define PSP_BLANK_PART                  0   ///< Blank part
#define PSP_PROTO_PART                  SMU_CC_PSP_FUSES_PROTO    ///< Proto Part
#define PSP_NON_SECURE_PART             (SMU_CC_PSP_FUSES_PROTO + SMU_CC_PSP_FUSES_PCPU_DIS)    ///< Non Secure Part
#define PSP_SECURE_PART                 (SMU_CC_PSP_FUSES_PROTO + SMU_CC_PSP_FUSES_SECURE)      ///< Secure Part
#define PSP_FRA_MODE                    (SMU_CC_PSP_FUSES_FRA_ENABLE + SMU_CC_PSP_FUSES_PROTO + SMU_CC_PSP_FUSES_SECURE)  ///< FRA Part

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */



UINT32
PspLibPciReadConfig (
  IN  UINT32 Register
  )
{
  UINT32 Value;
  PCI_ADDR PciAddr;

  Value = 0;
  PciAddr.AddressValue = MAKE_SBDFO (0, GET_PCI_BUS (Register), GET_PCI_DEV (Register), GET_PCI_FUNC (Register), GET_PCI_OFFSET (Register));

  LibAmdPciRead (AccessWidth32, PciAddr, &Value, (VOID *)-1);

  return Value;
}

VOID
PspLibPciWriteConfig (
  IN  UINT32 Register,
  IN  UINT32 Value
  )
{
  PCI_ADDR PciAddr;
  PciAddr.AddressValue = MAKE_SBDFO (0, GET_PCI_BUS (Register), GET_PCI_DEV (Register), GET_PCI_FUNC (Register), GET_PCI_OFFSET (Register));

  LibAmdPciWrite (AccessWidth32, PciAddr, &Value, (VOID *)-1);

}

UINT32
PspLibPciReadPspConfig (
  IN UINT16 Offset
  )
{
  return (PspLibPciReadConfig  ((UINT32) (PSP_PCI_BDA  + Offset)));
}

VOID
PspLibPciWritePspConfig (
  IN UINT16 Offset,
  IN  UINT32 Value
  )
{
  PspLibPciWriteConfig  ((UINT32) (PSP_PCI_BDA  + Offset), Value);
}

BOOLEAN
GetPspDirBase (
  IN OUT   UINT32     *Address
  )
{
  UINTN                     i;
  FIRMWARE_ENTRY_TABLE      *FirmwareTableBase;
  CONST UINT32 RomSigAddrTable[] =
  {
    0xFFFA0000, //  --> 512KB base
    0xFFF20000, //  --> 1MB base
    0xFFE20000, //  --> 2MB base
    0xFFC20000, //  --> 4MB base
    0xFF820000, //  --> 8MB base
    0xFF020000  //  --> 16MB base
  };

  for (i = 0; i < sizeof (RomSigAddrTable) / sizeof (UINT32); i++) {
    FirmwareTableBase  = (FIRMWARE_ENTRY_TABLE *) (UINTN) RomSigAddrTable[i];
    // Search flash for unique signature 0x55AA55AA
    if (FirmwareTableBase->Signature  == FIRMWARE_TABLE_SIGNATURE) {
      *Address = FirmwareTableBase->PspDirBase;
      return TRUE;
    }
  }

  return (FALSE);
}

BOOLEAN
PSPEntryInfo (
  IN       PSP_DIRECTORY_ENTRY_TYPE    EntryType,
  IN OUT   UINT64                      *EntryAddress,
  IN       UINT32                      *EntrySize
  )
{
  PSP_DIRECTORY         *PspDir;
  UINTN                 i;

  if (GetPspDirBase ((UINT32 *)&PspDir ) != TRUE) {
    return FALSE;
  }

  for (i = 0; i < PspDir->Header.TotalEntries; i++) {
    if (PspDir->PspEntry[i].Type == EntryType) {
      *EntryAddress = PspDir->PspEntry[i].Location;
      *EntrySize = PspDir->PspEntry[i].Size;
      return (TRUE);
    }
  }

  return (FALSE);
}

/**
  Check if PSP device is present

  @retval BOOLEAN  0: PSP Disabled, 1: PSP Enabled

**/
BOOLEAN
CheckPspDevicePresent (
  VOID
  )
{
  UINT32 SecureFuseReg;
  PspLibPciWriteConfig ( (UINT32)PCI_CONFIG_SMU_INDIRECT_INDEX, (UINT32)SMU_CC_PSP_FUSES_STATUS);
  SecureFuseReg = PspLibPciReadConfig ( (UINT32)PCI_CONFIG_SMU_INDIRECT_DATA);

  if (SecureFuseReg &= SMU_CC_PSP_FUSES_PROTO) {
    return (TRUE);
  }
  return (FALSE);
}

/**
  Check PSP Platform Seucre Enable State
  HVB & Secure S3 (Resume vector set to Dram, & core content will restore by uCode)
  will be applied if Psp Plaform Secure is enabled

  @retval BOOLEAN  0: PSP Platform Secure Disabled, 1: PSP Platform Secure  Enabled

**/
BOOLEAN
CheckPspPlatformSecureEnable (
  VOID
  )
{
  UINT32 SecureFuseReg;
  PspLibPciWriteConfig ( (UINT32)PCI_CONFIG_SMU_INDIRECT_INDEX, (UINT32)SMU_CC_PSP_FUSES_STATUS);
  SecureFuseReg = PspLibPciReadConfig ( (UINT32)PCI_CONFIG_SMU_INDIRECT_DATA);

  if (SecureFuseReg &= PLATFORM_SECURE_BOOT_EN) {
    return (TRUE);
  }
  return (FALSE);
}

/**
  Check PSP Recovery Flag
  Target will set Recovery flag if some PSP entry point by PSP directory has been corrupted.

  @retval BOOLEAN  0: Recovery Flag is cleared, 1: Recovery Flag has been set

**/
BOOLEAN
CheckPspRecoveryFlag (
  VOID
  )
{
  MBOX_STATUS *MboxStatus;

  //Init PSP MMIO
  PspBarInitEarly ();

  GetPspMboxStatus (&MboxStatus);

  return (BOOLEAN) (MboxStatus->Recovery);
}

/**
  Return the PspMbox MMIO location


  @retval BOOLEAN  FALSE: ERROR, TRUE: SUCCEED

**/
BOOLEAN
GetPspMboxStatus (
  IN OUT   MBOX_STATUS **MboxStatus
  )
{
  UINT32 PspMmio;

  if (GetPspBar1Addr (&PspMmio) == FALSE) {
    return (FALSE);
  }

  *MboxStatus = (MBOX_STATUS *)( (UINTN)PspMmio + PSP_MAILBOX_BASE + PSP_MAILBOX_STATUS_OFFSET);   // PSPMbox base is at offset CP2MSG_28 ie. offset 28*4 = 0x70

  return (TRUE);
}

BOOLEAN
PspBarInitEarly ()
{
  UINT32 PspMmioSize;
  UINT32 Value32;

  if (PspLibPciReadPspConfig  (PSP_PCI_DEVID_REG) == 0xffffffff) {
    return (FALSE);
  }

  //Check if PSP BAR has been assigned, if not do the PSP BAR initialation
  if (PspLibPciReadPspConfig (PSP_PCI_BAR1_REG) == 0) {
    /// Get PSP BAR1 Size
    PspLibPciWritePspConfig (PSP_PCI_BAR1_REG, 0xFFFFFFFF);
    PspMmioSize = PspLibPciReadPspConfig (PSP_PCI_BAR1_REG);
    PspMmioSize = ~PspMmioSize + 1;
    /// Assign BAR1 Temporary Address
    PspLibPciWritePspConfig (PSP_PCI_BAR1_REG, PSP_BAR1_TMP_BASE);
    PspLibPciWritePspConfig ( PSP_PCI_CMD_REG, 0x06);

    /// Enable GNB redirection to this space @todo use equate & also find proper fix
    PspLibPciWriteConfig ( ( (0x18 << 11) + (1 << 8) + 0xBC), ((PSP_BAR1_TMP_BASE + PspMmioSize -1) >> 8) & ~0xFF);
    PspLibPciWriteConfig ( ( (0x18 << 11) + (1 << 8) + 0xB8), (PSP_BAR1_TMP_BASE >> 8) | 3);
    /// Enable MsixBarEn, Bar1En, Bar3En
    PspLibPciWritePspConfig ( PSP_PCI_EXTRAPCIHDR_REG, 0x34);
    /// Capability chain update
    Value32 = PspLibPciReadPspConfig (PSP_PCI_MIRRORCTRL1_REG);
    Value32 &= ~D8F0x44_PmNxtPtrW_MASK;
    Value32 |= 0xA4;
    PspLibPciWritePspConfig (PSP_PCI_MIRRORCTRL1_REG, Value32);
  }

  return (TRUE);
}

/**
  Return the PspMMIO MMIO location

  @param[in] PspMmio Pointer to Psp MMIO address

  @retval BOOLEAN  0: Error, 1 Success
**/
BOOLEAN
GetPspBar1Addr (
  IN OUT   UINT32 *PspMmio
  )
{
  if (CheckPspDevicePresent () == FALSE) {
    return (FALSE);
  }

  *PspMmio = PspLibPciReadPspConfig  (PSP_PCI_BAR1_REG);

  if ((*PspMmio) == 0xffffffff) {
    return (FALSE);
  }

  return (TRUE);
}

/**
  Return the PspMMIO MMIO location

  @param[in] PspMmio Pointer to Psp MMIO address

  @retval BOOLEAN  0: Error, 1 Success
**/
BOOLEAN
GetPspBar3Addr (
  IN OUT   UINT32 *PspMmio
  )
{
  if (CheckPspDevicePresent () == FALSE) {
    return (FALSE);
  }

  *PspMmio = PspLibPciReadPspConfig  (PSP_PCI_BAR3_REG);

  if ((*PspMmio) == 0xffffffff) {
    return (FALSE);
  }

  return (TRUE);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the access width mask for the processor
 *
 *
 * @param[in]     AccessWidth     Access width
 * @retval        Width in number of bytes
 */


UINT8
PspLibAccessWidth (
  IN       ACCESS_WIDTH AccessWidth
  )
{
  UINT8 Width;

  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    Width = 1;
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    Width = 2;
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    Width = 4;
    break;
  case AccessWidth64:
  case AccessS3SaveWidth64:
    Width = 8;
    break;
  default:
    Width = 0;
  }
  return Width;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Read GNB indirect registers
 *
 *
 *
 * @param[in] Address         PCI address of indirect register
 * @param[in] IndirectAddress Offset of indirect register
 * @param[in] Width           Width
 * @param[out] Value           Pointer to value
 */
VOID
PspLibPciIndirectRead (
  IN       PCI_ADDR     Address,
  IN       UINT32       IndirectAddress,
  IN       ACCESS_WIDTH Width,
     OUT   VOID         *Value
  )
{
  UINT32  IndexOffset;

  IndexOffset = PspLibAccessWidth (Width);
  LibAmdPciWrite (Width, Address, &IndirectAddress, NULL);
  Address.AddressValue += IndexOffset;
  LibAmdPciRead (Width, Address, Value, NULL);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Write GNB indirect registers
 *
 *
 *
 * @param[in] Address         PCI address of indirect register
 * @param[in] IndirectAddress Offset of indirect register
 * @param[in] Width           Width
 * @param[in] Value           Pointer to value
 */
VOID
PspLibPciIndirectWrite (
  IN      PCI_ADDR      Address,
  IN      UINT32        IndirectAddress,
  IN      ACCESS_WIDTH  Width,
  IN      VOID          *Value
  )
{
  UINT32  IndexOffset;

  IndexOffset = PspLibAccessWidth (Width);
  LibAmdPciWrite (Width, Address, &IndirectAddress, NULL);
  Address.AddressValue += IndexOffset;
  LibAmdPciWrite (Width, Address, Value, NULL);
}


