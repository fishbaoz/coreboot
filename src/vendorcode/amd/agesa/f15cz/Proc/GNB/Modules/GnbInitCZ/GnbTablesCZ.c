/**
 * @file
 *
 * GNB init tables
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-26 11:00:43 +0800 (Thu, 26 Feb 2015) $
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
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbRegistersCZ.h"
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

GNB_TABLE ROMDATA GnbEarlierInitTableBeforeSmuCZ [] = {
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_SMUCsrIsocEn_MASK,
    (1 << D0F0x98_x07_SMUCsrIsocEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x1E_TYPE,
    D0F0x98_x1E_ADDRESS,
    D0F0x98_x1E_HiPriEn_MASK | D0F0x98_x1E_RxErrStatusDelay_MASK,
    (1 << D0F0x98_x1E_HiPriEn_OFFSET) | (0x48 << D0F0x98_x1E_RxErrStatusDelay_OFFSET)
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbEarlyInitTableCZ [] = {
  GNB_ENTRY_RMW (
    D0F0x64_x16_TYPE,
    D0F0x64_x16_ADDRESS,
    D0F0x64_x16_AerUrMsgEn_MASK,
    0x0 << D0F0x64_x16_AerUrMsgEn_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x64_x46_TYPE,
    D0F0x64_x46_ADDRESS,
    D0F0x64_x46_CgttLclkOverride_MASK,
    0x0 << D0F0x64_x46_CgttLclkOverride_OFFSET
    ),
// ACG AZ Host Addressing
  GNB_ENTRY_WR (
    D9F2xF4_x10_TYPE,
    D9F2xF4_x10_ADDRESS,
    0x00010000
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x11_TYPE,
    D9F2xF4_x11_ADDRESS,
    0
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x12_TYPE,
    D9F2xF4_x12_ADDRESS,
    0x00014000
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x13_TYPE,
    D9F2xF4_x13_ADDRESS,
    0
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbEnvInitTableCZ [] = {
//---------------------------------------------------------------------------
// ORB Init
  GNB_ENTRY_RMW (
    D0F0x98_x06_TYPE,
    D0F0x98_x06_ADDRESS,
    D0F0x98_x06_ReqCompatModeDis_MASK | D0F0x98_x06_HtdNoErr_MASK |
    D0F0x98_x06_HostRdRspPassPWMode_MASK | D0F0x98_x06_HostReqSnoopMode_MASK |
    D0F0x98_x06_HostReqPassPWMode_MASK | D0F0x98_x06_UmiNpMemWrEn_MASK,
    (0x1 << D0F0x98_x06_UmiNpMemWrEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IocBwOptEn_MASK | D0F0x98_x07_DropZeroMaskWrEn_MASK,
    (0x1 << D0F0x98_x07_IocBwOptEn_OFFSET) | (0x1 << D0F0x98_x07_DropZeroMaskWrEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IommuBwOptEn_MASK | D0F0x98_x07_IommuIsocPassPWMode_MASK |
    D0F0x98_x07_DmaReqRespPassPWMode_MASK | D0F0x98_x07_SpgBwOptEn_MASK,
    (0x1 << D0F0x98_x07_IommuBwOptEn_OFFSET) | (0x1 << D0F0x98_x07_IommuIsocPassPWMode_OFFSET) |
    (0x1 << D0F0x98_x07_SpgBwOptEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x08_TYPE,
    D0F0x98_x08_ADDRESS,
    D0F0x98_x08_NpWrrLenC_MASK | D0F0x98_x08_NpWrrLenD_MASK |
    D0F0x98_x08_NpWrrLenB_MASK | D0F0x98_x08_NpWrrLenA_MASK,
    (0x1 << D0F0x98_x08_NpWrrLenC_OFFSET) | (0x8 << D0F0x98_x08_NpWrrLenD_OFFSET) |
    (0x8 << D0F0x98_x08_NpWrrLenB_OFFSET) | (0x8 << D0F0x98_x08_NpWrrLenA_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x0C_TYPE,
    D0F0x98_x0C_ADDRESS,
    D0F0x98_x0C_GcmWrrLenA_MASK | D0F0x98_x0C_GcmWrrLenB_MASK,
    (0x8 << D0F0x98_x0C_GcmWrrLenA_OFFSET) | (0x8 << D0F0x98_x0C_GcmWrrLenB_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x28_TYPE,
    D0F0x98_x28_ADDRESS,
    D0F0x98_x28_ForceCoherentIntr_MASK | D0F0x98_x28_ForceFifoFlush_MASK,
    0x1 << D0F0x98_x28_ForceCoherentIntr_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x2C_TYPE,
    D0F0x98_x2C_ADDRESS,
    D0F0x98_x2C_SBDmaActiveMask_MASK |
    D0F0x98_x2C_SBDmaActiveMaskIntFCH_MASK | D0F0x98_x2C_CgttLclkOverride_MASK,
    (0x1 << D0F0x98_x2C_SBDmaActiveMask_OFFSET) |
    (0x1 << D0F0x98_x2C_SBDmaActiveMaskIntFCH_OFFSET)
    ),
//---------------------------------------------------------------------------
// Clump enable for unitID 5 10 21 22 23 for CZ
  GNB_ENTRY_RMW (
    D0F0x98_x3A_TYPE,
    D0F0x98_x3A_ADDRESS,
    0xE00420,
    0xE00420
    ),
  GNB_ENTRY_RMW (
    D18F0x110_TYPE,
    D18F0x110_ADDRESS,
    0xE00422,
    0xE00422
    ),

//---------------------------------------------------------------------------
// Clump enable Onion+
//  GNB_ENTRY_WR (
//    D18F0x114_TYPE,
//    D18F0x114_ADDRESS,
//    0xFFFFFFF2
//    ),
  // Programming a conservative watermark for NBP states
//  GNB_ENTRY_RMW (
//    GMMx6CD8_TYPE,
//    GMMx6CD8_ADDRESS,
//    GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
//    GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
//    GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_MASK,
//    (1 << GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
//    (1 << GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
//    (0x7FFF << GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
//    ),
//
//  GNB_ENTRY_RMW (
//    GMMx6CC8_TYPE,
//    GMMx6CC8_ADDRESS,
//    GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
//    GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
//    (0x3 << GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
//    (0x3 << GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
//    ),
//
//  GNB_ENTRY_RMW (
//    GMMx6CD4_TYPE,
//    GMMx6CD4_ADDRESS,
//    GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
//    (0x7FFF << GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
//    ),
//
//  GNB_ENTRY_RMW (
//    GMMx6CD4_TYPE,
//    GMMx6CD4_ADDRESS,
//    GMMx6CD4_STUTTER_ENABLE_MASK,
//    (1 << GMMx6CD4_STUTTER_ENABLE_OFFSET)
//    ),
//
//  GNB_ENTRY_RMW (
//    GMMx6CD8_TYPE,
//    GMMx6CD8_ADDRESS,
//    GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_MASK,
//    (1 << GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
//    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbMidInitTableCZ [] = {
  //Lock mapping
  GNB_ENTRY_RMW (
    D9F2xF4_x0F_TYPE,
    D9F2xF4_x0F_ADDRESS,
    D9F2xF4_x0F_Bar0MapLock_MASK | D9F2xF4_x0F_Bar1MapLock_MASK,
    (0x1 << D9F2xF4_x0F_Bar0MapLock_OFFSET) | (0x1 << D9F2xF4_x0F_Bar1MapLock_OFFSET)
    ),
  //Enable BAR decoder, lock bar1, bar1 hidden
  GNB_ENTRY_RMW (
    D9F2x48_TYPE,
    D9F2x48_ADDRESS,
    D9F2x48_Bar0En_MASK | D9F2x48_Bar1En_MASK |
    D9F2x48_MsixBarEn_MASK | D9F2x48_Bar1Hide_MASK |
    D9F2x48_Bar1Lock_MASK,
    (0x1 << D9F2x48_Bar0En_OFFSET) | (0x1 << D9F2x48_Bar1En_OFFSET) |
    (0x1 << D9F2x48_Bar1Hide_OFFSET) | (0x1 << D9F2x48_Bar1Lock_OFFSET)
    ),
// ACG AZ DMA Apertures
  GNB_ENTRY_WR (
    D9F2xF4_x28_TYPE,
    D9F2xF4_x28_ADDRESS,
    0
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x29_TYPE,
    D9F2xF4_x29_ADDRESS,
    0
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x2A_TYPE,
    D9F2xF4_x2A_ADDRESS,
    0xFF000000
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x2B_TYPE,
    D9F2xF4_x2B_ADDRESS,
    0xFFFF
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x2C_TYPE,
    D9F2xF4_x2C_ADDRESS,
    0
    ),
  GNB_ENTRY_WR (
    D9F2xF4_x2D_TYPE,
    D9F2xF4_x2D_ADDRESS,
    0
    ),
  GNB_ENTRY_RMW (
    D9F2xF4_x20_TYPE,
    D9F2xF4_x20_ADDRESS,
    D9F2xF4_x20_Aper0En_MASK | D9F2xF4_x20_Aper0Lock_MASK,
    (0x1 << D9F2xF4_x20_Aper0En_OFFSET) | (0x1 << D9F2xF4_x20_Aper0Lock_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D9F2x04_TYPE,
    D9F2x04_ADDRESS,
    D9F2x04_BusMasterEn_MASK,
    (0x1 << D9F2x04_BusMasterEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D9F2x44_TYPE,
    D9F2x44_ADDRESS,
    D9F2x44_PmNxtPtrW_MASK,
    (0xA4 << D9F2x44_PmNxtPtrW_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D9F2xF4_x48_TYPE,
    D9F2xF4_x48_ADDRESS,
    D9F2xF4_x48_InterruptPolarity_MASK,
    (0x1 << D9F2xF4_x48_InterruptPolarity_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D9F2xF4_x49_TYPE,
    D9F2xF4_x49_ADDRESS,
    D9F2xF4_x49_InterruptEnable_MASK,
    (0x1 << D9F2xF4_x49_InterruptEnable_OFFSET)
    ),

//---------------------------------------------------------------------------
// ORB clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x49_TYPE,
    D0F0x98_x49_ADDRESS,
    D0F0x98_x49_SoftOverrideClk6_MASK | D0F0x98_x49_SoftOverrideClk5_MASK |
    D0F0x98_x49_SoftOverrideClk4_MASK | D0F0x98_x49_SoftOverrideClk3_MASK |
    D0F0x98_x49_SoftOverrideClk2_MASK | D0F0x98_x49_SoftOverrideClk1_MASK |
    D0F0x98_x49_SoftOverrideClk0_MASK | D0F0x98_x49_OffHysteresis_MASK,
    (0x10 << D0F0x98_x49_OffHysteresis_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x4A_TYPE,
    D0F0x98_x4A_ADDRESS,
    D0F0x98_x4A_SoftOverrideClk6_MASK | D0F0x98_x4A_SoftOverrideClk5_MASK |
    D0F0x98_x4A_SoftOverrideClk4_MASK | D0F0x98_x4A_SoftOverrideClk3_MASK |
    D0F0x98_x4A_SoftOverrideClk2_MASK | D0F0x98_x4A_SoftOverrideClk1_MASK |
    D0F0x98_x4A_SoftOverrideClk0_MASK | D0F0x98_x4A_OffHysteresis_MASK,
    (0x10 << D0F0x98_x4A_OffHysteresis_OFFSET)
    ),
//---------------------------------------------------------------------------
// IOC clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x22_TYPE,
    D0F0x64_x22_ADDRESS,
    D0F0x64_x22_SoftOverrideClk4_MASK | D0F0x64_x22_SoftOverrideClk3_MASK |
    D0F0x64_x22_SoftOverrideClk2_MASK | D0F0x64_x22_SoftOverrideClk1_MASK |
    D0F0x64_x22_SoftOverrideClk0_MASK | D0F0x64_x22_SoftOverrideClk6_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x23_TYPE,
    D0F0x64_x23_ADDRESS,
    D0F0x64_x23_SoftOverrideClk4_MASK | D0F0x64_x23_SoftOverrideClk3_MASK |
    D0F0x64_x23_SoftOverrideClk2_MASK | D0F0x64_x23_SoftOverrideClk1_MASK |
    D0F0x64_x23_SoftOverrideClk0_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
// gBIF Interrupt & Swizzle
  GNB_ENTRY_RMW (
    D0F0xFC_x0F_TYPE,
    D0F0xFC_x0F_ADDRESS,
    D0F0xFC_x0F_GBIFExtIntrGrp_MASK | D0F0xFC_x0F_GBIFExtIntrSwz_MASK,
    (0x4 << D0F0xFC_x0F_GBIFExtIntrGrp_OFFSET) | (0x1 << D0F0xFC_x0F_GBIFExtIntrSwz_OFFSET)
    ),
// SPG Interrupt & Swizzle
  GNB_ENTRY_RMW (
    D0F0xFC_x30_TYPE,
    D0F0xFC_x30_ADDRESS,
    D0F0xFC_x30_SpgExtIntrGrp_MASK | D0F0xFC_x30_SpgExtIntrSwz_MASK,
    (0x2 << D0F0xFC_x30_SpgExtIntrGrp_OFFSET) | (0x1 << D0F0xFC_x30_SpgExtIntrSwz_OFFSET)
    ),
// ACG Interrupt & Swizzle
  GNB_ENTRY_RMW (
    D0F0xFC_x2F_TYPE,
    D0F0xFC_x2F_ADDRESS,
    D0F0xFC_x2F_ApgPortBExtIntrGrp_MASK | D0F0xFC_x2F_ApgPortBExtIntrSwz_MASK,
    (0x3 << D0F0xFC_x2F_ApgPortBExtIntrGrp_OFFSET) | (0x1 << D0F0xFC_x2F_ApgPortBExtIntrSwz_OFFSET)
    ),
//---------------------------------------------------------------------------
// SPG clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_SPG_CLOCK_GATING,
    D8F0xEC_x4A_TYPE,
    D8F0xEC_x4A_ADDRESS,
    D8F0xEC_x4A_Reserved_30_24_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_SPG_CLOCK_GATING,
    D8F0xEC_x4C_TYPE,
    D8F0xEC_x4C_ADDRESS,
    D8F0xEC_x4C_Reserved_30_24_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
// ACG AZ clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ACG_AZ_CLOCK_GATING,
    D9F2xEC_x4A_TYPE,
    D9F2xEC_x4A_ADDRESS,
    D9F2xEC_x4A_Reserved_30_24_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ACG_AZ_CLOCK_GATING,
    D9F2xEC_x4C_TYPE,
    D9F2xEC_x4C_ADDRESS,
    D9F2xEC_x4C_Reserved_30_24_MASK,
    0x0
    ),

//---------------------------------------------------------------------------
// Enable LCLK Deep Sleep
//  GNB_ENTRY_PROPERTY_RMW (
//    TABLE_PROPERTY_LCLK_DEEP_SLEEP,
//    D0F0xBC_xC0200310_TYPE,
//    D0F0xBC_xC0200310_ADDRESS,
//    D0F0xBC_xC0200310_SMU_BUSY_MASK_MASK |
//    D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_MASK |
//    D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_MASK |
//    D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_MASK |
//    D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_MASK |
//    D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_MASK |
//    D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_MASK |
//    D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_MASK |
//    D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_MASK |
//    D0F0xBC_xC0200310_L2IMU_IDLE_MASK_MASK |
//    D0F0xBC_xC0200310_ORB_IDLE_MASK_MASK |
//    D0F0xBC_xC0200310_ON_INB_WAKE_MASK_MASK |
//    D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_MASK |
//    D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_MASK |
//    D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_MASK |
//    D0F0xBC_xC0200310_DMAACTIVE_MASK_MASK |
//    D0F0xBC_xC0200310_RFE_BUSY_MASK_MASK |
//    D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_MASK |
//    D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_MASK,
//    ( 1 << D0F0xBC_xC0200310_SMU_BUSY_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_L2IMU_IDLE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_ORB_IDLE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_ON_INB_WAKE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_DMAACTIVE_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_RFE_BUSY_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_OFFSET) |
//    ( 1 << D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_OFFSET)
//    ),
//  GNB_ENTRY_PROPERTY_RMW (
//    TABLE_PROPERTY_LCLK_DEEP_SLEEP,
//    D0F0xBC_xC020008C_TYPE,
//    D0F0xBC_xC020008C_ADDRESS,
//    D0F0xBC_xC020008C_DIV_ID_MASK |
//    D0F0xBC_xC020008C_RAMP_DIS_MASK |
//    D0F0xBC_xC020008C_HYSTERESIS_MASK,
//    ( 5 << D0F0xBC_xC020008C_DIV_ID_OFFSET) |
//    ( 0 << D0F0xBC_xC020008C_RAMP_DIS_OFFSET) |
//    ( 0xF << D0F0xBC_xC020008C_HYSTERESIS_OFFSET)
//    ),
//  GNB_ENTRY_PROPERTY_RMW (
//    TABLE_PROPERTY_IGFX_DISABLED,
//    D0F0xBC_xC0200310_TYPE,
//    D0F0xBC_xC0200310_ADDRESS,
//    D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_MASK,
//    0x0
//    ),
//  GNB_ENTRY_PROPERTY_RMW (
//    TABLE_PROPERTY_LCLK_DEEP_SLEEP,
//    D0F0xBC_xC020008C_TYPE,
//    D0F0xBC_xC020008C_ADDRESS,
//    D0F0xBC_xC020008C_ENABLE_DS_MASK,
//    (0x1 << D0F0xBC_xC020008C_ENABLE_DS_OFFSET)
//    ),
//---------------------------------------------------------------------------
  GNB_ENTRY_TERMINATE
};
