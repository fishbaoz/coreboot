/* $NoKeywords:$ */
/**
 * @file
 *
 * Graphics Controller family specific service procedure
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 311507 $   @e \$Date: 2015-01-21 16:57:51 -0600 (Wed, 21 Jan 2015) $
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
#include  "FchPlatform.h"
#include  "Filecode.h"
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 * Default FCH interface settings at InitEnv phase.
 *----------------------------------------------------------------------------------------
 */
CONST FCH_INTERFACE ROMDATA FchInterfaceDefault = {
  SdVer3,              // SdConfig
  AzEnable,            // AzaliaControl
  IrRxTx0Tx1,          // IrConfig
  TRUE,                // UmiGen2
  SataAhci,                   // SataClass
  TRUE,                // SataEnable
  TRUE,                // IdeEnable
  TRUE,                // SataIdeMode
  TRUE,                // Ohci1Enable
  TRUE,                // Ohci2Enable
  TRUE,                // Ohci3Enable
  TRUE,                // Ohci4Enable
  TRUE,                // GppEnable
  AlwaysOff            // FchPowerFail
};


/*----------------------------------------------------------------
 *  InitEnv Phase Data Block Default (Failsafe)
 *----------------------------------------------------------------
 */
FCH_DATA_BLOCK   InitEnvCfgDefault = {
  NULL,                      // StdHeader
  {
    0xF8000000,              ///< PcieMmioBase
    0,                       ///< FchDeviceEnableMap
    0x7FFFFFFF,                       ///< FchDeviceD3ColdMap
    0,                       ///< XhciRomSourceBase
    0,                       ///< XhciRomTargetBase
    0,                       ///< XhciRamSourceBase
    0,                       ///< XhciRamTargetBase
    0,                       ///< SataDevSlpPort0S5Pin - Reserved
    0,                       ///< SataDevSlpPort1S5Pin - Reserved
  },

  {                          // FCH_ACPI
    0xB00,                   // Smbus0BaseAddress
    0xB20,                   // Smbus1BaseAddress
    0xE00,                   // SioPmeBaseAddress
    0xFEC000F0,              // WatchDogTimerBase
    0x800,                   // AcpiPm1EvtBlkAddr
    0x804,                   // AcpiPm1CntBlkAddr
    0x808,                   // AcpiPmTmrBlkAddr
    0x810,                   // CpuControlBlkAddr
    0x820,                   // AcpiGpe0BlkAddr
    0x00B0,                  // SmiCmdPortAddr
    0xFE00,                  // AcpiPmaCntBlkAddr
    TRUE,                    // AnyHt200MhzLink
    FALSE, //TRUE,                    // SpreadSpectrum
    UsePrevious,             // PwrFailShadow
    0,                       // StressResetMode
    FALSE,                   // MtC1eEnable
    NULL,                    // OemProgrammingTablePtr
    1,                       // SpreadSpectrumOptions - Spread Spectrum Option
    FALSE                    // Power down DISP2_CLK and PCIE_RCLK_Output for power savings
  },

  {                          // FCH_AB
    FALSE,                   // AbMsiEnable
    1,                       // ALinkClkGateOff
    1,                       // BLinkClkGateOff
    1,                       // GPP Clock Request.
    1,                       // GPP Clock Request.
    1,                       // GPP Clock Request.
    1,                       // GPP Clock Request.
    1,                       // GPP Clock Request.
    1,                       // AbClockGating
    0,                       // GppClockGating
    0,                       // UmiL1TimerOverride
    0,                       // UmiLinkWidth
    0,                       // UmiDynamicSpeedChange
    0,                       // PcieRefClockOverClocking
    0,                       // UmiGppTxDriverStrength
    FALSE,                   // NbSbGen2
    0,                       // FchPcieOrderRule
    0,                       // SlowSpeedAbLinkClock
    TRUE,                    // ResetCpuOnSyncFlood
    TRUE,                    // AbDmaMemoryWrtie3264B
    FALSE,                   // AbMemoryPowerSaving //ENH453986:A-Link and B-Link memory power saving setting not matching between BKDG and BIOS
    FALSE,                   // SbgDmaMemoryWrtie3264ByteCount
    FALSE,                   // SbgMemoryPowerSaving
    FALSE,                   // SBG Clock Gate *CZ
    FALSE,                   // XDMA DMA Write 16 byte mode *CZ
    FALSE,                   // XDMA memory power saving *CZ
    2,                       // XDMA PENDING NPR THRESHOLD *CZ
    FALSE                    // XDMA DNCPL ORDER DIS *CZ
  },

  {                                  // FCH_GPP
    {FALSE, FALSE, FALSE, FALSE, 0}, // GPP port configuration structure
    2,                               // Gpp Link Mode
    FALSE,                           // GPP Function
    FALSE,                           // Toggle GPP core reset
    0,                               // Hotplug
    0,                               // Gpp Found Gfx Device
    FALSE,                           // GPP Gen2
    0,                               // GPP Gen2 Strap
    FALSE,                           // GPP Memory Write Improve
    FALSE,                           // GPP Unhide Ports
    0,                               // Gpp Port Aspm
    FALSE,                           // GPP Lane Reversal
    FALSE,                           // GPP PHY PLL Power Down
    FALSE,                           // GPP Dynamic Power Saving
    FALSE,                           // PcieAer
    FALSE,                           // PCIe RAS
    FALSE,                           // PCIe Compliance
    FALSE,                           // PCIe Software Down Grade
    FALSE,                           // UMI PHY PLL Power Down
    FALSE,                           // Serial Debug Bus Enable
    0,                               // Gpp Hardware Down Grade
    0,                               // Gpp L1Immediate Ack
    FALSE,                           // New Gpp Algorithm
    0,                               // Hot Plug Ports Status
    0,                               // Fail Ports Status
    0,                               // Gpp PortMin Polling Time
    FALSE                            // Is CapsuleMode
  },

  {                          // FCH_USB
    TRUE,                    // Ohci1Enable
    TRUE,                    // Ohci2Enable
    TRUE,                    // Ohci3Enable
    TRUE,                    // Ohci4Enable
    TRUE,                    // Ehci1Enable
    TRUE,                    // Ehci2Enable
    TRUE,                    // Ehci3Enable
    TRUE,                    // Xhci0Enable
    TRUE,                    // Xhci1Enable
    TRUE,                   // UsbMsiEnable
    0,                       // OhciSsid
    0,                       // Ohci4Ssid
    0,                       // EhciSsid
    0,                       // XhciSsid
    FALSE,                   // UsbPhyPowerDown
    0,                       // UserDefineXhciRomAddr
    {0x21, 0x21, 0x21, 0x21, 0x22},      // Ehci18Phy
    {0x22, 0x22, 0x22, 0x21, 0x21},      // Ehci19Phy
    {0x21, 0x21, 0x21, 0x21},            // Ehci22Phy
    {0x24, 0x24, 0x21, 0x21},            // Xhci20Phy
    0,                                   // Ehci1DebugPortSel
    0,                                   // Ehci2DebugPortSel
    0,                                   // Ehci3DebugPortSel
    4,                                   // Ehci1NPort
    4,                                    // Ehci2NPort
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    0,                        //EhciSimpleDebugPort
    0,                        //UsbBatteryChargeEnable
    0,                        //ReduceUSB3PortToLastTwo
    0,                        //USB30PortInit
  },

  {                          // FCH_SATA
    FALSE,                   // SataMsiEnable
    0x00000000,              // SataIdeSsid
    0x00000000,              // SataRaidSsid
    0x00000000,              // SataRaid5Ssid
    0x00000000,              // SataAhciSsid
    {                        // SATA_ST
      0,                     // SataModeReg
      TRUE,                  // SataEnable
      0,                     // Sata6AhciCap
      TRUE,                  // SataSetMaxGen2
      FALSE,                 // IdeEnable
      01,                    // SataClkMode
    },
    SataAhci,           // SataClass
    0,                       // SataIdeMode
    0,                       // SataDisUnusedIdePChannel
    0,                       // SataDisUnusedIdeSChannel
    0,                       // IdeDisUnusedIdePChannel
    0,                       // IdeDisUnusedIdeSChannel
    0,                       // SataOptionReserved
    {                        // SATA_PORT_ST
      0,                     // SataPortReg
      FALSE,                  // Port0
      FALSE,                  // Port1
      FALSE,                  // Port2
      FALSE,                  // Port3
      FALSE,                  // Port4
      FALSE,                  // Port5
      FALSE,                  // Port6
      FALSE,                  // Port7
    },
    {                        // SATA_PORT_ST
      0,                     // SataPortReg
      FALSE,                 // Port0
      FALSE,                 // Port1
      FALSE,                 // Port2
      FALSE,                 // Port3
      FALSE,                 // Port4
      FALSE,                 // Port5
      FALSE,                 // Port6
      FALSE,                 // Port7
    },
    {                        // SATA_PORT_MD
      0,                     // SataPortMode
      0,                     // Port0
      0,                     // Port1
      0,                     // Port2
      0,                     // Port3
      0,                     // Port4
      0,                     // Port5
      0,                     // Port6
      0,                     // Port7
    },
    1,                       // SataAggrLinkPmCap
    1,                       // SataPortMultCap
    1,                       // SataClkAutoOff
    1,                       // SataPscCap
    0,                       // BiosOsHandOff
    1,                       // SataFisBasedSwitching
    0,                       // SataCccSupport
    1,                       // SataSscCap
    0,                       // SataMsiCapability
    0,                       // SataForceRaid
    0,                       // SataInternal100Spread
    0,                       // SataDebugDummy
    0,                       // SataTargetSupport8Device
    0,                       // SataDisableGenericMode
    FALSE,                   // SataAhciEnclosureManagement
    0,                       // SataSgpio0
    0,                       // SataSgpio1
    0,                       // SataPhyPllShutDown  //ENH453636: [CZ] Enable SATA PHY PLL Shutdown BIOS option by default
    FALSE,                   // SataHotRemovalEnh
    {                        // SATA_PORT_ST
      0,                     // SataPortReg
      FALSE,                 // Port0
      FALSE,                 // Port1
      FALSE,                 // Port2
      FALSE,                 // Port3
      FALSE,                 // Port4
      FALSE,                 // Port5
      FALSE,                 // Port6
      FALSE,                 // Port7
    },
    FALSE,                   // SataOobDetectionEnh
    FALSE,                   // SataPowerSavingEnh
    0,                       // SataMemoryPowerSaving
    FALSE,                   // SataRasSupport
    TRUE,                    // SataAhciDisPrefetchFunction
    FALSE,                   // SataDevSlpPort0
    FALSE,                   // SataDevSlpPort1
    6,                       // SataDbgTX_DRV_STR
    6,                       // SataDbgTX_DE_EMPH_STR
    0,                        // TempMmio
  },

  {                          // FCH_SMBUS
    0x00000000               // SmbusSsid
  },

  {                          // FCH_IDE
    TRUE,                    // IdeEnable
    FALSE,                   // IdeMsiEnable
    0x00000000               // IdeSsid
  },

  {                          // FCH_AZALIA
    AzEnable,               // AzaliaEnable
    FALSE,                   // AzaliaMsiEnable
    0x00000000,              // AzaliaSsid
    1,                       // AzaliaPinCfg
    0,                       // AzaliaFrontPanel
    0,                       // FrontPanelDetected
    0,                       // AzaliaSnoop
    0,                       // AzaliaDummy
    {                        // AZALIA_PIN
      2,                     // AzaliaSdin0
      2,                     // AzaliaSdin1
      2,                     // AzaliaSdin2
      2,                     // AzaliaSdin3
    },
    NULL,                    // *AzaliaOemCodecTablePtr
    NULL,                    // *AzaliaOemFpCodecTablePtr
  },

  {                          // FCH_SPI
    FALSE,                   // LpcMsiEnable
    0x00000000,              // LpcSsid
    0,                       // RomBaseAddress
    0,                       // Speed
    0,                       // FastSpeed
    0,                       // WriteSpeed
    0,                       // Mode
    0,                       // AutoMode
    0,                       // BurstWrite
    TRUE,                    // LpcClk0
    TRUE,                    // LpcClk1
    0,                       // SPI100_Enable
    {                        // Spi Device Profile
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
    }
  },

  {                          // FCH_PCIB
    FALSE,                   // PcibMsiEnable
    0x00000000,              // PcibSsid
    0x0F,                    // PciClks
    0,                       // PcibClkStopOverride
    FALSE,                   // PcibClockRun
  },

  {                          // FCH_GEC
    FALSE,                   // GecEnable
    0,                       // GecPhyStatus
    0,                       // GecPowerPolicy
    0,                       // GecDebugBus
    0xFED61000,              // GecShadowRomBase
    NULL,                    // *PtrDynamicGecRomAddress
  },

  {                          // FCH_SD
    SdDisable,               // SdConfig
    0,                       // Speed
    0,                       // BitWidth
    0x00000000,              // SdSsid
    Sd50MhzTraceCableLengthWithinSixInches,  // SdClockControl
    FALSE,
    0,
    15,                      //SdHostControllerVersion
    3,                       //SdrCapabilities
    0,                       // SdSlotType
    0,                       // SdForce18
    0,                       // SdDbgConfig
  },

  {0},                       // FCH_HWM

  {0,                        // FCH_IR
   0x23,                     // IrPinControl
  },

  {                          // FCH_HPET
    TRUE,                    // HpetEnable
    TRUE,                    // HpetMsiDis
    0xFED00000               // HpetBase
  },

  {                          // FCH_GCPU
    0,                       // AcDcMsg
    0,                       // TimerTickTrack
    0,                       // ClockInterruptTag
    0,                       // OhciTrafficHanding
    0,                       // EhciTrafficHanding
    0,                       // GcpuMsgCMultiCore
    0,                       // GcpuMsgCStage
  },

  {0},                       // FCH_IMC

  {                          // FCH_MISC
    FALSE,                   // NativePcieSupport
    FALSE,                   // S3Resume
    FALSE,                   // RebootRequired
    0,                       // FchVariant
    0,                       // CG2PLL
    {                        // TIMER_SMI-LongTimer
      FALSE,                 // Enable
      FALSE,                 // StartNow
      1000                   // CycleDuration
    },
    {                        // TIMER_SMI-ShortTimer
      FALSE,                 // Enable
      FALSE,                 // StartNow
      0x7FFF                 // CycleDuration
    },
    0,                       // Saving CpuId for FCH Module
    FALSE,                   // NoneSioKbcSupport - No KBC/SIO controller
    {                        // FCH Cs function structure
      FALSE,
      FALSE,
      FALSE,
      FALSE,
      FALSE,
      0,
      0,
      0,
      0,
      0,
    }
  },
  {0, 0},                    // FCH_IOMUX
  NULL,                      // PostOemGpioTable
};


